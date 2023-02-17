#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 5500
#define BUFF_SIZE 1024

void Menu(){
  printf("____________________________\n");
  printf("Vui long chon chuc nang \n");
  printf("1. MA HOA(phim 1)\n");
  printf("2. GIAI MA(phim 2)\n");
  printf("_____________________________\n");

}
/* client gui thong diep ma hoa hoac giai ma cho server */
void OpCode(char * buff);

void setPayLoad();

int main(){
  int8_t opcode;
  int16_t length;
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
	Menu();
	printf("chon chuc nang : \n");
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
		
	buff[bytes_received] = '\0';
	printf("Reply from server: %s", buff);
	
	
	//Step 4: Close socket
	close(client_sock);
	return 0;
}
void OpCode(int s, char * buff , int8_t opcode){
  char buffe[BUFF_SIZE];
  char namefile[30];
  int msg_len;
  // giai ma
  if(strcmp(buff,"1") == 0 ){
    printf("nhap khoa : ");
    fgets(buffe, BUFF_SIZE, stdin);
    msg_len = strlen(buffe);
    opcode = 1;
    // gui opcode len server
    send(s,buffe, msg_len, 0);
    // nhan thong bao tu server
    recv();
    //gui file len server
    printf("nhap ten file can giai ma : ");
    scanf("%s",namefile);
    FILE *ptr;
    if(ptr = fopen(namefile,"r") == NULL){
      printf("khong mo dc file \n");
    }
    bzero(buffe,"\0");
    while(fgets(buffe,BUFF_SIZE,ptr) == NULL); 
    msg_len = strlen(buffe);
    send(s,buffe,msg_len,0);      
    }
    fclose(ptr);
  }
  // ma hoa
  if(strcmp(buff,"0") == 0 ){
    printf("nhap khoa : ");
    scanf("%d",&key);
    fgets(buffe, BUFF_SIZE, stdin);
    msg_len = strlen(buffe);
    opcode = 1;
    send(s,buffe, msg_len, 0);
  }
    
}
void setPayLoad(){

}
