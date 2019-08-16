#include "stir_shaken.h"


stir_shaken_status_t stir_shaken_unit_test_sign_verify_data(void)
{
    stir_shaken_status_t status = STIR_SHAKEN_STATUS_FALSE;
	const char *path = "/usr/local/freeswitch/conf/stir_shaken/test";
    const char *data_test_pass = "unit test 2 pass";
    const char *data_test_fail = "unit test 2 fail";
    size_t datalen = 0;
    unsigned char sig[PBUF_LEN] = { 0 };
    size_t outlen = 0;

    char private_key_name[300] = { 0 };
    char public_key_name[300] = { 0 };
    
    EC_KEY *ec_key = NULL;
    EVP_PKEY *private_key = NULL;
    EVP_PKEY *public_key = NULL;
    int i = -1;
    
    sprintf("%s%s%s", private_key_name, path, '/', "u2_private_key.pem");
    sprintf("%s%s%s", public_key_name, path, '/', "u2_public_key.pem");

    printf("=== Unit testing: STIR/Shaken Sign-verify (in memory) [stir_shaken_unit_test_sign_verify_data]\n\n");
    
    // Generate new keys for this test
    status = stir_shaken_generate_keys(&ec_key, &private_key, &public_key, private_key_name, public_key_name);
    stir_shaken_assert(status == STIR_SHAKEN_STATUS_OK, "Err, failed to generate keys...\n\n");
    stir_shaken_assert(ec_key != NULL, "Err, failed to generate EC key\n\n");
    stir_shaken_assert(private_key != NULL, "Err, failed to generate private key\n\n");
    stir_shaken_assert(public_key != NULL, "Err, failed to generate public key\n\n");
    
    /* Test */
    printf("Signing...\n\n");

    datalen = strlen(data_test_pass);
    outlen = sizeof(sig);
    status = stir_shaken_do_sign_data_with_digest("sha256", private_key, data_test_pass, datalen, sig, &outlen);
    stir_shaken_assert(status == STIR_SHAKEN_STATUS_OK, "Failed to sign");
    
    printf("Verifying (against good data)...\n\n");
    i = stir_shaken_do_verify_data(data_test_pass, strlen(data_test_pass), sig, outlen, public_key);
    stir_shaken_assert(i == 0, "Err, verify failed\n\n");
    
    printf("Verifying (against bad data)...\n\n");
    i = stir_shaken_do_verify_data(data_test_fail, strlen(data_test_fail), sig, outlen, public_key);
    stir_shaken_assert(i == 1, "Err, verify failed\n\n");
    
    return STIR_SHAKEN_STATUS_OK;
}

int main(void)
{
	if (stir_shaken_unit_test_sign_verify_data() != STIR_SHAKEN_STATUS_OK) {
		return -1;
	}

	printf("Test 1: OK\n");

	return 0;
}
