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

extern const char *const VIAL_SOCKET_ERROR;

enum vial_socket_af { VIAL_SOCKET_AF_IP4, VIAL_SOCKET_AF_IP6 };

struct vial_socket_addr {
	enum vial_socket_af family;
	uint16_t port;
	union {
		uint32_t v4;
		uint8_t v6[16];
	} addr;
};

struct vial_socket {
	union {
		int fd;
		char cs[16];
	} socket;
};

enum vial_socket_type { VIAL_SOCKET_TYPE_STREAM, VIAL_SOCKET_TYPE_DGRAM };

enum vial_socket_shut { VIAL_SOCKET_SHUT_RECV, VIAL_SOCKET_SHUT_SEND, VIAL_SOCKET_SHUT_BOTH };

vial_error vial_socket_addr_resolve(const char *host, uint16_t port, struct vial_socket_addr **addresses, size_t *count);

vial_error vial_socket_addr_init(struct vial_socket_addr *self, const char *addr, uint16_t port);

vial_error vial_socket_addr_stringify(const struct vial_socket_addr *self, char *str);

int vial_socket_startup(void);

int vial_socket_cleanup(void);

vial_error vial_socket_init(struct vial_socket *self, enum vial_socket_af domain, enum vial_socket_type type);

vial_error vial_socket_initconn(struct vial_socket *self, const char *host, uint16_t port);

vial_error vial_socket_accept(struct vial_socket *self, struct vial_socket *socket, struct vial_socket_addr *address);

vial_error vial_socket_available(struct vial_socket *self, size_t *available);

vial_error vial_socket_bind(struct vial_socket *self, struct vial_socket *socket, const struct vial_socket_addr *address);

vial_error vial_socket_close(struct vial_socket *self);

vial_error vial_socket_connect(struct vial_socket *self, const struct vial_socket_addr *address);

vial_error vial_socket_listen(struct vial_socket *self, int backlog);

vial_error vial_socket_recv(struct vial_socket *self, void *buffer, size_t *length);

vial_error vial_socket_recvfrom(struct vial_socket *self, void *buffer, size_t *length, struct vial_socket_addr *address);

vial_error vial_socket_send(struct vial_socket *self, const void *buffer, size_t *length);

vial_error vial_socket_sendto(struct vial_socket *self, const void *buffer, size_t *length, const struct vial_socket_addr *address);

vial_error vial_socket_shutdown(struct vial_socket *self, enum vial_socket_shut how);

#endif
