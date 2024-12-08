// list/list.c
// 
// Implementation for linked list.
//
// <Author>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

list_t *list_alloc() { 
  list_t* mylist =  (list_t *) malloc(sizeof(list_t)); 
  mylist->head = NULL;
  return mylist
}

void list_free(list_t *l) {
  node_t* curr = l -> head;
  node_t *next;
  while (curr != NULL){
   next = curr-> next;
   free(curr);
   curr = next;
  }
  free(l);
}

void list_print(list_t *l) {
  node_t* curr = l -> head;
  while (curr != NULL){
    printf("%i ->", curr-> value);]
    curr = curr-> next;
  }
  printf("NULL \n");
}

char * listToString(list_t *l) {
  char* buf = (char *) malloc(sizeof(char) * 1024);
  char tbuf[20];

	node_t* curr = l->head;
  while (curr != NULL) {
    sprintf(tbuf, "%d->", curr->value);
    curr = curr->next;
    strcat(buf, tbuf);
  }
  strcat(buf, "NULL");
  return buf;
}

int list_length(list_t *l) { 
  int count = 0; 
  node_t* curr = l -> head;
  while(curr != NULL){
      count++;
      curr = curr -> next;
  }
  return count; 
   }

void list_add_to_back(list_t *l, elem value) {
  node_t *new_node = (node_t *) malloc(sizeof(node_t));
  new_node->value = value;
  new_node->next = NULL;

  if (l->head == NULL){
    l->head = new_node;
  }
  else{
    node_t *curr = l->head;
    while(curr->next != NULL){
      curr = curr->next;
    }
    curr->next = new_node;
  }
}

void list_add_to_front(list_t *l, elem value) {
     node_t* new_node = (node_t *) malloc(sizeof(node_t));
     new_node->value = value;
     new_node->next = NULL;

     /* Insert to front */

     new_node->next = l->head; // get head of list
     l-> head = new_node;
     
}

void list_add_at_index(list_t *l, elem value, int index) {
  node_t* new_node= (node_t*) malloc(sizeof(node_t));
  if (index==0){
    list_add_to_front(l,value);
    return;
  }
  new_node->value=value;
  node_t *curr = l->head;
  int i;
  for(i=0; i < index-1 && curr != NULL; i++ ){
    curr = curr->next;
  }
  if (curr == NULL){
    free(new_node);
    return;
  }
  new_node->next= curr->next;
  curr->next=new_node;
  }


elem list_remove_from_back(list_t *l) { 
  if (l->head==NULL)return -1;
  node_t *curr = l->head;
  node_t *prev = NULL;
  while( curr-> next != NULL){
    prev = curr;
    curr = curr->next;
  }
  if (prev==NULL){
    l->head = NULL;
  }else{
    prev->next = NULL
  }
  elem value = curr->value;
  free(curr)
  return value;
 }

elem list_remove_from_front(list_t *l) { 
  if (l->head==NULL) return -1;
  node_t *remove = l->head;
  elem value = remove->value;
  l->head = remove->next;
  free(remove);
  return value;
 }

elem list_remove_at_index(list_t *l, int index) { 
  if (index==0){
    return list_remove_from_front(l);
  }
  node_t *curr = l->head;
  node_t *prev = NULL;
  int i;
  for (i=0; i< index && curr != NULL; i++){
    prev = curr;
    curr = curr->next;
  }
  if (curr==NULL)return -1;
  prev->next = curr->next;
  elem value = curr->value;
  free(curr)
  return value;
 }

bool list_is_in(list_t *l, elem value) { 
  node_t* curr = l->head;
  while(curr != NULL){
    if( curr->value==value) return true;
    curr = curr->next;
  }
  return false;
 }

elem list_get_elem_at(list_t *l, int index) {
  node_t* curr =l->head;
  int i;
  for (i = 0; i < index && curr != NULL; i++){
          curr = current->next;
  }
  if (curr == NULL) return -1; 
  return curr->value;
  
}

int list_get_index_of(list_t *l, elem value) {
  node_t* curr =l->head;
  int count = 0;
  while(curr != NULL){
    if (curr->value == value)
      return count;
    curr = curr->next
    count++
  }
  return false;
}

