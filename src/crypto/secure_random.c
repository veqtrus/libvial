/*
Copyright (c) 2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <vial/crypto/secure_random.h>

#include <string.h>

#if defined(_WIN32) || defined(_WIN64)
#include <bcrypt.h>
#else
#include <fcntl.h>
#include <unistd.h>
#endif

bool vial_secure_random_get(void *buf, size_t size)
{
#if defined(_WIN32) || defined(_WIN64)
	return BCryptGenRandom(NULL, buf, size, BCRYPT_USE_SYSTEM_PREFERRED_RNG) == STATUS_SUCCESS;
#else
	int fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0)
		return false;
	ssize_t sz = read(fd, buf, size);
	return (close(fd) == 0) && (sz == (ssize_t) size);
#endif
}

#define _self ((struct vial_randstream *) self)

static vial_error impl_capabilities(void *self, int *capabilities)
{
	*capabilities = VIAL_STREAM_CAN_ALL ^ VIAL_STREAM_CAN_WRITE;
	return NULL;
}

static vial_error impl_seek(void *self, long offset, enum vial_stream_seek origin)
{
	bool back = offset < 0;
	if (back) offset = -offset;
	uint64_t position = ((uint64_t) _self->state.state[12]) * sizeof(_self->bytes) + _self->used_bytes;
	switch (origin) {
	case VIAL_STREAM_SEEK_SET:
		if (back)
			vial_error_throw(vial_arg_error_new(NULL, NULL));
		position = offset;
		break;
	case VIAL_STREAM_SEEK_CUR:
		if (back)
			position -= offset;
		else
			position += offset;
		break;
	case VIAL_STREAM_SEEK_END:
		if (!back && offset != 0)
			vial_error_throw(vial_arg_error_new(NULL, NULL));
		position = (UINT64_C(1) << 32) * sizeof(_self->bytes) - offset;
		break;
	}
	_self->state.state[12] = position / sizeof(_self->bytes);
	_self->used_bytes = position % sizeof(_self->bytes);
	vial_chacha20_next(&_self->state, _self->bytes);
	return NULL;
}

static vial_error impl_position(void *self, size_t *position)
{
	*position = ((size_t) _self->state.state[12]) * sizeof(_self->bytes) + _self->used_bytes;
	return NULL;
}

static vial_error impl_available(void *self, size_t *available)
{
	const size_t position = ((size_t) _self->state.state[12]) * sizeof(_self->bytes) + _self->used_bytes;
	*available = (UINT64_C(1) << 32) * sizeof(_self->bytes) - position;
	return NULL;
}

static vial_error impl_read(void *self, void *buf, size_t size)
{
	if (size == 0) return NULL;
	uint8_t *pbuf = buf;
	const unsigned rem_bytes = sizeof(_self->bytes) - _self->used_bytes;
	if (rem_bytes > size) {
		memcpy(pbuf, _self->bytes + _self->used_bytes, size);
		_self->used_bytes += size;
	} else {
		memcpy(pbuf, _self->bytes + _self->used_bytes, rem_bytes);
		vial_chacha20_next(&_self->state, _self->bytes);
		size -= rem_bytes;
		pbuf += rem_bytes;
		_self->used_bytes = 0;
		while (size >= sizeof(_self->bytes)) {
			memcpy(pbuf, _self->bytes, sizeof(_self->bytes));
			vial_chacha20_next(&_self->state, _self->bytes);
			size -= sizeof(_self->bytes);
			pbuf += sizeof(_self->bytes);
		}
		memcpy(pbuf, _self->bytes, size);
		_self->used_bytes = size;
	}
	return NULL;
}

VIAL_BEGIN_CLASS_DEF(vial_randstream, vial_object) VIAL_END_CLASS_DEF;

VIAL_BEGIN_IMPLEMENTATION(vial_randstream, vial_stream)
	impl_capabilities,
	vial_throws_notsup_error,
	vial_throws_notsup_error,
	impl_seek,
	impl_position,
	impl_available,
	impl_read,
	vial_throws_notsup_error,
VIAL_END_IMPLEMENTATION(vial_randstream, vial_stream);

vial_error vial_randstream_init(struct vial_randstream *self, const void *key, const void *nonce)
{
	vial_object_init(&self->as_vial_object);
	self->as_vial_object.typeinfo = &vial_randstream_typeinfo;
	self->as_vial_stream.vtable = &vial_randstream_as_vial_stream;
	self->used_bytes = 0;
	vial_chacha20_init(&self->state, key, nonce);
	vial_chacha20_next(&self->state, self->bytes);
	return NULL;
}

vial_error vial_randstream_init_random(struct vial_randstream *self)
{
	static const unsigned char nonce[12] = { 0 };
	unsigned char key[32];
	vial_object_init(&self->as_vial_object);
	self->as_vial_object.typeinfo = &vial_randstream_typeinfo;
	self->as_vial_stream.vtable = &vial_randstream_as_vial_stream;
	self->used_bytes = 0;
	if (!vial_secure_random_get(key, sizeof(key)))
		vial_error_throw(vial_error_new("Cannot get random bytes", NULL));
	vial_chacha20_init(&self->state, key, nonce);
	vial_chacha20_next(&self->state, self->bytes);
	return NULL;
}
