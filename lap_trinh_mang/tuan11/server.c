#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/select.h> 
#include <sys/time.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

#define PORT 5500   /* Port that will be opened */ 
#define BACKLOG 20   /* Number of allowed connections */
#define MAX_BUFFER_SIZE 1024
#define TIMEOUT 20
#define SECRET_KEY 0
#define MAX_NAME_SIZE 20
#define NO_OF_CLIENTS 10
#define Secretkey "Secret_key"
static int ALL_CLIENT[NO_OF_CLIENTS];
static int listen_fd = 0;
static int count = 0;
// data structure client
struct client
{ char name[MAX_NAME_SIZE];
  char chat[MAX_NAME_SIZE];
  int file_fd;
  int chat_fd[NO_OF_CLIENTS];
  int port;
  char ip[INET_ADDRSTRLEN];
};
// struct server
struct server_data
{
  int so_client;
  struct client list_client[NO_OF_CLIENTS];
};

struct server_data server;

int server_create_socket(int * listen_fd);
int server_build_fdset(int listen, fd_set *readfds,fd_set *writefds, fd_set *exceptfd);
int server_select(int max_fd,int listen_fd, fd_set * readfds,fd_set * writefds);
int server_new_client_handle(int listen_fd,int * new_socket_fd);
void server_deleta_client(int client_socket);
void server_add_client(struct sockaddr_in client_addr,int new_socket_fd);
int server_recv_from_client(int new_socket_fd,char *buffer);
int process_recv(int client_socket,char * buffer);
int find_the_client_index_list(int client_socket);
int server_send_to_client(int client_socket, char * mess);
void cleanup(void);
int Ramdom(int min,int max);

int main(){
  int socket = 0;
  fd_set readfds;
  fd_set writefds;
  fd_set exceptfd;
  int max_fd = 0;

  memset(&server,0,sizeof(struct server_data));
  printf("SERVER Started \n");

  if(server_create_socket(&listen_fd) != 0){
    perror("ERROR : create soket false");
    exit(0);
  }
  max_fd = listen_fd;
  while(1){
    max_fd = server_build_fdset(listen_fd,&readfds,&writefds,&exceptfd);
    server_select(max_fd, listen_fd, &readfds,&writefds);
  }

  cleanup();
  printf("bye from server\n");
  return 0;
}

int server_create_socket(int * listen_fd){
  struct sockaddr_in server_add;

  if((*listen_fd = socket(AF_INET,SOCK_STREAM,0)) == -1){
    perror("ERROR : socket created fales");
    return -1;
  }

  server_add.sin_family = AF_INET;
  server_add.sin_port = htons(PORT);
  server_add.sin_addr.s_addr = INADDR_ANY;

  if(0 != bind(*listen_fd,(struct sockaddr*)&server_add,sizeof(struct sockaddr))){
    perror("ERROR : creat bind false");
    return -1;
  }

  
  if(0 != listen(*listen_fd,BACKLOG)){
    perror("ERROR : creat listen false");
    return -1;
  }
  return 0;
}
int server_build_fdset(int listen, fd_set *readfds,fd_set *writefds, fd_set *exceptfd){
  int max_fd = listen;

  FD_ZERO(readfds);
  FD_SET(listen,readfds);
  FD_SET(STDIN_FILENO,readfds);
  fcntl(STDIN_FILENO,F_SETFL,O_NONBLOCK);
  
  for(int i = 0 ; i < server.so_client ; i++){
    FD_SET(server.list_client[i].file_fd,readfds);
    max_fd++;
  }
  return max_fd;
}
int server_select(int max_fd,int listen_fd, fd_set * readfds,fd_set * writefds){
  int new_socket_fd = 0;
  char recv_mess[MAX_BUFFER_SIZE];
  char send_buff[MAX_BUFFER_SIZE];
  int key;
  int i;
  struct timeval tv;
  tv.tv_sec = TIMEOUT;
  tv.tv_usec = 0;
  memset(recv_mess,0,sizeof(recv_mess));
  memset(send_buff,0,sizeof(send_buff));

  int acction = select(max_fd+1,readfds,writefds,NULL,&tv);

  if(acction == -1){
    perror("ERROR : SELECT");
    exit(0);
  }
  if(acction == 0){
    key = Ramdom(1,25);
    sprintf(send_buff,"%s %d",Secretkey,key);
    for( i = 0; i < server.so_client ; i++){
	server_send_to_client(ALL_CLIENT[i],send_buff);
    }
  }
  // lang nghe tu o cam 
  if(FD_ISSET(listen_fd,readfds)){
    server_new_client_handle(listen_fd,&new_socket_fd);
    printf("new socket cread = %d \n",new_socket_fd);
  }
  // lang nghe tu cac client
  for(int i = 0 ; i < server.so_client ; i++){
    if(FD_ISSET(server.list_client[i].file_fd,readfds)){
      printf("***************************************\n");
      printf("%d\n",server.list_client[i].file_fd);
      server_recv_from_client(server.list_client[i].file_fd,recv_mess);
    }
  }
  return 0;
}

int server_new_client_handle(int listen_fd,int * new_socket_fd){
  struct sockaddr_in client_addr;
  int len = sizeof(struct sockaddr);
  
  bzero(&client_addr,sizeof(client_addr));
  if((*new_socket_fd = accept(listen_fd,(struct sockaddr*)&client_addr,&len)) <0){
    perror("ERROR : accept faile");
    exit(1);
  }
  ALL_CLIENT[count] = *new_socket_fd;
  server_add_client(client_addr,*new_socket_fd);
  count++;
}

void server_add_client(struct sockaddr_in client_addr,int new_socket_fd){
  char ip[INET_ADDRSTRLEN] = {0};
  char buffer[MAX_BUFFER_SIZE] = {0};
  int check = 0;
  server_recv_from_client(new_socket_fd,buffer);
  if(check == 0){
    server_send_to_client(new_socket_fd,"hello");
    check ++;
  }
  int port = ntohs(client_addr.sin_port);
  inet_ntop(AF_INET,&(client_addr.sin_addr),ip,INET_ADDRSTRLEN);
  printf("client info : port : %d | ip : %s \n",port,ip);

  // them client vao struct
  strncpy(server.list_client[server.so_client].name,buffer,strlen(buffer));
  printf("name client : %s \n ",server.list_client[server.so_client].name);
  server.list_client[server.so_client].port = port;
  strcpy(server.list_client[server.so_client].ip,ip);
  server.list_client[server.so_client].file_fd = new_socket_fd;

  server.so_client++;
}

void server_delete_client(int socket_fd_del){
  int i = 0;
  int index = 0;
  for(i = 0 ; i < NO_OF_CLIENTS ; i++){
    if(server.list_client[i].file_fd == socket_fd_del){
      for(index = i ; index < NO_OF_CLIENTS; index ++){
	server.list_client[index] =  server.list_client[index+1];
      }
    }
  }
  server.so_client --;
  printf("socket delete = [%d]\n",socket_fd_del);
  close(socket_fd_del);
}

int server_recv_from_client(int new_socket_fd,char *buffer){
  int read_byte = 0;

  memset(buffer,0,strlen(buffer));

  if((read_byte = recv(new_socket_fd,buffer,MAX_BUFFER_SIZE,0)) > 0){
    process_recv(new_socket_fd,buffer);
  }
  else if(read_byte == 0){
    printf("client disconnect\n");
    server_delete_client(new_socket_fd);
  }
  else{
    printf("ERROR:recv failse\n");
  }
  return 0;
}
int process_recv(int client_socket,char * buffer){
  char chat_c[MAX_BUFFER_SIZE];
  char buffer_send[MAX_BUFFER_SIZE] = {0};
  int index_sender = 0;
  int index_recv = 0;
  int len = 0;
  int i;
  index_sender = find_the_client_index_list(client_socket);
  
  if(strncmp(buffer,"CONNECT",7) == 0 ){
    strcpy(server.list_client[index_sender].chat,"ALL_CLIENT");
    goto out;
  }
  // in ra danh sach nhung nguoi co trong phien chat
  if(strncmp(buffer,"LIST",4) == 0){
    memset(buffer,0,sizeof(buffer));
    for(i = 0 ; i < server.so_client ; i++){
      strcat(buffer,server.list_client[i].name);
      strcat(buffer,";");
    }
    server_send_to_client(client_socket,buffer);
    goto out;
  }
  // gui cho toan bo clien
  if((strcmp(buffer,"LIST") != 0) && (strcmp(buffer,"CONNECT") != 0) && (strlen(server.list_client[index_sender].name) != 0) ){
    //snprintf(buffer_send,sizeof(buffer_send),"%s:%s",server.list_client[index_sender].name, buffer);
    // printf("buffer = %s \n",buffer_send);
    for( i = 0; i < server.so_client ; i++){
      if(server.list_client[index_sender].file_fd != ALL_CLIENT[i])
      server_send_to_client(ALL_CLIENT[i],buffer);
    }
  }
 out:
  return 0;
}

int find_the_client_index_list(int client_socket){
  int index = 0;
  for(int i = 0 ; i < count ; i++){
    if(ALL_CLIENT[i] == client_socket){
      index = i;
    }
  }
  return index;
}

int server_send_to_client(int client_socket, char * mess){
  int write_byte = 0;
  int len = strlen(mess);
  if((write_byte = send(client_socket,mess,len,0)) > 0 ){
    printf("toan bo client nhan duoc tin tu %s \n",mess);
  }else{
    perror("ERROR : send failed");
    return -1;
  }
  return write_byte;
}

void cleanup(){
  close(listen_fd);
  for(int i = 0 ; i < server.so_client; i++){
    close(server.list_client[i].file_fd);
  }
}

int Ramdom(int min, int max){
  return min+(int)(rand()*(max-min+1.0)/(1.0+RAND_MAX));
}
