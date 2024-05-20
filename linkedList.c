// Nicolas Abelanet
// linkedList.c
// 10/16/20

#include <stdio.h>
#include <stdlib.h> // malloc
#include "hcompress.h"
#include "linkedList.h"

LinkedList* llCreate() {
  return NULL;
}

int llIsEmpty(LinkedList* ll) {
  return (ll == NULL);
}

void llDisplay(LinkedList* ll) {
  
  LinkedList* p = ll;

  printf("[");
  
  while (p != NULL) {
    printf("%d, ", p -> data -> freq);
    p = p->next;
  }

  printf("]\n");
}

void llAdd(LinkedList** ll, struct tnode* newData) {

  // Create the new node
  LinkedList* newNode = (LinkedList*)malloc(1 * sizeof(LinkedList));
  newNode->data = newData;
  newNode->next = NULL;
  
  // Find the end of the list
  LinkedList* p = *ll;

  if (p == NULL) {  // Add first element
    *ll = newNode;  // This is why we need ll to be a **

  } else {

    while (p->next != NULL) {
      p = p->next;
    }
    
    // Attach it to the end
    p->next = newNode;
  }

}

void llFree(LinkedList* ll) {
  LinkedList* p = ll;

  while (p != NULL) {
    LinkedList* oldP = p;
    p = p->next;
    free(oldP);
  }
  
}

// Adds a node into a linked list in order.
void list_add_in_order(LinkedList** ll, struct tnode* tn) {

  LinkedList* newNode = malloc(1 * sizeof(LinkedList));
  newNode -> data = tn;
  newNode -> next = NULL;
  
  LinkedList* curr = *ll; 

  // There are zero entries.
  if (curr == NULL) *ll = newNode;

  // The first entry is larger than the number you are putting in.
  else if (curr -> data -> freq >= tn -> freq) {
    *ll = newNode;
    newNode -> next = curr;
  }

  // The upper and lower bound are in the remainder of the list.
  else {

    // Loops until the next node is larger than the newNode. 
    while( (curr -> next != NULL) && (curr -> next -> data -> freq < tn -> freq) ) {
      curr = curr -> next;
    }

    LinkedList* lower = curr;
    LinkedList* upper = curr -> next;

    lower -> next = newNode;
    newNode -> next = upper;
    
  }
  
}
