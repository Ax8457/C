#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>
#include <openssl/evp.h>
/*
	This script is exactly the same as provided in the link below, but with a different md5_vector function and different crypto libs (openssl versus CryptoAPI) usage so that it can be compiled on Unix-based OS using :
	gcc -o script script.c -lssl -lcrypto
	Source : https://wifininjas.net/2019/08/23/wn-blog-012-can-you-crack-802-1x-wpa2-enterprise-wireless-data/
*/
typedef unsigned int u32;
typedef unsigned char u8;

#define os_malloc(s) malloc((s))
#define os_free(p) free((p))
#define os_memcpy(d, s, n) memcpy((d), (s), (n))
#define MD5_MAC_LEN 16

int md5_vector(size_t num_elem, const u8 *addr[], const size_t *len, u8 *mac) {
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (!ctx) {
        fprintf(stderr, "EVP_MD_CTX_new failed\n");
        return -1;
    }
    if (EVP_DigestInit_ex(ctx, EVP_md5(), NULL) != 1) {
        fprintf(stderr, "EVP_DigestInit_ex failed\n");
        EVP_MD_CTX_free(ctx);
        return -1;
    }
    for (size_t i = 0; i < num_elem; i++) {
        if (EVP_DigestUpdate(ctx, addr[i], len[i]) != 1) {
            fprintf(stderr, "EVP_DigestUpdate failed\n");
            EVP_MD_CTX_free(ctx);
            return -1;
        }
    }
    unsigned int md_len;
    if (EVP_DigestFinal_ex(ctx, mac, &md_len) != 1) {
        fprintf(stderr, "EVP_DigestFinal_ex failed\n");
        EVP_MD_CTX_free(ctx);
        return -1;
    }
    EVP_MD_CTX_free(ctx);
    return 0;
}

static u8 *decrypt_ms_key(const u8 *key, size_t len,
                          const u8 *req_authenticator,
                          const u8 *secret, size_t secret_len, size_t *reslen) {
    u8 *plain, *ppos, *res;
    const u8 *pos;
    size_t left, plen;
    u8 hash[MD5_MAC_LEN];
    int i, first = 1;
    const u8 *addr[3];
    size_t elen[3];
    if (len < 2 + 16)
        return NULL;
    pos = key + 2;
    left = len - 2;
    if (left % 16) {
        printf("Invalid ms key len %lu\n", (unsigned long) left);
        return NULL;
    }
    plen = left;
    ppos = plain = (u8 *)os_malloc(plen);
    if (plain == NULL)
        return NULL;
    plain[0] = 0;
    while (left > 0) {
        addr[0] = secret;
        elen[0] = secret_len;
        if (first) {
            addr[1] = req_authenticator;
            elen[1] = MD5_MAC_LEN;
            addr[2] = key;
            elen[2] = 2;
        } else {
            addr[1] = pos - MD5_MAC_LEN;
            elen[1] = MD5_MAC_LEN;
        }
        md5_vector(first ? 3 : 2, addr, elen, hash);
        first = 0;

        for (i = 0; i < MD5_MAC_LEN; i++)
            *ppos++ = *pos++ ^ hash[i];
        left -= MD5_MAC_LEN;
    }
    if (plain[0] == 0 || plain[0] > plen - 1) {
        printf("Failed to decrypt MPPE key\n");
        os_free(plain);
        return NULL;
    }
    res = (u8 *)os_malloc(plain[0]);
    if (res == NULL) {
        os_free(plain);
        return NULL;
    }
    os_memcpy(res, plain + 1, plain[0]);
    if (reslen)
        *reslen = plain[0];
    os_free(plain);
    return res;
}

void processTokens(char *authenticator, u8 *processedAuthenticator,
                   char *recvKey, u8 *processedRecvKey) {
    char *ptr = strtok(authenticator, ":");
    int i = 0;
    while (ptr) {
        processedAuthenticator[i++] = (u8)strtoul(ptr, NULL, 16);
        ptr = strtok(NULL, ":");
    }

    ptr = strtok(recvKey, ":");
    i = 0;
    while (ptr) {
        processedRecvKey[i++] = (u8)strtoul(ptr, NULL, 16);
        ptr = strtok(NULL, ":");
    }
}

void dumpPmk(const u8 *pmk) {
    printf("PMK is:\n");
    for (int i = 0; i < 32; i++)
        printf("%02x", pmk[i]);
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s secret authenticator recv-key\n", argv[0]);
        return 1;
    }
    if (strlen(argv[2]) != 47) {
        printf("Bad authenticator length\n");
        return 1;
    }
    if (strlen(argv[3]) != 149) {
        printf("Bad recv-key length\n");
        return 1;
    }
    u8 processedAuthenticator[16];
    u8 processedRecvKey[50];
    u8 *pmk;
    size_t pmklen = 0;
    processTokens(argv[2], processedAuthenticator, argv[3], processedRecvKey);
    pmk = decrypt_ms_key(processedRecvKey, 50,
                         processedAuthenticator,
                         (u8 *)argv[1], strlen(argv[1]), &pmklen);
    if (pmk) {
        dumpPmk(pmk);
        os_free(pmk);
    } else {
        printf("Failed to decrypt key\n");
    }
    return 0;
}
