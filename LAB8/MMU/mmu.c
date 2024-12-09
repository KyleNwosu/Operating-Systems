#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "list.h"
#include "util.h"


typedef struct block {
    int pid;
    int start;
    int end;
    struct block *next;
} block_t;

typedef struct node{
    block_t *blk;
    struct node *next;
} node_t;

typedef struct list{
    node_t  *head;
} list_t;


void TOUPPER(char * arr){
  
    for(int i=0;i<strlen(arr);i++){
        arr[i] = toupper(arr[i]);
    }
}

void get_input(char *args[], int input[][2], int *n, int *size, int *policy) 
{
  	FILE *input_file = fopen(args[1], "r");
	  if (!input_file) {
		    fprintf(stderr, "Error: Invalid filepath\n");
		    fflush(stdout);
		    exit(0);
	  }

    parse_file(input_file, input, n, size);
  
    fclose(input_file);
  
    TOUPPER(args[2]);
  
    if((strcmp(args[2],"-F") == 0) || (strcmp(args[2],"-FIFO") == 0))
        *policy = 1;
    else if((strcmp(args[2],"-B") == 0) || (strcmp(args[2],"-BESTFIT") == 0))
        *policy = 2;
    else if((strcmp(args[2],"-W") == 0) || (strcmp(args[2],"-WORSTFIT") == 0))
        *policy = 3;
    else {
       printf("usage: ./mmu <input file> -{F | B | W }  \n(F=FIFO | B=BESTFIT | W-WORSTFIT)\n");
       exit(1);
    }
        
}

void add_block_to_freelist(list_t *freelist, block_t *block, int policy) {
    if (policy == 1) { // FIFO
        list_add_to_back(freelist, block);
    } else if (policy == 2) { // BESTFIT
        list_add_ascending_by_blocksize(freelist, block);
    } else if (policy == 3) { // WORSTFIT
        list_add_descending_by_blocksize(freelist, block);
    }
}

void allocate_memory(list_t *freelist, list_t *alloclist, int pid, int blocksize, int policy) {
    node_t *current = freelist->head;
    node_t *prev = NULL;
    node_t *selected_node = NULL;
    node_t *prev_selected = NULL;
    block_t *fragment = NULL;

    // Search for a suitable block based on policy
    while (current != NULL) {
        block_t *blk = current->blk;
        int current_size = blk->end - blk->start + 1;

        if (current_size >= blocksize) {
            if (selected_node == NULL) { // First suitable block
                selected_node = current;
                prev_selected = prev;
            } else if (policy == 2) { // BESTFIT
                int selected_size = selected_node->blk->end - selected_node->blk->start + 1;
                if (current_size < selected_size) {
                    selected_node = current;
                    prev_selected = prev;
                }
            } else if (policy == 3) { // WORSTFIT
                int selected_size = selected_node->blk->end - selected_node->blk->start + 1;
                if (current_size > selected_size) {
                    selected_node = current;
                    prev_selected = prev;
                }
            }
        }

        prev = current;
        current = current->next;
    }

    if (selected_node == NULL) {
        printf("Error: Memory Allocation %d blocks\n", blocksize);
        return;
    }

    block_t *selected_blk = selected_node->blk;
    int original_end = selected_blk->end;

    // Remove selected block from freelist
    if (prev_selected == NULL) { // Selected is head
        freelist->head = selected_node->next;
    } else {
        prev_selected->next = selected_node->next;
    }

    // Allocate memory from the selected block
    selected_blk->pid = pid;
    selected_blk->end = selected_blk->start + blocksize - 1;

    // Handle leftover fragment
    if (selected_blk->end < original_end) {
        fragment = malloc(sizeof(block_t));
        fragment->pid = 0;
        fragment->start = selected_blk->end + 1;
        fragment->end = original_end;

        // Add fragment back to freelist based on policy
        add_block_to_freelist(freelist, fragment, policy);
    }

    // Add allocated block to alloclist in ascending order by address
    list_add_ascending_by_address(alloclist, selected_blk);

    // Free the selected node (only the node, not the block)
    free(selected_node);
}


void deallocate_memory(list_t *alloclist, list_t *freelist, int pid, int policy) {
    node_t *prev = NULL;
    node_t *current = alloclist->head;

    // Search for the block to deallocate
    while (current != NULL) {
        block_t *blk = current->blk;
        if (blk->pid == pid) {
            // Remove the node from alloclist
            if (prev == NULL) { // Head node
                alloclist->head = current->next;
            } else {
                prev->next = current->next;
            }

            // Reset block PID and add it back to freelist
            blk->pid = 0;
            add_block_to_freelist(freelist, blk, policy);

            // Free the node (not the block, as it's reused)
            free(current);
            return;
        }

        prev = current;
        current = current->next;
    }

    // Block with the given PID not found
    printf("Error: Can't locate Memory Used by PID: %d\n", pid);
}


list_t* coalese_memory(list_t * list){
  list_t *temp_list = list_alloc();
  block_t *blk;
  
  while((blk = list_remove_from_front(list)) != NULL) {  // sort the list in ascending order by address
        list_add_ascending_by_address(temp_list, blk);
  }
  
  // try to combine physically adjacent blocks
  
  list_coalese_nodes(temp_list);
        
  return temp_list;
}

void print_list(list_t * list, char * message){
    node_t *current = list->head;
    block_t *blk;
    int i = 0;
  
    printf("%s:\n", message);
  
    while(current != NULL){
        blk = current;
        printf("Block %d:\t START: %d\t END: %d", i, blk->start, blk->end);
      
        if(blk->pid != 0)
            printf("\t PID: %d\n", blk->pid);
        else  
            printf("\n");
      
        current = current->next;
        i += 1;
    }
}

/* DO NOT MODIFY */
int main(int argc, char *argv[]) 
{
   int PARTITION_SIZE, inputdata[200][2], N = 0, Memory_Mgt_Policy;
  
   list_t *FREE_LIST = list_alloc();   // list that holds all free blocks (PID is always zero)
   list_t *ALLOC_LIST = list_alloc();  // list that holds all allocated blocks
   int i;
  
   if(argc != 3) {
       printf("usage: ./mmu <input file> -{F | B | W }  \n(F=FIFO | B=BESTFIT | W-WORSTFIT)\n");
       exit(1);
   }
  
   get_input(argv, inputdata, &N, &PARTITION_SIZE, &Memory_Mgt_Policy);
  
   // Allocated the initial partition of size PARTITION_SIZE
   
   block_t * partition = malloc(sizeof(block_t));   // create the partition meta data
   partition->start = 0;
   partition->end = PARTITION_SIZE + partition->start - 1;
                                   
   list_add_to_front(FREE_LIST, partition);          // add partition to free list
                                   
   for(i = 0; i < N; i++) // loop through all the input data and simulate a memory management policy
   {
       printf("************************\n");
       if(inputdata[i][0] != -99999 && inputdata[i][0] > 0) {
             printf("ALLOCATE: %d FROM PID: %d\n", inputdata[i][1], inputdata[i][0]);
             allocate_memory(FREE_LIST, ALLOC_LIST, inputdata[i][0], inputdata[i][1], Memory_Mgt_Policy);
       }
       else if (inputdata[i][0] != -99999 && inputdata[i][0] < 0) {
             printf("DEALLOCATE MEM: PID %d\n", abs(inputdata[i][0]));
             deallocate_memory(ALLOC_LIST, FREE_LIST, abs(inputdata[i][0]), Memory_Mgt_Policy);
       }
       else {
             printf("COALESCE/COMPACT\n");
             FREE_LIST = coalese_memory(FREE_LIST);
       }   
     
       printf("************************\n");
       print_list(FREE_LIST, "Free Memory");
       print_list(ALLOC_LIST,"\nAllocated Memory");
       printf("\n\n");
   }
  
   list_free(FREE_LIST);
   list_free(ALLOC_LIST);
  
   return 0;
}
