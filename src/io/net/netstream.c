/*
Copyright (c) 2019 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <vial/io/net/netstream.h>

#include <string.h>

#include <vial/io/net/socket.h>
#include <vial/io/net/netstream.h>
#include <vial/utime.h>

#define _self ((struct vial_netstream *) self)

static void impl_dispose(void *self)
{
	memset(self, 0, sizeof(struct vial_netstream));
}

static vial_error impl_capabilities(void *self, int *capabilities)
{
	*capabilities = VIAL_STREAM_CAN_READ | VIAL_STREAM_CAN_WRITE | VIAL_STREAM_CAN_AVAILABLE;
	return NULL;
}

static vial_error impl_close(void *self)
{
	vial_error e = vial_socket_close(&_self->socket);
	if (e)
		vial_error_throw(vial_stream_io_error_new(NULL, NULL));
	memset(&_self->socket, 0, sizeof(_self->socket));
	return NULL;
}

static vial_error impl_flush(void *self) { return NULL; }

static vial_error impl_available(void *self, size_t *available)
{
	vial_error e = vial_socket_available(&_self->socket, available);
	if (e)
		vial_error_throw(vial_stream_io_error_new(NULL, NULL));
	return NULL;
}

static vial_error impl_read(void *self, void *buf, size_t size)
{
	vial_error e;
	size_t done = 0, read = 0;
	char *p = buf;
	uint64_t t0 = vial_mtime();
	while (done < size) {
		read = size - done;
		e = vial_socket_recv(&_self->socket, p, &read);
		if (e)
			vial_error_throw(vial_stream_io_error_new(NULL, NULL));
		if (read > 0) {
			p += read;
			done += read;
			t0 = vial_mtime();
			continue;
		}
		if (vial_mtime() > t0 + _self->read_timeout)
			vial_error_throw(vial_stream_io_error_new("Read timeout", NULL));
		vial_msleep(1);
	}
	return NULL;
}

static vial_error impl_write(void *self, const void *buf, size_t size)
{
	vial_error e;
	size_t done = 0, write = 0;
	const char *p = buf;
	uint64_t t0 = vial_mtime();
	while (done < size) {
		write = size - done;
		e = vial_socket_send(&_self->socket, p, &write);
		if (e)
			vial_error_throw(vial_stream_io_error_new(NULL, NULL));
		if (write > 0) {
			p += write;
			done += write;
			t0 = vial_mtime();
			continue;
		}
		if (vial_mtime() > t0 + _self->write_timeout)
			vial_error_throw(vial_stream_io_error_new("Write timeout", NULL));
		vial_msleep(1);
	}
	return NULL;
}

VIAL_BEGIN_CLASS_DEF(vial_netstream, vial_object) VIAL_END_CLASS_DEF;

VIAL_BEGIN_IMPLEMENTATION(vial_netstream, vial_object)
	impl_dispose,
VIAL_END_IMPLEMENTATION(vial_netstream, vial_object);

VIAL_BEGIN_IMPLEMENTATION(vial_netstream, vial_stream)
	impl_capabilities,
	impl_close,
	impl_flush,
	vial_throws_notsup_error,
	vial_throws_notsup_error,
	impl_available,
	impl_read,
	impl_write,
VIAL_END_IMPLEMENTATION(vial_netstream, vial_stream);

vial_error vial_netstream_init(struct vial_netstream *self, struct vial_socket socket)
{
	self->as_vial_object.typeinfo = &vial_netstream_typeinfo;
	self->as_vial_object.vtable = &vial_netstream_as_vial_object;
	self->as_vial_stream.vtable = &vial_netstream_as_vial_stream;
	self->read_timeout = self->write_timeout = 60000;
	self->socket = socket;
	return NULL;
}
