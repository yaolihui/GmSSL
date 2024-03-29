/*
 *  Copyright 2014-2023 The GmSSL Project. All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the License); you may
 *  not use this file except in compliance with the License.
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gmssl/sm4.h>
#include <gmssl/mem.h>
#include <gmssl/rand.h>
#include <gmssl/error.h>


int main(void)
{
	unsigned char key[16] = {
		0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,
		0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,
	};
	unsigned char iv[16] = {
		0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,
		0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,
	};

	SM4_CBC_CTX cbc_ctx;
	unsigned char inbuf[1024];
	unsigned char outbuf[1024 + 32];
	size_t inlen;
	size_t outlen;

	if (sm4_cbc_decrypt_init(&cbc_ctx, key, iv) != 1) {
		error_print();
		goto err;
	}

	fprintf(stderr, "read from stdin ...\n");
	while ((inlen = fread(inbuf, 1, sizeof(inbuf), stdin)) > 0) {
		if (sm4_cbc_decrypt_update(&cbc_ctx, inbuf, inlen, outbuf, &outlen) != 1) {
			error_print();
			goto err;
		}
		fwrite(outbuf, 1, outlen, stdout);
	}

	if (sm4_cbc_decrypt_finish(&cbc_ctx, outbuf, &outlen) != 1) {
		error_print();
		goto err;
	}
	fwrite(outbuf, 1, outlen, stdout);

err:
	gmssl_secure_clear(&cbc_ctx, sizeof(cbc_ctx));
	return 0;
}
