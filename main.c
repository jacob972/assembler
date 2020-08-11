#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "gettoken.h"
#include "symbol.h"

void printTokenList(const char line[])
{
	Token token;
	int tokenLength;
	printf("( ");
	for (const char *p = line; 0 < (tokenLength = gettoken(p, &token)); p += tokenLength) {
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
			int linenum = 0;
			char line[1000];

			while (fgets(line, 1000, pFile)) {

				Token token;
				const char *p = line;
				p += gettoken(p, &token);

				if (token.type == LABELDEF) {
					Token token2;
					gettoken(p, &token2);
					if (token2.type == INSTRUCTION) {
						// TODO Fill in label address
						symbolInsert(token.string, 0, ATTR_CODE);
					}
					else if (token2.type == DIRECTIVE &&
						(0 == strcmp("data", token2.string) ||
							0 == strcmp("string", token2.string))) {
						// TODO Fill in label address
						symbolInsert(token.string, 0, ATTR_DATA);
					}
				}

				printf("%03d: ", ++linenum);

				if (line[0] == ';') {
					printf("%s", line);
					continue;
				}

				printTokenList(line);
			}
			printf("------ Symbols ------\n");
			printSymbolTable();
			fclose(pFile);
		}
	}
}


