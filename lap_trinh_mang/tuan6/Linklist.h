#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct node  
{
   char name[20];
   char path[100];
   struct node *next;
};

struct node *head = NULL;
struct node *current = NULL;

//hien thi danh sach
void printList()
{
   struct node *ptr = head;
  
	
   //bat dau tu phan dau danh sach
   while(ptr != NULL)
	{        
	  printf("%s %s\n",ptr->name,ptr->path);
      ptr = ptr->next;
   }
	
  
}

//chen link tai vi tri dau tien
void insertFirst(char *name, char *path)
{
   //tao mot link
   struct node *link = (struct node*) malloc(sizeof(struct node));
	
   strcpy(link->name, name);
   strcpy(link->path,path);
  
  
	
   //tro link nay toi first node cu
   link->next = head;
	
   //tro first toi first node moi
   head = link;
}

//xoa phan tu dau tien
struct node* deleteFirst()
{

   //luu tham chieu toi first link
   struct node *tempLink = head;
	
   //danh dau next toi first link la first 
   head = head->next;
	
   //tra ve link bi xoa
   return tempLink;
}

//kiem tra list co trong hay khong
bool isEmpty()
{
   return head == NULL;
}

int length()
{
   int length = 0;
   struct node *current;
	
   for(current = head; current != NULL; current = current->next)
	{
      length++;
   }
	
   return length;
}

//tim mot link voi key da cho
struct node* find(char *name){
   //bat dau tim tu first link
   struct node* current = head;

   //neu list la trong
   if(head == NULL)
	{
      return NULL;
   }

   //duyet qua list
   while(strcmp(current->name,name) != 0 ){
	
      //neu day la last node
      if(current->next == NULL){
         return NULL;
      }else {
         //di chuyen toi next link
         current = current->next;
      }
   }      
	
   //neu tim thay du lieu, tra ve link hien tai
   return current;
}

//xoa mot link voi key da cho
struct node* deleteKey(char *name){

   //bat dau tu first link
   struct node* current = head;
   struct node* previous = NULL;
	
   //neu list la trong
   if(head == NULL){
      return NULL;
   }

   //duyet qua list
   while(strcmp(current->name,name) != 0){
	
      //neu day la last node
      if(current->next == NULL){
         return NULL;
      }else {
         //luu tham chieu toi link hien tai
         previous = current;
         //di chuyen toi next link
         current = current->next;             
      }
		
   }

   //cap nhat link
   if(current == head) {
      //thay doi first de tro toi next link
      head = head->next;
   }else {
      //bo qua link hien tai
      previous->next = current->next;
   }    
	
   return current;
}

//tim mot link voi key da cho
struct node* find2(char *path){

   //bat dau tim tu first link
   struct node* current = head;

   //neu list la trong
   if(head == NULL)
	{
      return NULL;
   }

   //duyet qua list
   while(strcmp(current->path,path) != 0 ){
	
      //neu day la last node
      if(current->next == NULL){
         return NULL;
      }else {
         //di chuyen toi next link
         current = current->next;
      }
   }      
	
   //neu tim thay du lieu, tra ve link hien tai
   return current;
}
