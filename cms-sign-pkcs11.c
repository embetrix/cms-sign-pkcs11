/*
 * Copyright (c) 2023 Embetrix.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/cms.h>
#include <openssl/engine.h>
#include <openssl/conf.h>
#include <openssl/err.h>

static EVP_PKEY *read_private_key(const char *private_key_name)
{
	EVP_PKEY *private_key = NULL;
	ENGINE *eng = NULL;
	BIO *b = NULL;

	if (!strncmp(private_key_name, "pkcs11:", 7)) {
		ENGINE_load_builtin_engines();
		eng = ENGINE_by_id("pkcs11");
		if (!eng) {
			fprintf(stderr, "Error: Load PKCS#11 ENGINE\n");
			return NULL;
		}

		if (!ENGINE_init(eng)) {
			fprintf(stderr, "Error: ENGINE_init\n");
			return NULL;
		}

		private_key = ENGINE_load_private_key(eng, private_key_name,
						      NULL, NULL);

	} 
	else {
		b = BIO_new_file(private_key_name, "rb");
		if (!b)
			goto out;

		private_key = PEM_read_bio_PrivateKey(b, NULL, NULL, NULL);
		BIO_free(b);
	}
out:
	if (!private_key)
		fprintf(stderr, "Error: failed loading private key %s\n", private_key_name);

	if (eng)
		ENGINE_free(eng);

	return private_key;
}

int main(int argc, char **argv)
{
    int ret = -1;

    BIO *in = NULL, *out = NULL, *cbio = NULL;
    X509 *scert = NULL;
    CMS_ContentInfo *cms = NULL;
    EVP_PKEY *sprivkey   = NULL;

    int flags = CMS_BINARY | CMS_PARTIAL | CMS_DETACHED ;

    if (argc != 5) {
        printf("Usage: %s <sign cert> <sign key id> <input> <signature output>\n", argv[0]);
        return ret;
    }

    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();
    ERR_clear_error();

     /* Read in signing key */
    sprivkey = read_private_key(argv[2]);
    if (!sprivkey)
        goto out;

    /* Read in signing certificate */
    cbio  = BIO_new_file(argv[1], "r");
    if (!cbio)
        goto out;

    scert = PEM_read_bio_X509(cbio, NULL, 0, NULL);
    if (!scert)
        goto out;

    /* open content to be signed */
    in = BIO_new_file(argv[3], "r");

    if (!in)
        goto out;


    /* sign content */
    cms = CMS_sign(NULL, NULL, NULL, in, flags);
    if (!cms)
        goto out;

    CMS_SignerInfo *si;
    si = CMS_add1_signer(cms, scert, sprivkey, EVP_sha256(), flags);
    if (!si)
        goto out;

    if (!CMS_final(cms, in, NULL, flags))
        goto out;

    /* create the signature */
    out = BIO_new_file(argv[4], "w");
    if (!out)
        goto out;

    if (!i2d_CMS_bio_stream(out, cms, in, flags))
        goto out;

    ret = 0;

out:
    if (ret) {
        fprintf(stderr, "Error Signing Data\n");
        ERR_print_errors_fp(stderr);
    }

    if (cms)
        CMS_ContentInfo_free(cms);

    if (scert)
        X509_free(scert);

    if (sprivkey)
        EVP_PKEY_free(sprivkey);

    if (in)
        BIO_free(in);

    if (out)
        BIO_free(out);

    if (cbio)
        BIO_free(cbio);

    return ret;
}
