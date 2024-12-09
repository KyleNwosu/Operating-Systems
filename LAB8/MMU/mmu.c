#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "list.h"
#include "util.h"

void allocate_memory(list_t *freelist, list_t *alloclist, int pid, int blocksize, int policy) {
    node_t *current = freelist->head;
    node_t *prev = NULL;
    node_t *selected_node = NULL;
    node_t *prev_selected = NULL;
    block_t *fragment = NULL;

    while (current != NULL) {
        block_t *blk = current->blk;
        int current_size = blk->end - blk->start + 1;

        if (current_size >= blocksize) {
            if (selected_node == NULL) {
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

    if (selected_blk->start + blocksize - 1 > original_end) {
        printf("Error: Insufficient memory\n");
        return;
    }

    if (prev_selected == NULL) {
        freelist->head = selected_node->next;
    } else {
        prev_selected->next = selected_node->next;
    }

    selected_blk->pid = pid;
    selected_blk->end = selected_blk->start + blocksize - 1;

    if (selected_blk->end < original_end) {
        fragment = malloc(sizeof(block_t));
        fragment->pid = 0;
        fragment->start = selected_blk->end + 1;
        fragment->end = original_end;

        add_block_to_freelist(freelist, fragment, policy);
    }

    list_add_ascending_by_address(alloclist, selected_blk);
    free(selected_node);
}
