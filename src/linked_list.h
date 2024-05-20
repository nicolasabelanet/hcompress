// Nicolas Abelanet
// linked_list.h
// 10/17/2020

#pragma once
#include "hcompress.h"

// A Single Node in the Linked List
typedef struct Node {
  TreeNode *data;
  struct Node *next;
} LinkedList;

LinkedList *ll_new();

int ll_is_empty(LinkedList *ll /* First Node of a Linked List */);

void ll_display(LinkedList *ll /* First Node of a Linked List */);

void ll_add(LinkedList **ll /* First Node of a Linked List */,
            TreeNode *new_node /* A Huffman Tree Node */);

void ll_free(LinkedList *ll /* First Node of a Linked List */);

void list_add_in_order(LinkedList **ll /* First Node of a Linked List */,
                       TreeNode *tn /* A Huffman Tree Node */);
