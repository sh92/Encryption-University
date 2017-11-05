#include <stdio.h>
#include <stdlib.h>


void Encrypt(char* buff, int key){
	if(*buff=='\n'){
		return;
	}
	char c  = *buff;
	int x = (int)(c - 'a');
	char e;
	if( x < 0 ){
		e = (char)(((c-'A')+key)%26+'A');
	}else{
		e = (char)(((c-'a')+key)%26+'a');
	}
	*buff = e;
}

void Decrypt(char* buff, int key){
	if(*buff=='\n'){
		return;
	}
	char c  = *buff;
	int x = (int)(c - 'a');
	char e;

	if( x < 0 ){
		e = (char)((c-'A'-key+26)%26+'A');
	}else{
		e = (char)((c-'a'-key+26)%26+'a');
	}

	*buff = e;
}

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

int main(void)
{
	char inputFileName[256];
	char outputFileName[256];
	FILE * input_FD;
	FILE * output_FD;
	int mode;
	int key;

	printf(">> Input File Name : ");
	scanf("%s", inputFileName);

	printf(">> Input Mode [ 0: Encrypt, 1: Decrypt ] : ");
	scanf("%d", &mode);

	printf(">> Input key : ");
	scanf("%d", &key);

	if(mode == 0) {
		sprintf(outputFileName, "encrypt.txt");
	}
	else if ( mode == 1 ) {
		sprintf(outputFileName, "decrypt.txt");
	}
	else{
		printf("[!] Mode Error !\n");
		exit(1);
	}

	input_FD = fopen(inputFileName, "rb");
	output_FD = fopen(outputFileName, "wb");
	int fileSize = GetFileSize(input_FD);
	char buff;
	int i;

	for (  i = 0 ; i < fileSize ; i++ ) {
		fread( &buff, sizeof(char), 1, input_FD );

		if ( mode == 0 ) {
			Encrypt( &buff, key );
		}else if ( mode == 1 ) {
			Decrypt( &buff, key );
		}

		fwrite( &buff, sizeof(char), 1, output_FD);
	}
	fclose(output_FD);
	fclose(input_FD);

	return 0;
}
