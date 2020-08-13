#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "gettoken.h"
#include "symbol.h"

int ic = 0, dc = 0, icf, dcf;
int code[1000];
int data[1000];

void printTokenList(char line[])
{
	Token token;
	int tokenLength;
	printf("( ");
	for (char *p = line; 0 < (tokenLength = gettoken(p, &token)); p += tokenLength) {
		switch (token.type) {
		case DIRECTIVE:
			printf("(%s '%s') ", token._typename, token.string);
			break;
		case LABELDEF:
			printf("(%s '%s') ", token._typename, token.string);
			break;
		case LABELREF:
			printf("(%s '%s') ", token._typename, token.string);
			break;
		case INSTRUCTION:
			printf("(%s '%s') ", token._typename, token.string);
			break;
		case IMMEDIATE:
			printf("(%s %d) ", token._typename, token.number);
			break;
		case REGISTER:
			printf("(%s %d) ", token._typename, token.number);
			break;
		case NUMBER:
			printf("(%s %d) ", token._typename, token.number);
			break;
		case STRING:
			printf("(%s \"%s\") ", token._typename, token.string);
			break;
		case COMMA:
			printf("(%s) ", token._typename);
			break;
		case UNIDENTIFIED:
		default:
			printf("(%s '%s') ", token._typename, token.string);
			break;
		}
	}
	printf(")\n");
}

void ProcessLine(char line[], int pass)
{
	char *p = line;
	Token token;

	p += gettoken(p, &token);

	if (token.type == LABELDEF) {
		p += gettoken(p, &token);
		if (token.type == INSTRUCTION) {
			symbolInsert(token.string, ic, ATTR_CODE);
		}
		else if (token.type == DIRECTIVE && 0 == strcmp("data", token.string) || 0 == strcmp("string", token.string)) {
			symbolInsert(token.string, dc, ATTR_DATA);
		}
	}

	// token is either INSTRUCTION or DIRECTIVE.

	if (token.type == DIRECTIVE) {
		if (0 == strcmp("string", token.string)) {
			// TODO: Handle .string directive.
		}
		else if (0 == strcmp("data", token.string)) {
			// TODO: Handle .data directive.
		}
		else if (0 == strcmp("entry", token.string)) {
			// TODO: Handle .entry directive.
		}
		else if (0 == strcmp("extern", token.string)) {
			// TODO: Handle .extern directive.
		}
		else {
			// TODO: Error: unknown directive.
		}
			if (0 == strcmp("string", token.string) || 0 == strcmp("data", token.string)) {
			Token token2;
			p += gettoken(p, &token2);;
			p += gettoken(p, &token2);

			while (token2.type == NUMBER) {
				data[dc++] = token2.number;
				p += gettoken(p, &token2);

				while (token2.type == COMMA)
					p += gettoken(p, &token2);
			}

			while (token2.type == STRING) {
				int k;
				for (k = 0; k < strlen(token2.string); k++) {
					data[dc++] = token2.string[k];
				}
				data[dc++] = 0;
				//dc += strlen(token2.string )+1;
				p += gettoken(p, &token2);
				while (token2.type == COMMA)
					p += gettoken(p, &token2);
			}
		}
	}
	else if (token.type == INSTRUCTION) {
		if (icCount(token) == 0)
			ic++;
		else if (icCount(token) == 1) {
			Token token2;
			p += gettoken(p, &token2);
			p += gettoken(p, &token2);
			while (token2.type == COMMA)
				p += gettoken(p, &token2);
			if (token2.type == IMMEDIATE || token2.type == LABELREF || token2.type == RELATIVE)
				ic += 2;
			else
				ic++;
		}
		else if (icCount(token) == 2) {
			ic++; int i = 2;
			Token token2;
			p += gettoken(p, &token2);;
			while (i) {
				p += gettoken(p, &token2);
				while (token2.type == COMMA)
					p += gettoken(p, &token2);
				if (token2.type == IMMEDIATE || token2.type == LABELREF || token2.type == RELATIVE)
					ic++;
				i--;
			}
		}
	}
	//p += gettoken(p, &token);
	//if (token.type == DIRECTIVE) {
	//	Token token2;
	//	gettoken(p, &token2);
	//	if (0 == strcmp("entry", token.string)) {
	//		int value;
	//		SymbolAttributes attr;
	//		if (symbolLookup(token2.string, &value, &attr)) {
	//			symbolUpdate(token2.string, dc, attr | ATTR_ENTRY);
	//		}
	//		else {
	//			symbolInsert(token2.string, dc, ATTR_ENTRY);
	//		}
	//	}
	//	else if (0 == strcmp("extern", token.string))
	//		symbolInsert(token2.string, dc, ATTR_EXTERNAL);
	//}
}

int main(int argc, char *argv[]) {

	for (int i = 1; i < argc; i++) {
		FILE *pFile;
		char fileName[100];

		sprintf(fileName, "%s.as", argv[i]);
		pFile = fopen(fileName, "r");
		if (pFile == NULL) {
			exit(ENOENT);
		}
		else {
			for (int pass = 0; pass < 2; pass++)
			{
				int linenum = 0;
				char line[1000];

				ic = 100; dc = 0;

				fseek(pFile, 0, SEEK_SET);

				while (fgets(line, 1000, pFile)) {

					if (pass == 1) printf("%03d: [IC: %03d, DC: %03d] %s", ++linenum, ic, dc, line);

					if (line[0] == ';') {
						continue;
					}

					ProcessLine(line, pass);

					//printTokenList(line);
				}
			}

			icf = ic;
			dcf = dc;
			printf("------\n");
			printf("ICF: %d, DCF: %d\n", icf, dcf);

			printf("------ Symbols ------\n");
			printSymbolTable();

			fclose(pFile);
		}
	}
}


