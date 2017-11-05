#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <pthread.h>
#include <errno.h>
#include <signal.h>
#include <arpa/inet.h>
#include <openssl/des.h>

#define NAME_SIZE	64
#define BLOCK_SIZE 16
#define MSG_SIZE BLOCK_SIZE
#define KEY_SIZE MSG_SIZE

pthread_mutex_t mutex;

typedef struct {
	char name[NAME_SIZE];
	unsigned char msg[MSG_SIZE];
} TALK;

void handler(void)
{
	fprintf(stdout, "\n\033[F\033[J");
	fprintf(stdout, "-------------------------------------\n");
	fprintf(stdout, ">> EXIT \n");
	fprintf(stdout, "-------------------------------------\n");
	exit(1);
}
