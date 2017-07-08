
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <libssh.h>

int authenticate_kbdint(ssh_session session, const char *password) {
    int err;

    err = ssh_userauth_kbdint(session, NULL, NULL);
    while (err == SSH_AUTH_INFO) {
        const char *instruction;
        const char *name;
        char buffer[128];
        int i, n;

        name = ssh_userauth_kbdint_getname(session);
        instruction = ssh_userauth_kbdint_getinstruction(session);
        n = ssh_userauth_kbdint_getnprompts(session);

        if (name && strlen(name) > 0) {
            printf("%s\n", name);
        }

        if (instruction && strlen(instruction) > 0) {
            printf("%s\n", instruction);
        }

        for (i = 0; i < n; i++) {
            const char *answer;
            const char *prompt;
            char echo;

            prompt = ssh_userauth_kbdint_getprompt(session, i, &echo);
            if (prompt == NULL) {
                break;
            }

            if (echo) {
                char *p;

                printf("%s", prompt);

                if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                    return SSH_AUTH_ERROR;
                }

                buffer[sizeof(buffer) - 1] = '\0';
                if ((p = strchr(buffer, '\n'))) {
                    *p = '\0';
                }

                if (ssh_userauth_kbdint_setanswer(session, i, buffer) < 0) {
                    return SSH_AUTH_ERROR;
                }

                memset(buffer, 0, strlen(buffer));
            } else {
                if (password && strstr(prompt, "Password:")) {
                    answer = password;
                } else {
                    buffer[0] = '\0';

                    if (ssh_getpass(prompt, buffer, sizeof(buffer), 0, 0) < 0) {
                        return SSH_AUTH_ERROR;
                    }
                    answer = buffer;
                }
                err = ssh_userauth_kbdint_setanswer(session, i, answer);
                memset(buffer, 0, sizeof(buffer));
                if (err < 0) {
                    return SSH_AUTH_ERROR;
                }
            }
        }
        err=ssh_userauth_kbdint(session,NULL,NULL);
    }

    return err;
}

static void error(ssh_session session){
        fprintf(stderr,"Authentication failed: %s\n",ssh_get_error(session));
}

int authenticate_console(ssh_session session){
  int rc;
  int method;
  char password[128] = {0};
  char *banner;

  // Try to authenticate
  rc = ssh_userauth_none(session, NULL);
  if (rc == SSH_AUTH_ERROR) {
    error(session);
    return rc;
  }

  method = ssh_userauth_list(session, NULL);
  while (rc != SSH_AUTH_SUCCESS) {
        if (method & SSH_AUTH_METHOD_GSSAPI_MIC){
                rc = ssh_userauth_gssapi(session);
                if(rc == SSH_AUTH_ERROR) {
                        error(session);
                        return rc;
                } else if (rc == SSH_AUTH_SUCCESS) {
                        break;
                }
        }
    // Try to authenticate with public key first
    if (method & SSH_AUTH_METHOD_PUBLICKEY) {
      rc = ssh_userauth_publickey_auto(session, NULL, NULL);
      if (rc == SSH_AUTH_ERROR) {
          error(session);
          return rc;
      } else if (rc == SSH_AUTH_SUCCESS) {
        break;
      }
    }

    // Try to authenticate with keyboard interactive";
    if (method & SSH_AUTH_METHOD_INTERACTIVE) {
      rc = authenticate_kbdint(session, NULL);
      if (rc == SSH_AUTH_ERROR) {
        error(session);
        return rc;
      } else if (rc == SSH_AUTH_SUCCESS) {
        break;
      }
    }

    if (ssh_getpass("Password: ", password, sizeof(password), 0, 0) < 0) {
        return SSH_AUTH_ERROR;
    }

    // Try to authenticate with password
    if (method & SSH_AUTH_METHOD_PASSWORD) {
      rc = ssh_userauth_password(session, NULL, password);
      if (rc == SSH_AUTH_ERROR) {
        error(session);
        return rc;
      } else if (rc == SSH_AUTH_SUCCESS) {
        break;
      }
    }
    memset(password, 0, sizeof(password));
  }

  banner = ssh_get_issue_banner(session);
  if (banner) {
    printf("%s\n",banner);
    ssh_string_free_char(banner);
  }

  return rc;
}

#ifdef _WIN32
#define strncasecmp _strnicmp
#endif

int verify_knownhost(ssh_session session){
  char *hexa;
  int state;
  char buf[10];
  unsigned char *hash = NULL;
  size_t hlen;
  ssh_key srv_pubkey;
  int rc;

  state=ssh_is_server_known(session);

  rc = ssh_get_publickey(session, &srv_pubkey);
  if (rc < 0) {
      return -1;
  }

  rc = ssh_get_publickey_hash(srv_pubkey,
                              SSH_PUBLICKEY_HASH_SHA1,
                              &hash,
                              &hlen);
  ssh_key_free(srv_pubkey);
  if (rc < 0) {
      return -1;
  }

  switch(state){
    case SSH_SERVER_KNOWN_OK:
      break; /* ok */
    case SSH_SERVER_KNOWN_CHANGED:
      fprintf(stderr,"Host key for server changed : server's one is now :\n");
      ssh_print_hexa("Public key hash",hash, hlen);
      ssh_clean_pubkey_hash(&hash);
      fprintf(stderr,"For security reason, connection will be stopped\n");
      return -1;
    case SSH_SERVER_FOUND_OTHER:
      fprintf(stderr,"The host key for this server was not found but an other type of key exists.\n");
      fprintf(stderr,"An attacker might change the default server key to confuse your client"
          "into thinking the key does not exist\n"
          "We advise you to rerun the client with -d or -r for more safety.\n");
      return -1;
    case SSH_SERVER_FILE_NOT_FOUND:
      fprintf(stderr,"Could not find known host file. If you accept the host key here,\n");
      fprintf(stderr,"the file will be automatically created.\n");
      /* fallback to SSH_SERVER_NOT_KNOWN behavior */
    case SSH_SERVER_NOT_KNOWN:
      hexa = ssh_get_hexa(hash, hlen);
      fprintf(stderr,"The server is unknown. Do you trust the host key ?\n");
      fprintf(stderr, "Public key hash: %s\n", hexa);
      ssh_string_free_char(hexa);
      if (fgets(buf, sizeof(buf), stdin) == NULL) {
            ssh_clean_pubkey_hash(&hash);
        return -1;
      }
      if(strncasecmp(buf,"yes",3)!=0){
            ssh_clean_pubkey_hash(&hash);
        return -1;
      }
      fprintf(stderr,"This new key will be written on disk for further usage. do you agree ?\n");
      if (fgets(buf, sizeof(buf), stdin) == NULL) {
            ssh_clean_pubkey_hash(&hash);
        return -1;
      }
      if(strncasecmp(buf,"yes",3)==0){
        if (ssh_write_knownhost(session) < 0) {
          ssh_clean_pubkey_hash(&hash);
          fprintf(stderr, "error %s\n", strerror(errno));
          return -1;
        }
      }

      break;
    case SSH_SERVER_ERROR:
      ssh_clean_pubkey_hash(&hash);
      fprintf(stderr,"%s",ssh_get_error(session));
      return -1;
  }
  ssh_clean_pubkey_hash(&hash);
  return 0;
}

ssh_session connect_ssh(const char *host, const char *user, int verbosity){
  ssh_session session;
  int auth=0;

  session=ssh_new();
  if (session == NULL) {
    return NULL;
  }

  if(user != NULL){
    if (ssh_options_set(session, SSH_OPTIONS_USER, user) < 0) {
      ssh_free(session);
      return NULL;
    }
  }

  if (ssh_options_set(session, SSH_OPTIONS_HOST, host) < 0) {
    ssh_free(session);
    return NULL;
  }
  ssh_options_set(session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
  if(ssh_connect(session)){
    fprintf(stderr,"Connection failed : %s\n",ssh_get_error(session));
    ssh_disconnect(session);
    ssh_free(session);
    return NULL;
  }
  if(verify_knownhost(session)<0){
    ssh_disconnect(session);
    ssh_free(session);
    return NULL;
  }
  auth=authenticate_console(session);
  if(auth==SSH_AUTH_SUCCESS){
    return session;
  } else if(auth==SSH_AUTH_DENIED){
    fprintf(stderr,"Authentication failed\n");
  } else {
    fprintf(stderr,"Error while authenticating : %s\n",ssh_get_error(session));
  }
  ssh_disconnect(session);
  ssh_free(session);
  return NULL;
}
