#pragma once
#define MAX_NAME_LEN 31

typedef enum {
	ATTR_CODE = 1,
	ATTR_DATA = 2,
	ATTR_ENTRY = 4,
	ATTR_EXTERNAL = 8
} SymbolAttributes;

typedef struct Node Node;
 Node *head;
 struct Node {
	char name[MAX_NAME_LEN+1];
	int value;
	SymbolAttributes attr;
	 struct Node *link;
};

void symbolInsert(const char *name, int value, SymbolAttributes attr);
void symbolUpdate(const char *name, int value, SymbolAttributes attr);
int symbolLookup(const char *name, int *value, SymbolAttributes *attr);
void printSymbolTable();

