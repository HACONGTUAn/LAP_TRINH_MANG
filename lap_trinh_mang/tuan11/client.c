#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <sys/select.h>
#include <unistd.h>
#include <fcntl.h>
#include "Ceaser.h"
#include <ctype.h>
#define MAX_BUFFER_SIZE 1024
#define CONNECTED "CONNECT"
char client_name[20] = {0};
 char result[100];
int key = 0;
int client_create_socket(int * listen_fd , char * SERVER_ADDR,int SERVER_PORT);
int client_send_to_server(int client_socket, char * send_msg);
int client_build_fdsets(int listenfd, fd_set * readfds, fd_set *writefds, fd_set *exceptfds);
int client_select(int max_fd,int listen_fd,fd_set *readfds, fd_set *writefds);
int client_recv_from_server(int client_socket,char*recv_msg,char*Gia_ma);
int Tach_chuoi_so(char *str);

int main(int argc , char * argv[] ){
  char* SERVER_ADDR;
  int SERVER_PORT;
  int listen_fd = 0;
  int new_socket = 0;
  int max_fd = 0;
  fd_set readfds;
  fd_set writefds;
  fd_set exceptfds;
  int check;
  int check_v2;
  char byte[10];
  if(argc != 4){
    printf("ban phai nhap <IP> <PORT> <NAME>\n");
    return 1;
  }

  SERVER_ADDR = argv[1];
  SERVER_PORT = atoi(argv[2]);
  strcpy(client_name,argv[3]);
  if(client_create_socket(&listen_fd,SERVER_ADDR,SERVER_PORT) != 0){
    perror("ERROR : socket creation failed");
    exit(0);
  }
  
  max_fd = listen_fd;
  if((check = recv(listen_fd,byte,sizeof(byte),0)) > 0){
    printf("connect...\n");
    int len = strlen(CONNECTED);
     if((check_v2 = send(listen_fd,CONNECTED,len,0)) <= 0){
    perror("ERROR : send failed");
    return -1;
  }
  }
  while(1){
    max_fd = client_build_fdsets(listen_fd,&readfds,&writefds,&exceptfds);
    client_select(max_fd,listen_fd,&readfds,&writefds);
  }
  close(listen_fd);
  return 0;
}

int client_create_socket(int * listen_fd , char * SERVER_ADDR,int SERVER_PORT){
  struct sockaddr_in server_addr;
  if((*listen_fd = socket(AF_INET,SOCK_STREAM,0)) == -1){
    perror("ERROR : socket creation failed");
    return -1;
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(SERVER_PORT);
  server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

  if(0 != connect(*listen_fd,(struct sockaddr *)&server_addr,sizeof(struct sockaddr))){
    perror("ERROR: connect failed");
    return -1;
  }
  client_send_to_server(*listen_fd,client_name);
  
  return 0;
}

int client_send_to_server(int client_socket, char * send_msg){
  int write_bytes = 0;
  int len = sizeof(send_msg);
  char Ma_hoa[len];
  Mahoa_Ceaser(send_msg,Ma_hoa,len,key);
  if((write_bytes = send(client_socket,Ma_hoa,len,0)) <= 0){
    perror("ERROR : send failed");
    return -1;
  }

  return write_bytes;
}
int client_build_fdsets(int listenfd, fd_set * readfds, fd_set *writefds, fd_set *exceptfds){
  int max_fd = listenfd;

  FD_ZERO(readfds);
  FD_SET(listenfd,readfds);
  FD_SET(listenfd,writefds);
  FD_SET(STDIN_FILENO,readfds);
  fcntl(STDIN_FILENO,F_SETFL,O_NONBLOCK);
  return max_fd;
}

int client_select(int max_fd,int listen_fd,fd_set *readfds, fd_set *writefds){
  char recv_msgg[MAX_BUFFER_SIZE];
  char send_buff[MAX_BUFFER_SIZE];
  char Giai_ma[MAX];
  bzero(recv_msgg,sizeof(recv_msgg));
  bzero(send_buff,sizeof(send_buff));
  bzero(Giai_ma,sizeof(Giai_ma));
  int action = select(max_fd+1,readfds,writefds,NULL,NULL);

  if(action == -1 || action == 0){
    perror("ERROR : select");
    exit(0);
  }

  if(FD_ISSET(listen_fd,readfds)){
    client_recv_from_server(listen_fd,recv_msgg,Giai_ma);
  }

  if(FD_ISSET(STDIN_FILENO,readfds)){
    if(read(0,send_buff,sizeof(send_buff)) > 0){
      client_send_to_server(listen_fd,send_buff);
    }
  }
  return 0;
}

int client_recv_from_server(int client_socket,char*recv_msg,char* Giai_ma){
  int read_bytes = 0;
  int len = sizeof(Giai_ma);
  khoitao(Giai_ma);
  if((read_bytes = recv(client_socket,recv_msg,MAX_BUFFER_SIZE,0))>0){
    if(strncmp(recv_msg,"Secret_key",10) == 0){
      Tach_chuoi_so(recv_msg);
      key = atoi(result);
    }
    else{
    Giaima_Ceaser(recv_msg,Giai_ma,len,key);
    //printf("%s-",recv_msg);
     printf("%s\n",Giai_ma);
    }
  }
  else if(read_bytes == 0){
    printf("client disconnected\n");
    close(client_socket);
  }
  else{
    printf("ERROR: recv failed\n");
  }
  return 0;
}
int Tach_chuoi_so(char *str){
  int i = 0;
  int n = 0;
  int len = strlen(str);
  
  for(i ; i <= len ; ++i){
    if(isdigit(str[i])){
      result[n] = str[i];
      n++;
      result[n] = '\0';
    }else if(n){
      n = 0;
    }
  }
  return 0;
}
