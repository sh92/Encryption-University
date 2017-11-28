#include "chat.h"

char* dkey = NULL;
void * recv_msg(void * socket) {
     int result;
     char buffer[BUFFER_SIZE];
     int sock = (int) socket;
     memset(buffer, 0, BUFFER_SIZE);
     while(1){
          result = recvfrom(sock, buffer, BUFFER_SIZE, 0, NULL, NULL);
          if (result < 0) {
               printf("Error receive !\n");
               return (void*) -1;
          } 
          des(buffer, dkey, strlen(buffer), 1);
          fprintf(stdout, "[A] %s", buffer);
     }
}

int main(int argc, char** argv) {

     if (argc < 2) {
          printf("usage: b  <port> \n");
          exit(1);
     }
     signal(SIGINT, (void *)handler);
     struct sockaddr_in addr;
     int sock, result;
     char buffer[BUFFER_SIZE];
     char * server_ip = "127.0.0.1";
     pthread_t recv_thread;
     void * thread_result;
     memset(&addr, 0, sizeof(addr));
     addr.sin_family = AF_INET;
     addr.sin_addr.s_addr = inet_addr(server_ip);
     addr.sin_port = htons(atoi(argv[1]));

     sock = socket(AF_INET, SOCK_STREAM, 0);
     if (sock < 0) {
          printf("Error creating socket!\n");
          exit(1);
     }

     printf("Input private key automately : prkey_b.pem \n");
     FILE* f = fopen("prkey_b.pem","rb");
     RSA* keypair = read_private_key(f);
     fclose(f);

     result = connect(sock, (struct sockaddr *) &addr, sizeof(addr));
     if (result < 0) {
         printf("Error connecting to the A!\n");
         exit(1);
     }
     printf("Connected to A!\n");

     result = recvfrom(sock, buffer, BUFFER_SIZE, 0, NULL, NULL);
     dkey = (char*)malloc(result);

     RSA_private_decrypt(result, buffer, dkey, keypair,  RSA_PKCS1_OAEP_PADDING);
     printf("Get encrypted key by b public key from A\n");
     printf("Dencrypted key by b private key\n");
     printf("Key:%s\n",dkey);

     memset(buffer, 0, BUFFER_SIZE);

     printf("Let's Chat!!!\n");

     result = pthread_create(&recv_thread, NULL, recv_msg, (void *) sock);

     while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
          fprintf(stdout, "[B] %s", buffer);
          des(buffer, dkey, strlen(buffer), 0);
          result = sendto(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &addr, sizeof(addr));
          if (result < 0) {
             printf("Error send!");
          }
     }
     if(dkey) free(dkey);
     close(sock);
     pthread_join(recv_thread, &thread_result);
     return 0;
}
