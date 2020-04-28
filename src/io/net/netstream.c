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

static void impl_dispose(struct vial_stream *self)
{
	memset(self, 0, sizeof(struct vial_netstream));
}

static vial_error impl_capabilities(struct vial_stream *self, int *capabilities)
{
	*capabilities = VIAL_STREAM_CAN_READ | VIAL_STREAM_CAN_WRITE | VIAL_STREAM_CAN_AVAILABLE;
	return NULL;
}

static vial_error impl_close(struct vial_stream *self)
{
	vial_error e = vial_socket_close(&_self->socket);
	if (e)
		return vial_error_new(VIAL_STREAM_IO_ERROR, VIAL_STREAM_IO_ERROR, e);
	memset(&_self->socket, 0, sizeof(_self->socket));
	return NULL;
}

static vial_error impl_flush(struct vial_stream *self) { return NULL; }

static vial_error impl_not_supported()
{
	return vial_error_new(VIAL_STREAM_NOT_SUPPORTED, VIAL_STREAM_NOT_SUPPORTED, NULL);
}

static vial_error impl_available(struct vial_stream *self, size_t *available)
{
	vial_error e = vial_socket_available(&_self->socket, available);
	if (e)
		return vial_error_new(VIAL_STREAM_IO_ERROR, VIAL_STREAM_IO_ERROR, e);
	return NULL;
}

static vial_error impl_read(struct vial_stream *self, void *buf, size_t size)
{
	vial_error e;
	size_t done = 0, read = 0;
	char *p = buf;
	uint64_t t0 = vial_mtime();
	while (done < size) {
		read = size - done;
		e = vial_socket_recv(&_self->socket, p, &read);
		if (e)
			return vial_error_new(VIAL_STREAM_IO_ERROR, VIAL_STREAM_IO_ERROR, e);
		if (read > 0) {
			p += read;
			done += read;
			t0 = vial_mtime();
			continue;
		}
		if (vial_mtime() > t0 + _self->read_timeout)
			return vial_error_new(VIAL_STREAM_IO_ERROR, "Read timeout", NULL);
		vial_msleep(1);
	}
	return NULL;
}

static vial_error impl_write(struct vial_stream *self, const void *buf, size_t size)
{
	vial_error e;
	size_t done = 0, write = 0;
	const char *p = buf;
	uint64_t t0 = vial_mtime();
	while (done < size) {
		write = size - done;
		e = vial_socket_send(&_self->socket, p, &write);
		if (e)
			return vial_error_new(VIAL_STREAM_IO_ERROR, VIAL_STREAM_IO_ERROR, e);
		if (write > 0) {
			p += write;
			done += write;
			t0 = vial_mtime();
			continue;
		}
		if (vial_mtime() > t0 + _self->write_timeout)
			return vial_error_new(VIAL_STREAM_IO_ERROR, "Write timeout", NULL);
		vial_msleep(1);
	}
	return NULL;
}

static const struct vial_stream_vtable vtable = {
	impl_dispose,
	impl_capabilities,
	impl_close,
	impl_flush,
	impl_not_supported,
	impl_not_supported,
	impl_available,
	impl_read,
	impl_write
};

vial_error vial_netstream_init(struct vial_netstream *self, struct vial_socket socket)
{
	self->stream.vtable = &vtable;
	self->read_timeout = self->write_timeout = 60000;
	self->socket = socket;
	return NULL;
}
