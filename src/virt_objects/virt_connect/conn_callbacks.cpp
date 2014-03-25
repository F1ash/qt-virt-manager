#include "conn_callbacks.h"

void connectCloseCallback(virConnectPtr _conn ATTRIBUTE_UNUSED,
                          int   reason,
                          void *opaque ATTRIBUTE_UNUSED)
{
    switch (reason) {
    case VIR_CONNECT_CLOSE_REASON_ERROR:
        fprintf(stderr, "Connection closed due to I/O error");
        break;
    case VIR_CONNECT_CLOSE_REASON_EOF:
        fprintf(stderr, "Connection closed due to end of file");
        break;
    case VIR_CONNECT_CLOSE_REASON_KEEPALIVE:
        fprintf(stderr, "Connection closed due to keepalive timeout");
        break;
    case VIR_CONNECT_CLOSE_REASON_CLIENT:
        fprintf(stderr, "Connection closed due to client request");
        break;
    default:
        fprintf(stderr, "Connection closed due to unknown reason");
        break;
    };
}
