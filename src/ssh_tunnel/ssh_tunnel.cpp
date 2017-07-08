extern "C" {
#include "ssh_common.c"
}
#include "ssh_tunnel.h"
#include <QProcess>
#include <QTcpSocket>

SSH_Tunnel::SSH_Tunnel(QObject *parent) :
    QThread(parent)
{

}

void SSH_Tunnel::setData(QVariantMap _data)
{

}
void SSH_Tunnel::run()
{
    /*
    ssh_session session;
    ssh_channel channel;
    char buffer[256];
    int nbytes;
    int rc;

    session = connect_ssh("stationar", "root", 0);
    if (session == nullptr) {
        return;
    };

    channel = ssh_channel_new(session);
    if (channel == nullptr) {
        ssh_disconnect(session);
        ssh_free(session);
        return;
    };

    rc = ssh_channel_open_session(channel);
    if (rc < 0) {
        goto failed;
    };

    rc = ssh_channel_request_exec(channel, "lsof");
    if (rc < 0) {
        goto failed;
    };

    nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
    while (nbytes > 0) {
        if (fwrite(buffer, 1, nbytes, stdout) != (unsigned int) nbytes) {
            goto failed;
        };
        nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
    };

    if (nbytes < 0) {
        goto failed;
    };

    ssh_channel_send_eof(channel);
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    ssh_free(session);

    return;
failed:
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    ssh_disconnect(session);
    ssh_free(session);

    return;
    */
    QProcess ssh_tunnel;
    ssh_tunnel.setProcessChannelMode(QProcess::SeparateChannels);
    connect(&ssh_tunnel, SIGNAL(readyRead()),
            this, SLOT(write_to_viewer()));
    ssh_tunnel.start();
    bool started = ssh_tunnel.waitForStarted();
    emit connection_result(started, 0);
    ssh_tunnel.waitForFinished(-1);
}

/* private slots */
uint SSH_Tunnel::get_free_port_for_listen()
{
    return 0;
}
void SSH_Tunnel::write_to_viewer()
{

}
void SSH_Tunnel::write_to_remote_graphic_channel()
{

}
