#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/wait.h>
#include <errno.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <string.h>
#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 5550
#define BUFF_SIZE 1024
char buffer[BUFF_SIZE];

// gui file len server
void sendFile(int conn_sock,char path[]){
  FILE* fp;
  char buffer1[BUFF_SIZE];
  int n;

  fp = fopen(path,"r");
  if(fp == NULL){
    printf("khong ton tai file\n");
    exit(1);
  }
   while ((n = fread(buffer1, 1, BUFF_SIZE, fp)) > 0) {
    send(conn_sock, buffer1, n, 0);
  }
   printf("file da duoc gui len sever xu ly\n");
   //  close(conn_sock);
   fclose(fp);
}

void ThuongThanhHoa(char buff[],int n){
  int i;
  for(i = 0 ; i < n ; i++){
    buffer[i] = toupper(buff[i]);
  }
}

// nhan file tu server 
void recvFile(int sockfd, char*path ){
  FILE* fp;
  char buff1[BUFF_SIZE];
  int n;
 
  fp = fopen(path,"wb");
  if (fp == NULL) {
    printf("Error opening file!\n");
    close(sockfd);
    exit(1);
   }
  n = recv(sockfd,buff1,BUFF_SIZE, 0);
  fputs(buff1,fp);
  
  //close(sockfd);
  fclose(fp);   
  printf("file da duoc xu ly xong !!!! \n");
}

int main(int argc, char * argv[]){
  if(argc != 2){
    printf("./client <path file name>");
    return 1;
  }
  char *filename = argv[1];
  int client_sock,n;
	char buff[BUFF_SIZE + 1];
	struct sockaddr_in server_addr; /* server's address information */
	int msg_len, bytes_sent, bytes_received;
	
	//Step 1: Construct socket
	client_sock = socket(AF_INET,SOCK_STREAM,0);
	
	//Step 2: Specify server address
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
	
	//Step 3: Request to connect server
	if(connect(client_sock, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) < 0){
		printf("\nError!Can not connect to sever! Client exit imediately! ");
		return 0;
	}
		
	//Step 4: Communicate with server

	char path[BUFF_SIZE] = "data_client/";
	strcat(path,filename);
	//send message
	send(client_sock,filename, strlen(filename), 0);// gui ten file
	// nhan thong bao tu phia server
	n = recv(client_sock, buff, BUFF_SIZE, 0);
	if(n == 0 ){
	  // server khong nhan duoc ten cua tap tin
	  printf("server khong phan hoi \n");
	}
	else{
	  buff[n] = '\0';
	  ThuongThanhHoa(buff,n);
	  sendFile(client_sock,path);
	  char path2[BUFF_SIZE] = "data_client/";
	  strcat(path2,buffer);
	  recvFile(client_sock,path2);
	   close(client_sock);
	}
	return 0;
}
