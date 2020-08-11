#include "symbol.h"
#include <stdio.h>
#include <stdlib.h> // for malloc

static Node *head = NULL;

void symbolInsert(const char *name, int value, SymbolAttributes attr)
{
	Node *temp = (Node *)malloc(sizeof(Node));
	if (!temp) {
		printf("the memory is full");
		exit(0);

	}
	strcpy(temp->name, name);
	temp->value = value;
	temp->attr = attr;
	temp->next = NULL;
	if (!head)
		head = temp;
	else {
		temp->next = head;
		head = temp;
		/// Node *current = head;
		//while (current->next)
			//current = current->next;
		//temp = current->next;
	}


}

void symbolUpdate(const char *name, int value, SymbolAttributes attr)
{
}

void symbolLookup(const char *name, int *value, SymbolAttributes *attr)
{
}

void printSymbolTable()
{
	Node *node = head;
	printf("( ");
	while (node) {
		printf("('%s' %d %d) ", node->name, node->value, node->attr);
		node = node->next;
	}
	printf(")\n");
}
