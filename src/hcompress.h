// Nicolas Abelanet
// 10/18/2020
// hcompress.h

#pragma once
#include <stdio.h>

// State information aout reading characters.
// Used by the decoding process.
typedef struct ReadState {
  FILE *read;
  unsigned char buffer;
  int count;
  struct TreeNode *tree;
} ReadState;

// A single node in the Tree.
typedef struct TreeNode {
  int freq;
  int c;
  struct TreeNode *left;
  struct TreeNode *right;
  struct TreeNode *parent;
  int is_dynamic_alloc;
} TreeNode;

// State information about writing characters.
// Used by the encoding process.
typedef struct WriteState {
  FILE *write;
  unsigned char buffer;
  int count;
  TreeNode *table;
} WriteState;

TreeNode *
create_freq_table(char *filename /* Name of File Used To Create Huffman Table */);

void print_table(TreeNode *table /* Huffman Table */);

TreeNode *create_huffman_tree(TreeNode *table /* Huffman Table */);

void encode_file(char *filename /* Name of File That Will Be Encoded */,
                TreeNode *table /* Huffman Table */);

void decode_file(char *filename /* Name of File That Will Be Decoded */,
                TreeNode *tree /* Huffan Tree */);

void encode_and_write_byte(WriteState *ws /* State of Writing */,
                        char c /* Character To Be Encoded */);

void append_code(WriteState *ws /* State of Writing */,
                char *code /* Encoded Character Code */,
                int length /* Length of Encoded Character */);

void append_bit(WriteState *ws /* State of Writing */, int b /* 1 or 0 */);

void write_byte(WriteState *ws /* State of Writing */);

void read_byte(ReadState *rs /* State of Reading */);

int read_bit(ReadState *rs /* State of Reading */);

char decode_char(ReadState *rs /* State of Reading */,
                char *buffer /* Character Buffer */);

void print_byte(unsigned char byte /* Byte */);

void free_tree(TreeNode *tree /* Huffman Tree */);

void create_tree_node(TreeNode *TreeNode /* Huffman Tree Node */,
                      int is_dynamic_alloc /* Is Dynamically Allocated */);
