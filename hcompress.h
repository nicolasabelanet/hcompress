// Nicolas Abelanet
// 10/18/2020
// hcompress.h

#pragma once

// State information about writing characters.
// Used by the encoding process.
struct writeState {
  FILE* write;
  unsigned char buffer;
  int count;
  struct tnode* table;
};

// State information aout reading characters.
// Used by the decoding process.
struct readState {
  FILE* read;
  unsigned char buffer;
  int count;
  struct tnode* tree;
};

// A single node in the Tree. 
struct tnode {
  int freq;
  int c;
  struct tnode* left;
  struct tnode* right;
  struct tnode* parent;
  int isDynamicAlloc;
};

struct tnode* createFreqTable(
			      char* fileName /* Name of File Used To Create Huffman Table */);

void printTable(
		struct tnode* table /* Huffman Table */);

struct tnode* createHuffmanTree(
				struct tnode* table /* Huffman Table */);

void encodeFile(
		char* fileName /* Name of File That Will Be Encoded */,
		struct tnode* table /* Huffman Table */);

void decodeFile(
		char* fileName /* Name of File That Will Be Decoded */,
		struct tnode* tree /* Huffan Tree */);

void encodeAndWriteByte(
			struct writeState* ws /* State of Writing */,
			char c /* Character To Be Encoded */);

void appendCode(
		struct writeState* ws /* State of Writing */,
		char* code /* Encoded Character Code */,
		int length /* Length of Encoded Character */);

void appendBit(
	       struct writeState* ws /* State of Writing */,
	       int b /* 1 or 0 */);

void writeByte(
	       struct writeState* ws /* State of Writing */);

void readByte(
	      struct readState* rs /* State of Reading */);

int readBit(
	    struct readState* rs /* State of Reading */);

char decodeChar(struct readState* rs /* State of Reading */,
		char* buffer /* Character Buffer */);

void printByte(
	       unsigned char byte /* Byte */);

void freeTree(
	      struct tnode* tree /* Huffman Tree */ );

void tnodeConstructor(
		      struct tnode* tnode /* Huffman Tree Node */,
		      int isDynamicAlloc /* Is Dynamically Allocated */);
