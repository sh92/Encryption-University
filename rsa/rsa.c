#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bio.h>

#define KEY_LENGTH 1024
#define PUB_EXP 3

#define FILE_SIZE 256

int GetFileSize(FILE *fp)
{
    int offset_bkup;
    int fsize;

    offset_bkup = ftell(fp);
    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);

    fseek(fp, offset_bkup, SEEK_SET);
    printf("File Size = %d\n", fsize);

    return fsize;
}

int main(){
    int pri_len;
    int pub_len;
    char* pri_key;
    char* pub_key;

    char FileName[FILE_SIZE];

    RSA* keypair = RSA_new();
    keypair = RSA_generate_key(KEY_LENGTH, PUB_EXP, NULL, NULL);

    BIO* pri = BIO_new(BIO_s_mem());
    BIO* pub = BIO_new(BIO_s_mem());

    PEM_write_bio_RSAPrivateKey(pri, keypair, NULL, NULL, 0, NULL, NULL);
    PEM_write_bio_RSAPublicKey(pub, keypair);

    pri_len = BIO_pending(pri);
    pub_len = BIO_pending(pub);

    pri_key = (char*)malloc(pri_len+1);
    pub_key = (char*)malloc(pub_len+1);

    BIO_read(pri, pri_key, pri_len);
    BIO_read(pub, pub_key, pub_len);

    pri_key[pri_len] = '\0';
    pub_key[pub_len] = '\0';

    FILE *key_file;
    FILE *plain;
    FILE *encrypted;
    FILE *decrypted;
    
    int file_size = 0;
    printf("input FileName : ");
    scanf("%s", FileName);
    plain = fopen(FileName, "rb");

    file_size = GetFileSize(plain);

    key_file = fopen("prkey.pem", "w");
    fprintf(key_file, "%s", pri_key);
    fclose(key_file);

    key_file = fopen("pukey.pem", "w");
    fprintf(key_file, "%s", pri_key);
    fclose(key_file);

    key_file = fopen("rsa.key", "w");
    RSA_print_fp(key_file, keypair, 0);
    fclose(key_file);

    printf("%s\n", pri_key);
    printf("%s\n", pub_key);
    printf("file size: %d\n", file_size);

    char* plain_buffer = (char*)malloc(sizeof(char)*file_size);
    char* cipher_buffer = (char*)malloc(RSA_size(keypair));

    memset(plain_buffer, 0, sizeof(char)*file_size);
    memset(cipher_buffer, 0, RSA_size(keypair));
    fread(plain_buffer, sizeof(char), file_size, plain);
    fclose(plain);


    int encrypted_length = RSA_public_encrypt(file_size-1, plain_buffer, cipher_buffer, keypair, RSA_PKCS1_OAEP_PADDING);
    char* decrypted_buffer = (char*)malloc(encrypted_length);
    memset(decrypted_buffer,0,sizeof(char)*encrypted_length);
    encrypted = fopen("encrypt.txt", "wb");
    decrypted = fopen("decrypt.txt", "wb");
    fprintf(encrypted, "%s", cipher_buffer);
    fclose(encrypted);
    

    int decrypted_length = RSA_private_decrypt(encrypted_length, cipher_buffer, decrypted_buffer, keypair, RSA_PKCS1_OAEP_PADDING);
    fwrite(decrypted_buffer, sizeof(char), decrypted_length, decrypted);
    fclose(decrypted);

    if(plain_buffer) free(plain_buffer);
    if(cipher_buffer) free(cipher_buffer);
    if(decrypted_buffer) free(decrypted_buffer);

    if(keypair) RSA_free(keypair);
    if(pub_key) BIO_free_all(pub);
    if(pri_key) BIO_free(pri);
    return 0;
}

