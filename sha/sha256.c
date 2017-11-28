#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
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
    SHA256_CTX ctx;
    SHA256_Init(&ctx);

    char FileName[FILE_SIZE];
    FILE *plain;
    FILE *hashed;
    FILE *original;
    int mode = 0;
    int file_size = 0;
    int i=0;

    printf("[1] Generate Hash [2] Hash Check [3] Quit ? ");
    scanf("%d", &mode);

    if(mode==3){
        return 0;
    }
    
    
    printf("input FileName : ");
    scanf("%s", FileName);
    plain = fopen(FileName, "rb");

    file_size = GetFileSize(plain);

    char* plain_buffer = (char*)malloc(sizeof(char)*file_size);
    unsigned char hashed_buffer[SHA256_DIGEST_LENGTH];
    unsigned char ori_hashed_buffer[SHA256_DIGEST_LENGTH];

    memset(plain_buffer, 0, sizeof(char)*file_size);
    memset(hashed_buffer, 0, sizeof(char)* SHA256_DIGEST_LENGTH);
    memset(ori_hashed_buffer, 0, sizeof(char)* SHA256_DIGEST_LENGTH);

    fread(plain_buffer, sizeof(char), file_size, plain);
    fclose(plain);
    SHA256_Update(&ctx, plain_buffer, file_size);
    SHA256_Final(hashed_buffer, &ctx);

    int original_file_size= 0;
    original = fopen("hash.txt", "rb");
    original_file_size = GetFileSize(original);
    char* ori_buffer = (char*)malloc(sizeof(char)*original_file_size);
    fread(ori_buffer, sizeof(char), original_file_size, original);
    fclose(original);

    if(mode==2){

        SHA256_Init(&ctx);
        SHA256_Update(&ctx, ori_buffer, original_file_size);
        SHA256_Final(ori_hashed_buffer, &ctx);


        int isSame = 1;
        printf("Original Hash : ");
        for(i=0; i < SHA256_DIGEST_LENGTH;i++){
            printf("%02x", ori_hashed_buffer[i]);
        }
        printf("\n");

        printf("Generated Hash : ");
        for(i=0; i < SHA256_DIGEST_LENGTH;i++){
            printf("%02x", hashed_buffer[i]);
            if(hashed_buffer[i] != ori_hashed_buffer[i])
                isSame = 0;
        }
        printf("\n");

        if(isSame) {
            printf("EQUAL");
        }else{
            printf("DIFFERENT");
        }

        fclose(original);
        return 0;
    }

    if(mode==1){
        for(i=0; i < SHA256_DIGEST_LENGTH;i++){
            printf("%02x", hashed_buffer[i]);
        }
        fclose(hashed);
    }

    if(plain_buffer) free(plain_buffer);
    if(ori_buffer) free(ori_buffer);
    return 0;
}

