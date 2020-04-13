/*
Copyright (c) 2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <vial/base_encoding.h>

static const char base64_alphabet[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

long vial_base_encode(char *dst, const uint8_t *src, size_t len, int base)
{
	long count;
	uint_fast32_t val;
	switch (base) {
	case 16:
		count = len * 2;
		while (len > 0) {
			val = *src >> 4;
			dst[0] = (val < 10 ? '0' : 'A' - 10) + val;
			val = *src & 15;
			dst[1] = (val < 10 ? '0' : 'A' - 10) + val;
			len--;
			src++;
			dst += 2;
		}
		*dst = '\0';
		return count;
	case -16:
		count = len * 2;
		while (len > 0) {
			val = *src >> 4;
			dst[0] = (val < 10 ? '0' : 'a' - 10) + val;
			val = *src & 15;
			dst[1] = (val < 10 ? '0' : 'a' - 10) + val;
			len--;
			src++;
			dst += 2;
		}
		*dst = '\0';
		return count;
	case 64:
	case -64:
		count = (len / 3) * 4;
		while (len >= 3) {
			val = src[0];
			val <<= 8;
			val |= src[1];
			val <<= 8;
			val |= src[2];
			dst[3] = base64_alphabet[val & 63];
			val >>= 6;
			dst[2] = base64_alphabet[val & 63];
			val >>= 6;
			dst[1] = base64_alphabet[val & 63];
			val >>= 6;
			dst[0] = base64_alphabet[val];
			len -= 3;
			src += 3;
			dst += 4;
		}
		switch (len) {
		case 2:
			val = src[0];
			val <<= 8;
			val |= src[1];
			val <<= 2;
			dst[2] = base64_alphabet[val & 63];
			val >>= 6;
			dst[1] = base64_alphabet[val & 63];
			val >>= 6;
			dst[0] = base64_alphabet[val];
			if (base == 64) {
				dst[3] = '=';
				count += 4;
				dst += 4;
			} else {
				count += 3;
				dst += 3;
			}
			break;
		case 1:
			val = src[0];
			val <<= 4;
			dst[1] = base64_alphabet[val & 63];
			val >>= 6;
			dst[0] = base64_alphabet[val];
			if (base == 64) {
				dst[3] = dst[2] = '=';
				count += 4;
				dst += 4;
			} else {
				count += 2;
				dst += 2;
			}
			break;
		}
		*dst = '\0';
		return count;
	default:
		return -1;
	}
}

long vial_base_decode(uint8_t *dst, const char *src, int base)
{
	size_t len = 0;
	uint_fast32_t val, digit;
	switch (base) {
	case 16:
	case -16:
		while ((digit = *src)) {
			if (digit >= '0' && digit <= '9')
				digit -= '0';
			else if (digit >= 'A' && digit <= 'F')
				digit -= 'A' - 10;
			else if (digit >= 'a' && digit <= 'f')
				digit -= 'a' - 10;
			else
				return -1;
			len++;
			src++;
			if (len & 1) {
				val = digit << 4;
			} else {
				*dst = val | digit;
				val = 0;
				dst++;
			}
		}
		/* single digit followed by end of string */
		if (len & 1)
			return -1;
		return len / 2;
	case 64:
	case -64:
		val = 0;
		while ((digit = *src)) {
			if (digit >= 'A' && digit <= 'Z')
				val |= digit - 'A';
			else if (digit >= 'a' && digit <= 'z')
				val |= digit - 'a' + 26;
			else if (digit >= '0' && digit <= '9')
				val |= digit - '0' + 52;
			else if (digit == '+')
				val |= 62;
			else if (digit == '/')
				val |= 63;
			else if (digit == '=')
				break;
			else
				return -1;
			src++;
			if (++len & 3) {
				val <<= 6;
			} else {
				dst[2] = val & 0xFF;
				val >>= 8;
				dst[1] = val & 0xFF;
				val >>= 8;
				dst[0] = val;
				val = 0;
				dst += 3;
			}
		}
		switch (len & 3) {
		case 3:
			val >>= 8;
			dst[1] = val & 0xFF;
			val >>= 8;
			dst[0] = val;
			return (len / 4) * 3 + 2;
		case 2:
			val >>= 10;
			dst[0] = val;
			return (len / 4) * 3 + 1;
		case 1:
			return -1;
		case 0:
			return (len / 4) * 3;
		}
	default:
		return -1;
	}
}
