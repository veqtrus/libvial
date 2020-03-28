/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_IO_NET_SOCKET_H
#define VIAL_IO_NET_SOCKET_H

#include <stddef.h>
#include <stdint.h>

#include <vial/error.h>

extern const char *const VSOCKET_ERROR;

enum vSocketAF { vSocketAF_IP4, vSocketAF_IP6 };

struct vSocketAddr {
	enum vSocketAF family;
	uint16_t port;
	union {
		uint32_t v4;
		uint8_t v6[16];
	} addr;
};

struct vSocket {
	union {
		int fd;
		char cs[16];
	} socket;
};

enum vSocketType { vSocketType_STREAM, vSocketType_DGRAM };

enum vSocketShutdown { vSocketShutdown_RECV, vSocketShutdown_SEND, vSocketShutdown_BOTH };

error_t vSocketAddr_resolve(const char *host, uint16_t port, struct vSocketAddr **addresses, size_t *count);

error_t vSocketAddr_init(struct vSocketAddr *self, const char *addr, uint16_t port);

error_t vSocketAddr_stringify(const struct vSocketAddr *self, char *str);

int vSocket_startup(void);

int vSocket_cleanup(void);

error_t vSocket_init(struct vSocket *self, enum vSocketAF domain, enum vSocketType type);

error_t vSocket_initconn(struct vSocket *self, const char *host, uint16_t port);

error_t vSocket_accept(struct vSocket *self, struct vSocket *socket, struct vSocketAddr *address);

error_t vSocket_available(struct vSocket *self, size_t *available);

error_t vSocket_bind(struct vSocket *self, struct vSocket *socket, const struct vSocketAddr *address);

error_t vSocket_close(struct vSocket *self);

error_t vSocket_connect(struct vSocket *self, const struct vSocketAddr *address);

error_t vSocket_listen(struct vSocket *self, int backlog);

error_t vSocket_recv(struct vSocket *self, void *buffer, size_t *length);

error_t vSocket_recvfrom(struct vSocket *self, void *buffer, size_t *length, struct vSocketAddr *address);

error_t vSocket_send(struct vSocket *self, const void *buffer, size_t *length);

error_t vSocket_sendto(struct vSocket *self, const void *buffer, size_t *length, const struct vSocketAddr *address);

error_t vSocket_shutdown(struct vSocket *self, enum vSocketShutdown how);

#endif
