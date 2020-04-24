/*
Copyright (c) 2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <vial/crypto/sha256.h>
#include <vial/crypto/aes.h>
#include <vial/encoding/base_encoding.h>
#include <vial/utime.h>

static int test_sha256()
{
	int errors = 0;
	char hash[32];
	struct test {
		const char *msg;
		uint8_t hash[32];
	} tests[] = {
		{
			"abc",
			{ 0xBA, 0x78, 0x16, 0xBF, 0x8F, 0x01, 0xCF, 0xEA, 0x41, 0x41, 0x40, 0xDE, 0x5D, 0xAE, 0x22, 0x23,
				0xB0, 0x03, 0x61, 0xA3, 0x96, 0x17, 0x7A, 0x9C, 0xB4, 0x10, 0xFF, 0x61, 0xF2, 0x00, 0x15, 0xAD }
		}, {
			"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
			{ 0x24, 0x8D, 0x6A, 0x61, 0xD2, 0x06, 0x38, 0xB8, 0xE5, 0xC0, 0x26, 0x93, 0x0C, 0x3E, 0x60, 0x39,
				0xA3, 0x3C, 0xE4, 0x59, 0x64, 0xFF, 0x21, 0x67, 0xF6, 0xEC, 0xED, 0xD4, 0x19, 0xDB, 0x06, 0xC1 }
		}, { NULL }
	};
	for (struct test *t = tests; t->msg != NULL; t++) {
		vial_sha256_hash(hash, t->msg, strlen(t->msg));
		if (memcmp(hash, t->hash, 32)) {
			printf("SHA256 failed for: %s\n", t->msg);
			errors++;
		}
	}
	return errors;
}

static int test_aes()
{
	int errors = 0;
	uint8_t buf0[16], buf1[16];
	char text_buf[40];
	struct test {
		const char *plain, *key, *cipher;
	} tests[] = {
		{
			"6BC1BEE22E409F96E93D7E117393172A",
			"2B7E151628AED2A6ABF7158809CF4F3C",
			"3AD77BB40D7A3660A89ECAF32466EF97"
		}, {
			"AE2D8A571E03AC9C9EB76FAC45AF8E51",
			"2B7E151628AED2A6ABF7158809CF4F3C",
			"F5D3D58503B9699DE785895A96FDBAAF"
		}, {
			"F69F2445DF4F9B17AD2B417BE66C3710",
			"2B7E151628AED2A6ABF7158809CF4F3C",
			"7B0C785E27E8AD3F8223207104725DD4"
		}, { NULL }
	};
	struct vial_aes aes;
	for (struct test *t = tests; t->plain != NULL; t++) {
		vial_base_decode(buf0, t->key, 16);
		vial_aes_init(&aes, VIAL_AES_MODE_ECB, 128, buf0, NULL);
		vial_base_decode(buf0, t->plain, 16);
		vial_base_decode(buf1, t->cipher, 16);
		vial_aes_encrypt(&aes, buf0, buf0, 16);
		if (memcmp(buf0, buf1, 16)) {
			vial_base_encode(text_buf, buf0, 16, 16);
			printf("AES failed encrypting: plain=%s,\n  expected=%s, encrypted=%s\n", t->plain, t->cipher, text_buf);
			errors++;
		}
		vial_base_decode(buf0, t->plain, 16);
		vial_aes_decrypt(&aes, buf1, buf1, 16);
		if (memcmp(buf0, buf1, 16)) {
			vial_base_encode(text_buf, buf1, 16, 16);
			printf("AES failed decrypting: cypher=%s,\n  expected=%s, decrypted=%s\n", t->cipher, t->plain, text_buf);
			errors++;
		}
		vial_base_decode(buf0, t->plain, 16);
		vial_base_decode(buf1, t->plain, 16);
		vial_aes_encrypt(&aes, buf0, buf0, 16);
		vial_aes_decrypt(&aes, buf0, buf0, 16);
		if (memcmp(buf0, buf1, 16)) {
			puts("decrypt(encrypt(buf)) != buf");
			errors++;
		}
	}
	return errors;
}

static void bench_sha256()
{
	uint8_t buf[] = { 0x24, 0x8D, 0x6A, 0x61, 0xD2, 0x06, 0x38, 0xB8, 0xE5, 0xC0, 0x26, 0x93, 0x0C, 0x3E, 0x60, 0x39,
		0xA3, 0x3C, 0xE4, 0x59, 0x64, 0xFF, 0x21, 0x67, 0xF6, 0xEC, 0xED, 0xD4, 0x19, 0xDB, 0x06, 0xC1 };
	uint64_t duration = vial_utime();
	for (int i = 0; i < 1000000; i++)
		vial_sha256_hash(buf, buf, sizeof(buf));
	duration = vial_utime() - duration;
	printf("SHA256 benchmark: %f MB/s\n", 32e6 / duration);
}

static void bench_aes()
{
	uint8_t buf[] = { 0x24, 0x8D, 0x6A, 0x61, 0xD2, 0x06, 0x38, 0xB8, 0xE5, 0xC0, 0x26, 0x93, 0x0C, 0x3E, 0x60, 0x39 },
		key[] = { 0xA3, 0x3C, 0xE4, 0x59, 0x64, 0xFF, 0x21, 0x67, 0xF6, 0xEC, 0xED, 0xD4, 0x19, 0xDB, 0x06, 0xC1 };
	struct vial_aes aes;
	vial_aes_init(&aes, VIAL_AES_MODE_CBC, 128, key, NULL);
	uint64_t duration = vial_utime();
	for (int i = 0; i < 1000000; i++)
		vial_aes_encrypt(&aes, buf, buf, sizeof(buf));
	duration = vial_utime() - duration;
	printf("AES128 benchmark: %f MB/s\n", 16e6 / duration);
}

int main()
{
	puts("Testing crypto...");
	printf("Total errors: %d\n", test_sha256() + test_aes());
	bench_sha256();
	bench_aes();
	return 0;
}

