#include "symbol.h"
#include <stdio.h>
#include <stdlib.h> // for malloc

void symbolInsert(const char *name, int value, SymbolAttributes attr)
{
	Node *symbol = (Node *)malloc(sizeof(Node));
	if (!symbol) {
		printf("the memory is full");
		exit(0);

	}
	strcpy(symbol->name, name);
	symbol->value = value;
	symbol->attr = attr;
	symbol->link = NULL;
	if (!head)
		head = symbol;
	else {
		symbol->link = head;
		head = symbol;
	}


}

void symbolUpdate(const char *name, int value, SymbolAttributes attr)
{
	Node *symbol = head;
	while (symbol) {
		if (0 == strcmp(symbol->name, name)) {
			symbol->value = value;
			symbol->attr = attr;
		}
		symbol = symbol->link;

	}
}

int symbolLookup(const char *name, int *value, SymbolAttributes *attr)
{
	Node *symbol = head;
	while (symbol) {
		//strcat(name, ":")
		//if (0 == strcmp(symbol->name, strcat(name, ":"))) {
			
		//}
		
		if ( 0 == strcmp(symbol->name, name)) {
			*value = symbol->value;
			*attr = symbol->attr;
			return 1;
		}
		symbol = symbol->link;
	}
	return 0;
}


void printSymbolTable()
{
	Node *symbol = head;
	printf("( ");
	while (symbol) {
		printf("(%s %d %d) ", symbol->name, symbol->value, symbol->attr);
		symbol = symbol->link;
	}
	printf(")\n");
}
