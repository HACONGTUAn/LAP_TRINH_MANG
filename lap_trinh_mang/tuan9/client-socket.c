#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <ctype.h>
#include <sys/wait.h>
int main(int argc, const char * argv[]) {
    //create a socket
    int network_socket;
    network_socket=socket(AF_INET,SOCK_STREAM,0);
    
    //specify an address for the socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(5500);
    server_address.sin_addr.s_addr=INADDR_ANY;
    
    int connection_status = connect(network_socket, (struct sockaddr *) & server_address, sizeof(server_address));
    //check for connection_status
    if(connection_status==-1)
    {
        printf("The connection has error\n\n");
    }
    
    if(connection_status==0)
    {
      
    //receive data from the server
	char response[256];
    	recv(network_socket, &response, sizeof(response), 0);
    while(1){

	printf("enter a message to echo\n");
	bzero(response,0);
	fgets(response,strlen(response),stdin);
    	int send_status= send(network_socket,response, sizeof(response),0);
	if (strcmp(response,"q")==0){break;}
	if (strcmp(response,"Q")==0){break;}
	else{
	  int size_recv;
	  size_recv = recv(network_socket,&response,sizeof(response),0);
	  if(strcmp(response,"Wrong text format") == 0){
	    printf("Wrong text format\n");
	    break;
	  }
	  else{
	    response[size_recv] = '\0';
	    
	    printf("server tra ve : %s \n",response);
	  }
	}
    }
	}
    //close the socket
    close(network_socket);
    
    return 0;
}
