#include <stdio.h>
//#include <conio.h>
#include <string.h>
#include <stdlib.h>


int Mahoa_Ceaser(char *Nguon,char *Mahoa,int len,int key)
{
  int i;
  for(i = 0; i < len ; i ++){
    if(Nguon[i] >= 'A' && Nguon <= 'Z')
      Mahoa[i] = (Nguon[i]-'A'+key)%26+'A';
    else if(Nguon[i] >= 'a' && Nguon[i] <= 'z')
       Mahoa[i] = (Nguon[i]-'a'+key)%26+'a';
    else Mahoa[i] = Nguon[i];
  }
  return 0;
}
int Giaima_Ceaser(char *Nguon,char *GiaiMa,int len,int key){
 int i;
  for(i = 0; i < len ; i ++){
    if(Nguon[i] >= 'A' && Nguon <= 'Z')
      GiaiMa[i] = (Nguon[i]-'A'-key)%26+'A';
    else if(Nguon[i] >= 'a' && Nguon[i] <= 'z')
       GiaiMa[i] = (Nguon[i]-'a'-key)%26+'a';
    else GiaiMa[i] = Nguon[i];
  }
  return 0;
}
int khoitao(char *str,int SIZE){
  int i;
  for(i =0 ; i<SIZE;i++){
    str[i] = 0;
  }
  return 0;
}
