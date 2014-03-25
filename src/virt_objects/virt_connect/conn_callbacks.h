#ifndef CONN_CALLBACKS_H
#define CONN_CALLBACKS_H

#include "stdio.h"
#include "libvirt/libvirt.h"
#include "libvirt/virterror.h"

#ifndef ATTRIBUTE_UNUSED
# define ATTRIBUTE_UNUSED __attribute__((__unused__))
#endif

void connectCloseCallback(virConnectPtr, int, void*);

#endif // CONN_CALLBACKS_H
