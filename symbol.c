#include "symbol.h"
#include <stdio.h>
#include <stdlib.h> // for malloc

void symbolInsert(const char *name, int value, SymbolAttributes attr)
{
	  Node *temp= (Node*) malloc(sizeof(Node));
	if (!temp) {
		printf("the memory is full");
		exit(0);
		
	}
	strcpy(temp->name, name);
	temp->value = value;
	temp->attr = attr;
	temp->link=NULL;
	if (!head)
		head = temp;
	else {
		temp->link= head;
		head = temp;
		/// Node *current = head;
		//while (current->link)
			//current = current->link;
		//temp = current->link;
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
	Node *temp1=head;
	while (temp1) {
		printf("(%s %d %d)\t",temp1->name, temp1->value, temp1->attr);
		temp1 = temp1->link;

	}
}
