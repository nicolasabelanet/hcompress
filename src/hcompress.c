// Nicolas Abelanet
// hcompress.c
// 10/17/20

#include "hcompress.h"
#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define VERBOSE_TABLE
// #define VERBOSE_TREE
// #define VERBOSE_ENCODE
// #define VERBOSE_DECODE

int main(int argc, char *argv[]) {

  if (argc != 3) {
    printf("Error: The correct format is \"hcompress -e filename\" or "
           "\"hcompress -d filename.huf\"\n");
    fflush(stdout);
    exit(1);
  }

  TreeNode *table = create_freq_table("decind.txt");
  TreeNode *tree = create_huffman_tree(table);

  if (strcmp(argv[1], "-e") == 0) {
    encode_file(argv[2], table);
  } else {
    decode_file(argv[2], tree);
  }

  free_tree(tree);
  free(table);

  return 0;
}

void print_table(TreeNode *table) {
  for (int i = 0; i < 128; i++) {
    printf("ASCII Code: %d, Character: %c, Frequency %d\n", table[i].c,
           table[i].c, table[i].freq);
  }
}

// Reads in a filename and creates a frequency table with that file.
TreeNode *create_freq_table(char *filename) {

  // Creates a table with every ASCII Character from 0 - 127
  TreeNode *table = malloc(128 * sizeof(TreeNode));

  // Initializes each entry
  for (int i = 0; i < 128; i++) {
    create_tree_node(&table[i], 0);
    table[i].c = i;
  }

  FILE *file = fopen(filename, "r");

  char buffer;

  // Populate the table with the correct frequencies.
  while (fscanf(file, "%c", &buffer) != EOF) {
    table[(int)buffer].freq++;
  }

  fclose(file);

#ifdef VERBOSE_TABLE
  printTable(table);
#endif

  return table;
}

// Creates a tree from a given frequency table.
TreeNode *create_huffman_tree(TreeNode *table) {

  LinkedList *ll = ll_new();

  int count = 0;

  // Creates an ordered linked list
  for (int i = 0; i < 128; i++) {
    list_add_in_order(&ll, &table[i]);
    count++;
  }

#ifdef VERBOSE_TREE
  printf("Assembled Table:\n");
  llDisplay(ll);
#endif

  // Loops while there is more than one entry in the linked list.
  while (count > 1) {

    // Assigns the first and second elements.
    LinkedList *a = ll;
    LinkedList *b = a->next;

    // If the count is more than 2 assign the linked list the the
    // third entry. If not set it to NULL. This will be fixed
    // when the newNode is added in.
    if (count > 2)
      ll = b->next;
    else
      ll = NULL;

#ifdef VERBOSE_TREE
    printf("a: %d, b: %d\n", a->data->freq, b->data->freq);
#endif

    // Clears the next values for a and b.
    a->next = NULL;
    b->next = NULL;

#ifdef VERBOSE_TREE
    printf("Removed a and b:\n");
    llDisplay(ll);
#endif

    // Creates a new node to be placed into the linked list.
    TreeNode *newNode = malloc(1 * sizeof(TreeNode));
    create_tree_node(newNode, 1);

    // Sets the correct relationships between the newNode and a and b.
    newNode->freq = (a->data->freq) + (b->data->freq);
    newNode->left = a->data;
    newNode->right = b->data;

    a->data->parent = newNode;
    b->data->parent = newNode;

    list_add_in_order(&ll, newNode);

    count--;

#ifdef VERBOSE_TREE
    printf("Left: %d, Right: %d\n", newNode->left->freq, newNode->right->freq);

    printf("\nNew Linked List...\n");
    llDisplay(ll);
#endif

    ll_free(a);
    ll_free(b);
  }

#ifdef VERBOSE_TREE
  printf("\nFinal List...\n");
  llDisplay(ll);
#endif

  struct TreeNode *root = ll->data;

  ll_free(ll);

  // The root node.
  return root;
}

// Takes a given table and encodes a file.
void encode_file(char *fileName, TreeNode *table) {

  // Appends .huf to the file name.
  char newName[strlen(fileName) + 4];
  strcpy(newName, fileName);
  strcat(newName, ".huf");

  FILE *read = fopen(fileName, "r");

  if (read == NULL) {
    printf("Error Encoding: Could not find file \"%s\"\n", fileName);
    exit(0);
  }

  printf("Encoding \"%s\"\n", fileName);

  struct WriteState ws;
  ws.write = fopen(newName, "wb");
  ws.buffer = 0;
  ws.count = 0;
  ws.table = table;

  char buffer;

  // Scans in a new character, encodes, and writes the encoded character.
  while (fscanf(read, "%c", &buffer) != EOF) {
    encode_and_write_byte(&ws, buffer);
  }

  // Adds the NULL character to the end of the file.
  encode_and_write_byte(&ws, 0);

  // Makes sure there is a full byte at the end
  // of the file.
  if (ws.count > 0)
    write_byte(&ws);

  printf("Encoded \"%s\" as \"%s\"\n", fileName, newName);

  fclose(read);
  fclose(ws.write);
}

// Takes a given tree and decodes a file.
void decode_file(char *filename, struct TreeNode *tree) {

  // Appends .dec to the filename.
  char newName[strlen(filename) + 4];
  strcpy(newName, filename);
  strcat(newName, ".dec");

  FILE *write = fopen(newName, "w");

  struct ReadState rs;
  rs.read = fopen(filename, "rb");
  rs.buffer = 0;
  rs.count = -1;
  rs.tree = tree;

  if (rs.read == NULL) {
    printf("Error Decoding: Could not find file \"%s\"\n", filename);
    exit(1);
  }

  printf("Decoding \"%s\"\n", filename);

  char buffer;

  // Decodes an encoded character and writes it the file.
  while (decode_char(&rs, &buffer) != 0) {
    fprintf(write, "%c", buffer);
  }

  printf("Decoded \"%s\" as \"%s\"\n", filename, newName);

  fclose(rs.read);
  fclose(write);
}

// Encodes a character and writes a byte if necessary.
void encode_and_write_byte(WriteState *ws, char c) {

  // Sets the curr equal to the corresponding tnode in the table.
  TreeNode *curr = &(ws->table[(int)c]);
  TreeNode *child = NULL;

  char buffer[100];
  int bLength = 0;

#ifdef VERBOSE_ENCODE
  printf("Encode: %c\n", curr->c);
#endif

  // Reads from a given leaf until it reaches the root.
  // buffer is the path from the leaf to the root.
  while (curr != NULL && curr->parent != NULL) {

    child = curr;
    curr = curr->parent;

    // Checks if the child is the left or right child of the parent.
    if (curr->left == child)
      buffer[bLength] = '0';
    else if (curr->right == child)
      buffer[bLength] = '1';

    bLength++;
  }

  char code[bLength];

  // Reverses the path to now be from the parent to the leaf.
  for (int i = 0; i < bLength; i++) {
    code[i] = buffer[bLength - 1 - i];
  }

#ifdef VERBOSE_ENCODE
  printf("Character Code: ");
  for (int i = 0; i < bLength; i++) {
    printf("%c", code[i]);
  }
  printf("\n");
#endif

  // Appends the character code to the file.
  append_code(ws, code, bLength);
}

// Appends a character code to the file bit by bit.
void append_code(WriteState *ws, char *code, int length) {
  for (int i = 0; i < length; i++) {
    if (code[i] == '1')
      append_bit(ws, 1);
    else
      append_bit(ws, 0);
  }
}

// Appends a bit to the byte buffer.
void append_bit(struct WriteState *ws, int b) {

  unsigned char mask = 1;

  // Appends 1 to the buffer.
  if (b == 1) {
    ws->buffer = (ws->buffer) << 1;
    ws->buffer = (ws->buffer) | mask;
  }

  // Appends 0 to the buffer.
  else {
    ws->buffer = (ws->buffer) << 1;
  }

#ifdef VERBOSE_ENCODE
  printf("Append Bit %d\n", b);
  printf("New Byte: ");
  printByte(ws->buffer);
#endif

  ws->count++;

  // The buffer has reached a byte. Write the byte and
  // reset the buffer and count.
  if (ws->count >= 8) {
    write_byte(ws);
    ws->count = 0;
    ws->buffer = 0;
  }
}

// Writes the byte from the buffer to the file.
void write_byte(WriteState *ws) {

  // Makes sure the byte written is a full byte.
  ws->buffer = ws->buffer << (8 - ws->count);

#ifdef VERBOSE_ENCODE
  printf("Writing Byte: ");
  printByte(ws->buffer);
#endif

  // Writes the byte to the file.
  fwrite(&(ws->buffer), 1, 1, ws->write);
}

// Reads in a byte from a file.
void read_byte(ReadState *rs) {
  fread(&(rs->buffer), 1, 1, rs->read);
  rs->count = 8;
}

// Returns the next bit from the buffer.
int read_bit(struct ReadState *rs) {

  // No bits left. Read in a new byte.
  if (rs->count <= 0) {
    read_byte(rs);

#ifdef VERBOSE_DECODE
    printf("Read Byte: ");
    printByte(rs->buffer);
#endif
  }

  // This is a 1 in the 8th bit.
  unsigned char mask = 128;

  // Determines if the left bit in the buffer is a 1.
  int bit = 0;
  if ((rs->buffer) & mask)
    bit = 1;

#ifdef VERBOSE_DECODE
  printf("Decode Buffer: ");
  printByte(rs->buffer);
#endif

  // Shifts the buffer left.
  rs->buffer = (rs->buffer) << 1;
  rs->count--;

#ifdef VERBOSE_DECODE
  printf("Read Bit: %d\n", bit);
#endif

  return bit;
}

// Returns the ASCII character from a encoded character.
// Works similiarly to fscanf.
char decode_char(ReadState *rs, char *buffer) {

  // The root of the tree.
  TreeNode *curr = rs->tree;

  // Follows the path down the tree until a leaf is found.
  while (curr->left != NULL && curr->right != NULL) {
    if (read_bit(rs) == 0)
      curr = curr->left;
    else
      curr = curr->right;
  }

  // Sets the buffer to the character found by following the
  // character code.
  *buffer = curr->c;

#ifdef VERBOSE_DECODE
  printf("Reached Leaf Node: %c \n", curr->c);
#endif

  // Returns the character.
  return curr->c;
}

// Prints a byte to the console.
void print_byte(unsigned char byte) {

  char s[9];

  for (int i = 0; i < 8; i++) {

    if (byte & 1)
      *(s + 8 - i - 1) = '1';
    else
      *(s + 8 - i - 1) = '0';

    byte = byte >> 1;
  }

  *(s + 8) = '\0';

  printf("%s\n", s);
}

// Frees all the dynmically allocated tnodes
void free_tree(TreeNode *tree) {

  if (tree->left != NULL) {
    free_tree(tree->left);
  }

  if (tree->right != NULL) {
    free_tree(tree->right);
  }

  if (tree->is_dynamic_alloc == 1) {
    free(tree);
  }
}

// Initializes all the variables for the tnode structure.
void create_tree_node(TreeNode *tnode, int is_dynamic_alloc) {
  tnode->freq = 0;
  tnode->c = 0;
  tnode->left = NULL;
  tnode->right = NULL;
  tnode->parent = NULL;
  tnode->is_dynamic_alloc = is_dynamic_alloc;
}
