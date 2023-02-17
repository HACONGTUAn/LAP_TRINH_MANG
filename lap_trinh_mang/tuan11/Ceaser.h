#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 1024

int Mahoa_Ceaser(char s[],char Str_Mahoa[],int len,int k)
{
	int i;
int rotation;
for( i = 0; i< len; i++) {
    if (s[i] >= 'a' && s[i] <= 'z') {
        if((s[i] + k) > 'z' ) {
            rotation = (s[i] - 26) + k;
            Str_Mahoa[i] = rotation;
        } else {
            Str_Mahoa[i] = s[i]+k;
        }

    } else if (s[i] >= 'A' && s[i] <= 'Z') {
        if((s[i] + k) >= 'Z' ) {
            rotation = (s[i] - 26) + k;
            Str_Mahoa[i] = rotation;
        } else {
            Str_Mahoa[i] = s[i]+k;
        }
    } 
}
return 0;
}



int Giaima_Ceaser(char s[],char Str_Giaima[],int len,int k)
{	int i;
int rotation;
for( i = 0; i< len; i++) {
    if (s[i] >= 'a' && s[i] <= 'z') {
        if((s[i] - k) < 'a' ) {
            rotation = (s[i] + 26) - k;
            Str_Giaima[i] = rotation;
        } else {
            Str_Giaima[i] = s[i]-k;
        }

    } else if (s[i] >= 'A' && s[i] <= 'Z') {
        if((s[i] - k) < 'A' ) {
            rotation = (s[i] + 26) - k;
            Str_Giaima[i] = rotation;
        } else {
            Str_Giaima[i] = s[i]-k;
        }
    } 
}
 return 0;
}
int khoitao(char * Str){
  for(int i = 0 ; i < MAX ; i++){
    Str[i] = 0;
  }
  return 0;
}
