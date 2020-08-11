#pragma once

#include <stdlib.h>

#define MAX_NAME_LEN 31

typedef enum {
	ATTR_CODE = 1,
	ATTR_DATA = 2,
	ATTR_ENTRY = 4,
	ATTR_EXTERNAL = 8
} SymbolAttributes;

struct node {
	char name[MAX_NAME_LEN + 1];
	int value;
	SymbolAttributes attr;
	struct node *next;
};
typedef struct node Node;

void symbolInsert(const char *name, int value, SymbolAttributes attr);
void symbolUpdate(const char *name, int value, SymbolAttributes attr);
void symbolLookup(const char *name, int *value, SymbolAttributes *attr);
void printSymbolTable();

