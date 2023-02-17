#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define BUFF_SIZE 1024

char* get_extension(char* filename) {
  char* dot = strrchr(filename, '.');
  if (!dot || dot == filename)
    return "";
  return dot + 1;
}

// only allow .jpg, .bmp, .png
/*
void sendFile(int sockfd, char* fileName) {
  FILE* fp;
  char buffer[BUFF_SIZE];
  int n;

  char* extension = get_extension(fileName);
  if (strcmp(extension, "jpg") != 0 && strcmp(extension, "bmp") != 0 &&
      strcmp(extension, "png") != 0) {
    printf("\nError: Wrong file format\n");
    close(sockfd);
    exit(1);
  }

  fp = fopen(fileName, "r");
  if (fp == NULL) {
    printf("\nError: File not found\n");
    close(sockfd);
    exit(1);
  }

  // send file name
  send(sockfd, fileName, strlen(fileName), 0);
  recv(sockfd, buffer, BUFF_SIZE, 0);
  int status = buffer[0] - '0';
  if (status == 0) {
    printf("\nError: File already exists\n");
    close(sockfd);
    exit(1);
  }

  while ((n = fread(buffer, 1, BUFF_SIZE, fp)) > 0) {
    send(sockfd, buffer, n, 0);
  }

  fclose(fp);
}

*/

void recvFile(int sockfd, char* fileName , char*path ){
  FILE* fp;
  char buffer[BUFF_SIZE];
  int n; 
  char* extension = get_extension(fileName);
  // kiem tra tap tin co dung cu phap hay khong
  if (strcmp(extension, "jpg") != 0 && strcmp(extension, "bmp") != 0 &&
      strcmp(extension, "png") != 0) {
    printf("\nError: Wrong file format\n");
    close(sockfd);
    exit(1);
  }
   // kiem tra xem co tap tin can tai tren server hay khong
   recv(sockfd,buffer, BUFF_SIZE, 0);
   int status = buffer[0] - '0';
   if(status == 0){
     printf("Error:File Not Found!\n");
     close(sockfd);
     exit(1);
   }
 
  // nhan file tu server va ghi vao thu muc
  send(sockfd,fileName, strlen(fileName), 0);
  fp = fopen(path,"wb");
   if (fp == NULL) {
    printf("Error opening file!\n");
    return ;
  }
  while ((n = recv(sockfd, buffer, BUFF_SIZE, 0)) > 0) {
    fwrite(buffer, sizeof(char), n, fp);
  }
  printf("\nSuccess: File transfer is completed\n");
  close(sockfd);
  fclose(fp);
}


int main(int argc , char * argv[]){
  if(argc != 4){
    printf("./client <IP> <PORT> <FILE NAME> \n");
    return 1;
  }
  char* SERVER_ADDR = argv[1];
  int SERVER_PORT = atoi(argv[2]);
  char* filename = argv[3];
  int client_sock;
  char buff[BUFF_SIZE];
  struct sockaddr_in server_addr; /* server's address information */
  int msg_len, bytes_sent, bytes_received;

  // Step 1: Construct socket
  client_sock = socket(AF_INET, SOCK_STREAM, 0);

  // Step 2: Specify server address
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(SERVER_PORT);
  server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

  // Step 3: Request to connect server
  if (connect(client_sock, (struct sockaddr*)&server_addr,
              sizeof(struct sockaddr)) < 0) {
    printf("\nError!Can not connect to sever! Client exit imediately! ");
    return 0;
  }

  // Step 4: send file to server
  char path[BUFF_SIZE] = "image_clinet_downloads/";
  strcat(path,filename);
  // kiem tra file da ton tai ben client chua
    send(client_sock,filename, strlen(filename), 0);
  FILE*fp;
  if((fp = fopen(path,"r")) != NULL){
     printf("File Already Exists! \n ");
     fclose(fp);
     close(client_sock);
  }else{
  recvFile(client_sock,filename,path);
  }
  // Step 5: Close socket
  close(client_sock);
  return 0;

}
