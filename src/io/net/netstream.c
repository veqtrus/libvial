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

#define _self ((struct vNetStream *) self)

static void impl_dispose(struct vStream *self)
{
	memset(self, 0, sizeof(struct vNetStream));
}

static error_t impl_capabilities(struct vStream *self, int *capabilities)
{
	*capabilities = VSTREAM_SUPPORTS_READ | VSTREAM_SUPPORTS_WRITE | VSTREAM_SUPPORTS_AVAILABLE;
	return NULL;
}

static error_t impl_close(struct vStream *self)
{
	error_t e = vSocket_close(&_self->socket);
	if (e)
		return error_new(VSTREAM_IO_ERROR, VSTREAM_IO_ERROR, e);
	memset(&_self->socket, 0, sizeof(_self->socket));
	return NULL;
}

static error_t impl_flush(struct vStream *self) { return NULL; }

static error_t impl_not_supported()
{
	return error_new(VSTREAM_NOT_SUPPORTED, VSTREAM_NOT_SUPPORTED, NULL);
}

static error_t impl_available(struct vStream *self, size_t *available)
{
	error_t e = vSocket_available(&_self->socket, available);
	if (e)
		return error_new(VSTREAM_IO_ERROR, VSTREAM_IO_ERROR, e);
	return NULL;
}

static error_t impl_read(struct vStream *self, void *buf, size_t size)
{
	error_t e;
	size_t done = 0, read = 0;
	char *p = buf;
	uint64_t t0 = mtime();
	while (done < size) {
		read = size - done;
		e = vSocket_recv(&_self->socket, p, &read);
		if (e)
			return error_new(VSTREAM_IO_ERROR, VSTREAM_IO_ERROR, e);
		if (read > 0) {
			p += read;
			done += read;
			t0 = mtime();
			continue;
		}
		if (mtime() > t0 + _self->read_timeout)
			return error_new(VSTREAM_IO_ERROR, "Read timeout", NULL);
		msleep(1);
	}
	return NULL;
}

static error_t impl_write(struct vStream *self, const void *buf, size_t size)
{
	error_t e;
	size_t done = 0, write = 0;
	const char *p = buf;
	uint64_t t0 = mtime();
	while (done < size) {
		write = size - done;
		e = vSocket_send(&_self->socket, p, &write);
		if (e)
			return error_new(VSTREAM_IO_ERROR, VSTREAM_IO_ERROR, e);
		if (write > 0) {
			p += write;
			done += write;
			t0 = mtime();
			continue;
		}
		if (mtime() > t0 + _self->write_timeout)
			return error_new(VSTREAM_IO_ERROR, "Write timeout", NULL);
		msleep(1);
	}
	return NULL;
}

static const struct vStream_vtable vtable = {
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

error_t vNetStream_init(struct vNetStream *self, struct vSocket socket)
{
	self->stream.vtable = &vtable;
	self->read_timeout = self->write_timeout = 60000;
	self->socket = socket;
	return NULL;
}
