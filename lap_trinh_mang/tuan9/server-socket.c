#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

//#include <ctype.h>
//Remember to use -pthread when compiling this server's source code

void *connection_handler(void *);

int CheckString(char * mess_client,int n){
  int i;
  for(i = 0 ; i < n ; i++){
    if((mess_client[i] < 65 && mess_client[i] > 90)||(mess_client[i] < 97 )){
      return 1;
    }
  }
  return 0;
}

void StringUpDate(char * mess_client, int n){
  int i = 0;
  for(i ; i <= n; ++i){
    if(mess_client[i] >= 'a' && mess_client[i] <= 'z'){
     mess_client[i] = mess_client[i]-32;
    }
  }
}

int main()
{
  //  char server_message[100] = "Hello from Server!!\n";
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
	
    if (server_socket==-1){
	perror("Socket initialisation failed");
	exit(EXIT_FAILURE);
	}
    else
	printf("Server socket created successfully\n");

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5500);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    
    //bind the socket to the specified IP addr and port
    if (bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr))!=0){
	printf("socket bind failed...\n"); 
        exit(0);
	}
    else
	printf("Socket successfully binded..\n"); 
    
    if (listen(server_socket, 3)!=0){
		printf("Listen failed...\n"); 
        exit(0); 
    } 
    else
        printf("Server listening..\n"); 
   
    int no_threads=0;
    pthread_t threads[3];
    while (no_threads<3){
      printf("Listening...\n");
	int client_socket = accept(server_socket, NULL, NULL);
	puts("Connection accepted");
	if(pthread_create( &threads[no_threads], NULL ,  connection_handler , &client_socket) < 0){
	perror("Could not create thread");
	return 1;}
    	if (client_socket < 0) { 
        	printf("server acccept failed...\n"); 
        	exit(0); 
    		} 
    	else
        	printf("Server acccept the client...\n");
	puts("Handler assigned");
	no_threads++;
	}
	int k=0;
    for (k=0;k<3;k++){
	pthread_join(threads[k],NULL);
}

    //int send_status;
    //  send_status=send(server_socket, server_message, sizeof(server_message), 0);
    close(server_socket);
    
    return 0;
}


void *connection_handler(void *client_socket){
	int socket = *(int*) client_socket;
	int read_len;
       	char server_message[100]="Hello from server\n";
	int send_status;
    	send_status = send(socket, server_message, sizeof(server_message), 0);
	char client_message[100];
	char *remove_n;
	while( (read_len=recv(socket,client_message, 100,0))>0)
	{
	  if((remove_n = strchr(client_message,'\n')) != NULL)	{
	  *remove_n = '\0';
	}
	  int status = 0;//CheckString(client_message,read_len);
	 if(status == 1){
	   bzero(client_message,0);
	   strcpy(client_message,"Wrong text format");
	   send_status = send(socket , client_message , sizeof(client_message),0);
	   close(socket);
	 }
	 if(status == 0){
	  
	   StringUpDate(client_message,read_len);
	   send_status = send(socket,client_message,sizeof(client_message),0);
	  
	 }
	}
	
	return 0;
}

