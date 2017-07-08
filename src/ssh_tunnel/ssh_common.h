/*
Copyright 2009 Aris Adamantiadis

This file is part of the SSH Library

You are free to copy this file, modify it in any way, consider it being public
domain. This does not apply to the rest of the library though, but it is
allowed to cut-and-paste working code from this file to any license of
program.
The goal is to show the API in action. It's not a reference on how terminal
clients must be made or how a client should react.
*/

#ifndef SSH_COMMON_H
#define SSH_COMMON_H

#include <libssh.h>
static int authenticate_console(ssh_session session);
static int authenticate_kbdint(ssh_session session, const char *password);
static int verify_knownhost(ssh_session session);
static ssh_session connect_ssh(const char *hostname, const char *user, int verbosity);

#endif /* SSH_COMMON_H */
