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
#include "Linklist.h"
#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 5550
#define BUFF_SIZE 1024

void NhanMess(int sockfd){
  char buff1[BUFF_SIZE];
  int n;
  n = recv(sockfd,buff1,BUFF_SIZE, 0);
  printf("%s\n",buff1);
    
}


int main(){
	int client_sock;
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
	
	//send message
	printf(" WelCom Grop Chat\n");
	printf("_______________________________\n");
	printf("vui long dang nhap tai khoan : ");
	memset(buff,'\0',(strlen(buff)+1));
	fgets(buff, BUFF_SIZE, stdin);		
	msg_len = strlen(buff);
		
	bytes_sent = send(client_sock, buff, msg_len, 0);
	if(bytes_sent < 0)
		perror("\nError: ");
        

	//receive echo reply
	bytes_received = recv(client_sock, buff, BUFF_SIZE, 0);
	if (bytes_received < 0)
	  perror("\nError: ");
	else if (bytes_received == 0)
	  printf("Connection closed.\n");
	int status = buff[0] - '0';
	if(status == 0){
	  printf("tai khoan ban khong ton tai\n");
	  printf("hay tim chu Grup va dang ky tai khoan\n");
	  close(client_sock);
	}
	if(status == 1){
	  printf("vui long nhap mat khau : ");
	  memset(buff,'\0',(strlen(buff)+1));
	  fgets(buff, BUFF_SIZE, stdin);		
	  msg_len = strlen(buff);
	  bytes_sent = send(client_sock, buff, msg_len, 0);
	  if(bytes_sent < 0)
		perror("\nError: ");
	  //nhan phan hoi tu sever
	  bytes_received = recv(client_sock, buff, BUFF_SIZE, 0);
	  status = buff[0] - '0';
	  
	  if(status == 0){
	    printf("mat khau sai !!!!");
	    close(client_sock);
	  }
	  if(status == 1){
	    printf("NHOM CHAT : \n");
	    // vong lap den khi client "bye" thi moi ket thuc
	    int check_loop = 1;
	  
	    strcpy(buff,"helloserver");
	    bytes_sent = send(client_sock, buff,BUFF_SIZE, 0);
	    printf("tin nhan truoc : \n");
	      NhanMess(client_sock);
	    
	      while(check_loop == 1){
		fgets(buff, BUFF_SIZE, stdin);
		msg_len = strlen(buff);
		bytes_sent = send(client_sock, buff,msg_len, 0);
		NhanMess(client_sock);	
	      }
	     
	  }

	}
	
	//Step 4: Close socket
	close(client_sock);
	return 0;
}
