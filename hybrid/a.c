#include "chat.h"
#define MAX_CLIENT_IP_LEN 100
#define MAX_CLIENT_NUM 2

char dkey[8] = {0,};

void * recv_msg(void * socket) {
    int sock, ret;
    char buffer[BUFFER_SIZE];
    sock = (int) socket;
    memset(buffer, 0, BUFFER_SIZE);
    while(1){
        ret = recvfrom(sock, buffer, BUFFER_SIZE, 0, NULL, NULL);
        if (ret <= 0) {
            printf("Error receive!\n");
            return (void*) -1;
        }
    des(buffer, dkey, strlen(buffer), 1);
    printf("[B] %s", buffer);
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf("usage: %s <port> \n", argv[0]);
        exit(1);
    }
    struct sockaddr_in server, from;
    int sock, len, ret, send_sock;
    char buffer[BUFFER_SIZE];
    char client_ip[MAX_CLIENT_IP_LEN];
    pthread_t recv_thread;
    void* thread_result; 
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr= INADDR_ANY;
    server.sin_port = htons(atoi(argv[1]));


    printf("Input public key automately : pukey_b.pem \n");
    FILE* f = fopen("pukey_b.pem","rb");
    RSA* keypair = read_public_key(f);
    fclose(f);

    printf("Input key(8) : ");
    while(1){
        scanf("%s", dkey);
        if(strlen(dkey)==8) {
            break;
        }
        printf("Key Length is 8!!\n");
        printf("Input key(8) : ");
        memset(&dkey, 0, sizeof(dkey));
     }


     printf("Encrypting des key by b's public key...\n");
     char* encrypted_buffer = (char*)malloc(RSA_size(keypair));
     int encrypted_length = RSA_public_encrypt(strlen(dkey), dkey, encrypted_buffer, keypair, RSA_PKCS1_OAEP_PADDING);
     sock = socket(AF_INET, SOCK_STREAM, 0);
     if (sock < 0) {
        printf("Error socket!\n");
        exit(1);
     }

     if(bind(sock, (struct sockaddr *) &server, sizeof(server)) < 0){
        printf("Error binding!\n");
        exit(1);
     }

     printf("Waiting for B connection \n");
     listen(sock, MAX_CLIENT_IP_LEN);

     len = sizeof(from);
     send_sock = accept(sock, (struct sockaddr *) &from, &len);
     if (send_sock < 0) {
        printf("Error accepting socket connection!\n");
        exit(1);
     }

     inet_ntop(AF_INET, &(from.sin_addr), client_ip, MAX_CLIENT_IP_LEN);
     sendto(send_sock, encrypted_buffer, encrypted_length, 0, (struct sockaddr *) &from, len);
     pthread_create(&recv_thread, NULL, recv_msg, (void *) send_sock);

     memset(buffer, 0, BUFFER_SIZE);
     while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
        printf("[A] %s", buffer);
        des(buffer, dkey, strlen(buffer), 0);
        ret = sendto(send_sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &from, len);
        if (ret < 0) {
            printf("Error send \n");
            exit(1);
        }
     }

      if(encrypted_buffer) free(encrypted_buffer);

     pthread_join(recv_thread, &thread_result);
     close(send_sock);
     close(sock);
     return 0;
}
