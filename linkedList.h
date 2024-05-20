// Nicolas Abelanet
// linkedList.h
// 10/17/2020

#pragma once
#include "hcompress.h"

// A Single Node in the Linked List
typedef struct node {
  struct tnode* data;
  struct node* next;
} LinkedList;


LinkedList* llCreate();

int llIsEmpty(
	      LinkedList* ll /* First Node of a Linked List */);

void llDisplay(
	       LinkedList* ll /* First Node of a Linked List */);

void llAdd(
	   LinkedList** ll /* First Node of a Linked List */,
	   struct tnode* newNode /* A Huffman Tree Node */);

void llFree(
	    LinkedList* ll /* First Node of a Linked List */);

void list_add_in_order(
		       LinkedList** ll /* First Node of a Linked List */,
		       struct tnode* tn /* A Huffman Tree Node */);

