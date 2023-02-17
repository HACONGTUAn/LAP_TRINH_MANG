#include<stdio.h>
#include<sys/socket.h>
#include<netdb.h>
#include<arpa/inet.h>
int main(int argc , char* arg[]){
  int i;
  struct hostent *he;
  struct in_addr **aliases;
  struct in_addr addr;
  char **addstr;
  if(argc > 2){
    printf("chi duoc 1 tham so \n");
    return 0;
  }
  else{

    if(inet_aton(arg[1],&addr) != NULL){
  he = gethostbyaddr(&addr, sizeof(addr),AF_INET);
    
    if(he == NULL){
      printf("khong tim thay dia chia\n");
      return 1;
    }
     
  printf("host name : %s \n",he->h_name);
printf("Alias name : \n");
  for(addstr = he->h_aliases ; *addstr != NULL  ; addstr++){
    printf("%s \n", *addstr);
  }
    }

    if(inet_aton(arg[1],&addr) == NULL){
      
     he =  gethostbyname(arg[1]);
     printf("%s\n",he->h_name);
    
    if(he == NULL){
      printf("khong tim thay dia chia\n");
      return 1;
    }
     
    printf("Official IP: %s \n",inet_ntoa(*(struct in_addr*)he->h_addr));
  
  printf("Alias IP : \n");
  for(addstr = he->h_aliases ; *addstr != NULL  ; addstr++){
    printf("%s \n", *addstr);
  }
    }
  }
}
