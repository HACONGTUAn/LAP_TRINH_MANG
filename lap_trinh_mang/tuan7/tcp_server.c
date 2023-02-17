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
#define PORT 5550
#define BACKLOG 20
#define BUFF_SIZE 1024

//char Buffer[BUFF_SIZE];
 char path[BUFF_SIZE] = "data_server/";
/* Handler process signal*/
void sig_chld(int signo);

/*
* Receive and echo message to client
* [IN] sockfd: socket descriptor that connects to client 	
*/
void echo(int sockfd);

// server : nhan file va chuyen thanh chu hoa
void recvFile(int sockfd, char* fileName, char* path){
  FILE*fp;
  char buffer[BUFF_SIZE];
  char Buffer[BUFF_SIZE];
  int n; 
  int i;
  strcat(path,fileName);
  // nhan file tu client va ghi vao thu muc
  fp = fopen(path,"wb");
   if (fp == NULL) {
    printf("Error opening file!\n");
    return ; 
  }
  while ((n = recv(sockfd, buffer, BUFF_SIZE, 0)) > 0) {
    for(i = 0 ; i < n; i++){
      Buffer[i] = toupper(buffer[i]);
    }
    fwrite(Buffer, sizeof(char), n, fp);
    send(sockfd,Buffer,BUFF_SIZE,0);
  }
  fclose(fp);
}
//server:gui file nguoc lai file client
void sendFile(int conn_sock,char path[]){
  FILE* fp;
  char buffer[BUFF_SIZE];
  int n;
  fp = fopen(path,"r");
  if(fp == NULL){
    printf("khong ton tai file\n");
    close(conn_sock);
    exit(1);
  }
  while ( (n=fread(buffer,1, BUFF_SIZE, fp)) >  0) {
     if(send(conn_sock, buffer, n, 0) < 0){
       printf("loi gui file \n");
     }
  }
  printf("da gui file cho client\n");
  // close(conn_sock);
   fclose(fp);
}

int main(){
 
	int listen_sock, conn_sock; /* file descriptors */
	struct sockaddr_in server; /* server's address information */
	struct sockaddr_in client; /* client's address information */
	pid_t pid;
	int sin_size;

	if ((listen_sock=socket(AF_INET, SOCK_STREAM, 0)) == -1 ){  /* calls socket() */
		printf("socket() error\n");
		return 0;
	}
	
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;         
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = htonl(INADDR_ANY);  /* INADDR_ANY puts your IP address automatically */   

	if(bind(listen_sock, (struct sockaddr*)&server, sizeof(server))==-1){ 
		perror("\nError: ");
		return 0;
	}     

	if(listen(listen_sock, BACKLOG) == -1){  
		perror("\nError: ");
		return 0;
	}
	
	/* Establish a signal handler to catch SIGCHLD */
	signal(SIGCHLD, sig_chld);

	while(1){
		sin_size=sizeof(struct sockaddr_in);
		if ((conn_sock = accept(listen_sock, (struct sockaddr *)&client, &sin_size))==-1){
			if (errno == EINTR)
				continue;
			else{
				perror("\nError: ");			
				return 0;
			}
		}
		
		/* For each client, fork spawns a child, and the child handles the new client */
		pid = fork();
		
		/* fork() is called in child process */
		if(pid  == 0){
			close(listen_sock);
			printf("You got a connection from %s\n", inet_ntoa(client.sin_addr)); /* prints client's IP */
			echo(conn_sock);					
			exit(0);
		}
		
		/* The parent closes the connected socket since the child handles the new client */
		close(conn_sock);
	}
	close(listen_sock);
	return 0;
}

void sig_chld(int signo){
	pid_t pid;
	int stat;
	
	/* Wait the child process terminate */
	while((pid = waitpid(-1, &stat, WNOHANG))>0)
		printf("\nChild %d terminated\n",pid);
}

void echo(int sockfd) {
	char buff[BUFF_SIZE];
	int bytes_sent, bytes_received,n;
	
	bytes_received = recv(sockfd, buff, BUFF_SIZE, 0); //blocking
	if (bytes_received < 0)
		perror("\nError: ");
	else if (bytes_received == 0)
		printf("Connection closed.");
	
	bytes_sent = send(sockfd, buff, bytes_received, 0); /* echo to the client */
	if (bytes_sent < 0)
		perror("\nError: ");
	buff[bytes_received] = '\0';
	char name[BUFF_SIZE];
	char path2[BUFF_SIZE] = "data_server/";
	strcpy(name,buff);
	//nhan file
	recvFile(sockfd,buff,path);	
	strcat(path2,name);
	//gui file
	sendFile(sockfd,path2);
	
	close(sockfd);
}
