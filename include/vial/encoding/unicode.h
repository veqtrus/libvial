/*
Copyright (c) 2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#ifndef VIAL_ENCODING_UNICODE_H
#define VIAL_ENCODING_UNICODE_H

#include <stdint.h>

extern const uint8_t vial_utf8_transitions[256];

struct vial_unicode {
	unsigned state, error;
};

#define VIAL_UNICODE_INIT { 0, 0 }

static inline
void vial_unicode_init(struct vial_unicode *self)
{
	self->error = self->state = 0;
}

static inline
unsigned vial_utf8_decode(struct vial_unicode *self, uint32_t *code, unsigned in)
{
	const unsigned next_state = vial_utf8_transitions[(in & 0xF8) | (self->state & 7)];
	in &= next_state | 7;
	*code = in | ((*code << 6) & -((self->state >> 2) & 1));
	self->state = next_state & 7;
	self->error |= next_state & 0x80;
	return ((next_state >> 2) & 1) ^ 1;
}

static inline
unsigned vial_utf8_encode(uint32_t code, uint8_t *out)
{
	const unsigned offset =
			(code >= 0x80U) +
			(code >= 0x800U) +
			(code >= 0x10000U),
		mask = 0xFFU >> (offset + 2 * (code >= 0x80U));
	out[0] = (0xFFU & ((~mask) << 1)) | ((code >> (6 * offset)) & mask);
	code <<= 32 - 6 * offset;
	out[1] = 128 | ((code >> 26) & 63);
	out[2] = 128 | ((code >> 20) & 63);
	out[3] = 128 | ((code >> 14) & 63);
	return offset + 1;
}

static inline
unsigned vial_utf16_decode(struct vial_unicode *self, uint32_t *code, unsigned in)
{
	const uint32_t cur_state = self->state,
		sur1 = (in & 0xFC00U) == 0xD800U,
		sur2 = (in & 0xFC00U) == 0xDC00U;
	self->state = sur1 & ~cur_state;
	self->error |= (cur_state & ~sur2) | (~cur_state & sur2);
	in &= 0x3FFU | ((sur1 | sur2) - 1);
	*code = (in | ((*code << 10) & -cur_state)) + (cur_state << 16);
	return self->state ^ 1;
}

static inline
unsigned vial_utf16_encode(uint32_t code, uint16_t *out)
{
	const uint32_t large = code >= 0x10000U;
	code -= large << 16;
	const uint32_t sur1 = 0xD800U | ((code >> 10) & 0x3FFU);
	out[0] = (code & (large - 1)) | (sur1 & -large);
	out[1] = 0xDC00U | (code & 0x3FFU);
	return large + 1;
}

#endif
