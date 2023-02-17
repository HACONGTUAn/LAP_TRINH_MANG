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
#define PORT 5550
#define BACKLOG 20
#define BUFF_SIZE 1024

/* Handler process signal*/
void sig_chld(int signo);

/*
* Receive and echo message to client
* [IN] sockfd: socket descriptor that connects to client 	
*/
void echo(int sockfd);
//kiem tra xem nguoi dung co trong danh sach khach hay khong
int check_name(char * buff){
  if(find(buff) != NULL){
    return 1;
  }
  return 0;
}
//kiem tra mat khau
int check_pass(char * buff){
  if(find2(buff) != NULL){
    return 1;
  }
  return 0;
}
void GuiFile(int sockfd){
  int buffer[BUFF_SIZE];
  int n;
  FILE*fp;
  if((fp = fopen("groupchat.txt","r")) == NULL){
    printf("khong mo duoc file\n");
    exit(1);
  }
   while ( (n=fread(buffer,1, BUFF_SIZE, fp)) >  0) {
     if(send(sockfd, buffer, n, 0) < 0){
       printf("loi gui file \n");
     }
   }
  fclose(fp);
}
void NhanMess(int sockfd){
  char buff[BUFF_SIZE];
  int bytes_received;
   bytes_received = recv(sockfd, buff, BUFF_SIZE, 0);
   if (bytes_received < 0){
     perror("\nError: ");
     
   }
   FILE * fp;
   if((fp = fopen("groupchat.txt","a"))==NULL){
     printf("khong mo duoc file \n");
     exit(1);
   }
   fwrite(buff,1,bytes_received,fp);
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
	/************************************************/
  FILE*fptr;
  char name[20];
  char pass[20];
  int status;
  struct node *ptr;
  fptr = fopen("account.txt","r");
  if(fptr == NULL){
    printf("khong mo dc file\n");
  }
  // dua data vao link_list
while( fscanf(fptr,"%s %s %d ",name,pass,&status) != EOF){
	  insertFirst(name,pass,status);
	}
 fclose(fptr);
 // printList();
	/*************************************************/
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
	int bytes_sent, bytes_received;
	char* remove_n;
	bytes_received = recv(sockfd, buff, BUFF_SIZE, 0); //blocking
	if (bytes_received < 0)
		perror("\nError: ");

        if (bytes_received == 0)
		printf("Connection closed.");
	if((remove_n = strchr(buff,'\n')) != NULL)	{
	  *remove_n = '\0';
	}
	if(check_name(buff) == 0){
	  bytes_sent = send(sockfd,"0",1,0);
	}
	if(check_name(buff) == 1){
	  bytes_sent = send(sockfd,"1",1,0);
	  // strcmp(buff,"hello");
	  bytes_received = recv(sockfd, buff, BUFF_SIZE, 0);
	  if (bytes_received < 0)
	    perror("\nError: ");
	  
	  if (bytes_received == 0)
	    
	    printf("Connection closed.");
	  if((remove_n = strchr(buff,'\n')) != NULL)	{
	    *remove_n = '\0';
	  }
	  if(check_pass(buff) == 0){
	    bytes_sent = send(sockfd,"0",1,0);
	  }
	  if(check_pass(buff) == 1){
	    bytes_sent = send(sockfd,"1",1,0);
	    
	    bytes_received = recv(sockfd, buff, BUFF_SIZE, 0);
	   
	    if(strcmp(buff,"helloserver")==0){
	      printf("gui mess cho client\n");
	      GuiFile(sockfd);
	      int check_loop = 1;
	      while(check_loop == 1){
	      NhanMess(sockfd);
	      if (bytes_sent < 0){
		perror("\nError: ");
		check_loop =0;
	      }
	      //bytes_sent = send(sockfd,"1",1,0);
	      GuiFile(sockfd);
	      }
	    }
	  }
 /* echo to the client */
	if (bytes_sent < 0)
		perror("\nError: ");
	}
	close(sockfd);
}
