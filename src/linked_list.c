// Nicolas Abelanet
// linked_list.c
// 10/16/20

#include "linked_list.h"
#include "hcompress.h"
#include <stdio.h>
#include <stdlib.h>

LinkedList *ll_new() { return NULL; }

int ll_is_empty(LinkedList *ll) { return (ll == NULL); }

void ll_display(LinkedList *ll) {

  LinkedList *p = ll;

  printf("[");

  while (p != NULL) {
    printf("%d, ", p->data->freq);
    p = p->next;
  }

  printf("]\n");
}

void ll_add(LinkedList **ll, TreeNode *new_data) {

  // Create the new node
  LinkedList *new_node = (LinkedList *)malloc(1 * sizeof(LinkedList));
  new_node->data = new_data;
  new_node->next = NULL;

  // Find the end of the list
  LinkedList *p = *ll;

  if (p == NULL) { // Add first element
    *ll = new_node; // This is why we need ll to be a **

  } else {

    while (p->next != NULL) {
      p = p->next;
    }

    // Attach it to the end
    p->next = new_node;
  }
}

void ll_free(LinkedList *ll) {
  LinkedList *p = ll;

  while (p != NULL) {
    LinkedList *oldP = p;
    p = p->next;
    free(oldP);
  }
}

// Adds a node into a linked list in order.
void list_add_in_order(LinkedList **ll, struct TreeNode *tn) {

  LinkedList *newNode = malloc(1 * sizeof(LinkedList));
  newNode->data = tn;
  newNode->next = NULL;

  LinkedList *curr = *ll;

  // There are zero entries.
  if (curr == NULL)
    *ll = newNode;

  // The first entry is larger than the number you are putting in.
  else if (curr->data->freq >= tn->freq) {
    *ll = newNode;
    newNode->next = curr;
  }

  // The upper and lower bound are in the remainder of the list.
  else {

    // Loops until the next node is larger than the newNode.
    while ((curr->next != NULL) && (curr->next->data->freq < tn->freq)) {
      curr = curr->next;
    }

    LinkedList *lower = curr;
    LinkedList *upper = curr->next;

    lower->next = newNode;
    newNode->next = upper;
  }
}
