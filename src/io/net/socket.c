/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <vial/io/net/socket.h>

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <vial/foreach.h>

#if defined(_WIN32) || defined(_WIN64)
const char *inet_ntop(int af, const void *src, char *dst, size_t size);
int inet_pton(int af, const char *src, void *dst);
#include <winsock2.h>
#include <ws2tcpip.h>
#define VIAL_WINSOCK_VERSION ((2 << 8) + 2)
#define return_error return get_error(WSAGetLastError(), __FILE__, __LINE__)
typedef SOCKET socket_t;
#else
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#define return_error return get_error(errno, __FILE__, __LINE__)
typedef int socket_t;
#endif

#ifndef EAFNOSUPPORT
#define EAFNOSUPPORT WSAEAFNOSUPPORT
#endif
#ifndef EPROTOTYPE
#define EPROTOTYPE WSAEPROTOTYPE
#endif
#ifndef EINVAL
#define EINVAL WSAEINVAL
#endif

#ifndef SHUT_RD
#define SHUT_RD SD_RECEIVE
#endif
#ifndef SHUT_WR
#define SHUT_WR SD_SEND
#endif
#ifndef SHUT_RDWR
#define SHUT_RDWR SD_BOTH
#endif

const char *const VSOCKET_ERROR = "VSOCKET_ERROR";

#ifdef VIAL_WINSOCK_VERSION
typedef struct {
	short sin6_family;
	uint16_t sin6_port;
	uint32_t sin6_flowinfo;
	struct { char a[16]; } sin6_addr;
	uint32_t sin6_scope_id;
} sockaddr_in6_t;
#else
typedef struct sockaddr_in6 sockaddr_in6_t;
#endif

union vsockaddr {
	struct sockaddr_in in;
	sockaddr_in6_t in6;
};

static error_t get_error(int err, const char *file, int line)
{
	char text[32];
	sprintf(text, "%d", err);
	return error_make(VSOCKET_ERROR, text, NULL, file, line);
}

static socket_t get_socket(const struct vSocket *self)
{
#ifdef VIAL_WINSOCK_VERSION
	socket_t socket;
	memcpy(&socket, self->socket.cs, sizeof(socket));
	return socket;
#else
	return self->socket.fd;
#endif
}

static void set_socket(struct vSocket *self, socket_t socket)
{
#ifdef VIAL_WINSOCK_VERSION
	memcpy(self->socket.cs, &socket, sizeof(socket));
#else
	self->socket.fd = socket;
#endif
}

static error_t get_sockaddr(const struct vSocketAddr *self, union vsockaddr *addr)
{
	memset(addr, 0, sizeof(*addr));
	if (self->family == vSocketAF_IP4)
		addr->in.sin_family = AF_INET;
	else if (self->family == vSocketAF_IP6)
		addr->in.sin_family = AF_INET6;
	else
		return get_error(EAFNOSUPPORT, __FILE__, __LINE__);
	addr->in.sin_port = htons(self->port);
	if (self->family == vSocketAF_IP6)
		memcpy(&addr->in6.sin6_addr, &self->addr.v6, sizeof(addr->in6.sin6_addr));
	else
		memcpy(&addr->in.sin_addr, &self->addr.v4, sizeof(addr->in.sin_addr));
	return NULL;
}

static error_t set_sockaddr(struct vSocketAddr *self, const union vsockaddr *addr)
{
	if (addr->in.sin_family == AF_INET)
		self->family = vSocketAF_IP4;
	else if (addr->in.sin_family == AF_INET6)
		self->family = vSocketAF_IP6;
	else
		return get_error(EAFNOSUPPORT, __FILE__, __LINE__);
	self->port = ntohs(addr->in.sin_port);
	if (self->family == vSocketAF_IP6)
		memcpy(&self->addr.v6, &addr->in6.sin6_addr, sizeof(addr->in6.sin6_addr));
	else
		memcpy(&self->addr.v4, &addr->in.sin_addr, sizeof(addr->in.sin_addr));
	return NULL;
}

static size_t get_addrinfo_length(const struct addrinfo *addr)
{
	size_t length;
	for (length = 0; addr; addr = addr->ai_next)
		length++;
	return length;
}

error_t vSocketAddr_resolve(const char *host, uint16_t port, struct vSocketAddr **addresses, size_t *count)
{
	error_t e;
	struct addrinfo *cur_addr_info, *addr_info = NULL;
	struct vSocketAddr *cur_addr;
	char port_str[8];
	sprintf(port_str, "%d", (int) port);
	if (getaddrinfo(host, port_str, NULL, &addr_info))
		return_error;
	*count = get_addrinfo_length(addr_info);
	if (*count == 0) {
		*addresses = NULL;
		freeaddrinfo(addr_info);
		return NULL;
	}
	cur_addr = *addresses = calloc(*count, sizeof(**addresses));
	for (cur_addr_info = addr_info; cur_addr_info; cur_addr_info = cur_addr_info->ai_next) {
		e = set_sockaddr(cur_addr, (union vsockaddr *) cur_addr_info->ai_addr);
		if (e) {
			free(*addresses);
			freeaddrinfo(addr_info);
			*addresses = NULL;
			error_rethrow(e);
		}
		cur_addr->port = port;
		cur_addr++;
	}
	freeaddrinfo(addr_info);
	return NULL;
}

error_t vSocketAddr_init(struct vSocketAddr *self, const char *addr, uint16_t port)
{
	memset(self, 0, sizeof(*self));
	if (inet_pton(AF_INET6, addr, &self->addr.v6) == 1)
		self->family = vSocketAF_IP4;
	else if (inet_pton(AF_INET, addr, &self->addr.v4) == 1)
		self->family = vSocketAF_IP6;
	else
		return get_error(EINVAL, __FILE__, __LINE__);
	self->port = port;
	return NULL;
}

error_t vSocketAddr_stringify(const struct vSocketAddr *self, char *str)
{
	union vsockaddr saddr;
	char addr_str[64] = "";
	int port = self->port;
	error_rethrow(get_sockaddr(self, &saddr));
	if (self->family == vSocketAF_IP4) {
		if (inet_ntop(saddr.in.sin_family, &saddr.in.sin_addr, addr_str, sizeof(addr_str)) == NULL)
			return_error;
		sprintf(str, "%s:%d", addr_str, port);
	} else {
		if (inet_ntop(saddr.in6.sin6_family, &saddr.in6.sin6_addr, addr_str, sizeof(addr_str)) == NULL)
			return_error;
		sprintf(str, "[%s]:%d", addr_str, port);
	}
	return NULL;
}

int vSocket_startup(void)
{
#ifdef VIAL_WINSOCK_VERSION
	WSADATA wsa_data;
	return WSAStartup(VIAL_WINSOCK_VERSION, &wsa_data);
#else
	return 0;
#endif
}

int vSocket_cleanup(void)
{
#ifdef VIAL_WINSOCK_VERSION
	return WSACleanup();
#endif
}

error_t vSocket_init(struct vSocket *self, enum vSocketAF domain, enum vSocketType type)
{
	int idomain, itype;
	if (domain == vSocketAF_IP4)
		idomain = AF_INET;
	else if (domain == vSocketAF_IP6)
		idomain = AF_INET6;
	else
		return get_error(EAFNOSUPPORT, __FILE__, __LINE__);
	if (type == vSocketType_STREAM)
		itype = SOCK_STREAM;
	else if (type == vSocketType_STREAM)
		itype = SOCK_DGRAM;
	else
		return get_error(EPROTOTYPE, __FILE__, __LINE__);
	socket_t s = socket(idomain, itype, 0);
#ifdef VIAL_WINSOCK_VERSION
	if (s == INVALID_SOCKET)
		return_error;
#else
	if (s < 0)
		return_error;
#endif
	set_socket(self, s);
	return NULL;
}

error_t vSocket_initconn(struct vSocket *self, const char *host, uint16_t port)
{
	error_t e;
	struct vSocketAddr *addrs, *addr;
	size_t addr_len;
	error_rethrow(vSocketAddr_resolve(host, port, &addrs, &addr_len));
	foreach(addr, addrs, addr_len) {
		e = vSocket_init(self, addr->family, vSocketType_STREAM);
		if (e) {
			error_free(e);
			continue;
		}
		e = vSocket_connect(self, addr);
		if (e) {
			error_free(e);
			vSocket_close(self);
			continue;
		}
		return NULL;
	}
	return error_new(VSOCKET_ERROR, "Not found", NULL);
}

error_t vSocket_accept(struct vSocket *self, struct vSocket *socket, struct vSocketAddr *address)
{
	union vsockaddr addr;
	size_t size = sizeof(addr);
	socket_t s = accept(get_socket(self), (struct sockaddr *) &addr, (void*)&size);
#ifdef VIAL_WINSOCK_VERSION
	if (s == INVALID_SOCKET)
		return_error;
#else
	if (s < 0)
		return_error;
#endif
	set_socket(socket, s);
	if (address)
		error_rethrow(set_sockaddr(address, &addr));
	return NULL;
}

error_t vSocket_available(struct vSocket *self, size_t *available)
{
#ifdef VIAL_WINSOCK_VERSION
	u_long len = 0;
	if (ioctlsocket(get_socket(self), FIONREAD, &len))
		return_error;
#else
	size_t len = 0;
	if (ioctl(get_socket(self), FIONREAD, &len))
		return_error;
#endif
	*available = len;
	return NULL;
}

error_t vSocket_bind(struct vSocket *self, struct vSocket *socket, const struct vSocketAddr *address)
{
	union vsockaddr addr;
	error_rethrow(get_sockaddr(address, &addr));
	socket_t s = bind(get_socket(self), (struct sockaddr *) &addr, sizeof(addr));
#ifdef VIAL_WINSOCK_VERSION
	if (s == INVALID_SOCKET)
		return_error;
#else
	if (s < 0)
		return_error;
#endif
	set_socket(socket, s);
	return NULL;
}

error_t vSocket_close(struct vSocket *self)
{
	int res;
#ifdef VIAL_WINSOCK_VERSION
	res = closesocket(get_socket(self));
#else
	res = close(get_socket(self));
#endif
	if (res)
		return_error;
	return NULL;
}

error_t vSocket_connect(struct vSocket *self, const struct vSocketAddr *address)
{
	union vsockaddr addr;
	error_rethrow(get_sockaddr(address, &addr));
	if (connect(get_socket(self), (struct sockaddr *) &addr, sizeof(addr)))
		return_error;
	return NULL;
}

error_t vSocket_listen(struct vSocket *self, int backlog)
{
	if (listen(get_socket(self), backlog))
		return_error;
	return NULL;
}

error_t vSocket_recv(struct vSocket *self, void *buffer, size_t *length)
{
	int res = recv(get_socket(self), buffer, *length, 0);
	if (res < 0) {
		*length = 0;
		return_error;
	}
	*length = res;
	return NULL;
}

error_t vSocket_recvfrom(struct vSocket *self, void *buffer, size_t *length, struct vSocketAddr *address)
{
	union vsockaddr addr;
	size_t size = sizeof(addr);
	int res = recvfrom(get_socket(self), buffer, *length, 0, (struct sockaddr *) &addr, (void*)&size);
	if (res < 0) {
		*length = 0;
		return_error;
	}
	*length = res;
	if (address)
		error_rethrow(set_sockaddr(address, &addr));
	return NULL;
}

error_t vSocket_send(struct vSocket *self, const void *buffer, size_t *length)
{
	int res = send(get_socket(self), buffer, *length, 0);
	if (res < 0) {
		*length = 0;
		return_error;
	}
	*length = res;
	return NULL;
}

error_t vSocket_sendto(struct vSocket *self, const void *buffer, size_t *length, const struct vSocketAddr *address)
{
	union vsockaddr addr;
	error_rethrow(get_sockaddr(address, &addr));
	int res = sendto(get_socket(self), buffer, *length, 0, (struct sockaddr *) &addr, sizeof(addr));
	if (res < 0) {
		*length = 0;
		return_error;
	}
	*length = res;
	return NULL;
}

error_t vSocket_shutdown(struct vSocket *self, enum vSocketShutdown how)
{
	int ihow;
	if (how == vSocketShutdown_RECV)
		ihow = SHUT_RD;
	else if (how == vSocketShutdown_SEND)
		ihow = SHUT_WR;
	else if (how == vSocketShutdown_BOTH)
		ihow = SHUT_RDWR;
	else
		return get_error(EINVAL, __FILE__, __LINE__);
	if (shutdown(get_socket(self), ihow))
		return_error;
	return NULL;
}
