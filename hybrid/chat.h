#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

#include <pthread.h>
#include <errno.h>
#include <signal.h>
#include <arpa/inet.h>
#include <openssl/des.h>

#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bio.h>


#define NAME_SIZE 100
#define BLOCK_SIZE 128
#define BUFFER_SIZE 1000
#define KEY_SIZE 256


void handler(void)
{
    fprintf(stdout, "\n\033[F\033[J");
    fprintf(stdout, "-------------------------------------\n");
    fprintf(stdout, ">> EXIT \n");
    fprintf(stdout, "-------------------------------------\n");
    exit(1);
}


int des(char* msg, char* key, unsigned int msg_len, int mode) {
    DES_key_schedule des_ks;
    DES_cblock des_key = {0, };
    DES_cblock iv = {0, };
    unsigned int i, result, padding;

    unsigned char block_in[BLOCK_SIZE] = {0, };
    unsigned char block_out[BLOCK_SIZE] = { 0, };

    DES_string_to_key(key, &des_key);
    DES_set_key_checked(&des_key, &des_ks);
    memcpy(block_in, msg, msg_len);
    if (mode == 0) {
       if(msg_len < BLOCK_SIZE) {
           padding = BLOCK_SIZE - msg_len;
           block_in[BLOCK_SIZE-1] = padding;
       }
       DES_ncbc_encrypt(block_in, block_out, BLOCK_SIZE, &des_ks, &iv, DES_ENCRYPT);
       result = BLOCK_SIZE;
    }
    else if (mode == 1) {
       DES_ncbc_encrypt(block_in, block_out, BLOCK_SIZE, &des_ks, &iv, DES_DECRYPT);
       padding = block_out[BLOCK_SIZE -1];
       result = BLOCK_SIZE - padding;
    }
    memcpy(msg, block_out, BLOCK_SIZE);
    return result;
}

int GetFileSize(FILE *fp)
{
    int offset_bkup;
    int filesize;
    offset_bkup = ftell(fp);
    fseek(fp, 0, SEEK_END);
    filesize = ftell(fp);
    fseek(fp, offset_bkup, SEEK_SET);
    return filesize;
}

RSA* read_public_key(FILE* fp) {
  FILE* f = fp;
  long filesize = GetFileSize(f);
  char *rsa_key = malloc(filesize + 1);
  fread(rsa_key, filesize, 1, f);
  rsa_key[filesize] = 0;
  BIO *rsa_bio = BIO_new(BIO_s_mem());
  BIO_write(rsa_bio, rsa_key, filesize);

  RSA *rsa = RSA_new();
  PEM_read_bio_RSA_PUBKEY(rsa_bio, &rsa, NULL, NULL);
  free(rsa_key);
  fclose(f);
  return rsa;
}

RSA* read_private_key(FILE* fp) {
  FILE *f = fp;
  long filesize = GetFileSize(f);
  char* rsa_key = malloc(filesize + 1);
  fread(rsa_key, filesize, 1, f);
  rsa_key[filesize] = 0;
  BIO *rsa_bio = BIO_new(BIO_s_mem());
  BIO_write(rsa_bio, rsa_key, filesize);

  RSA *rsa = RSA_new();
  PEM_read_bio_RSAPrivateKey(rsa_bio, &rsa, NULL, NULL);
  free(rsa_key);
  fclose(f);
  return rsa;
}
