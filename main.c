#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "gettoken.h"
#include "symbol.h"

int ic = 0, dc = 0, icf, dcf;
unsigned int code[1000];
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

static struct {
	char name[5];
	int opcode;
	int funct;
} instructions[] = {
	//0 mov
	{.name = "mov", .opcode = 0 },
	{.name = "cmp", .opcode = 1},
	{.name = "add", .opcode = 2, .funct = 1},
	{.name = "sub", .opcode = 2, .funct = 2},
	{.name = "lea", .opcode = 4},
	{.name = "clr", .opcode = 5,.funct = 1},
	{.name = "not", .opcode = 5,.funct = 2},
	{.name = "inc", .opcode = 5,.funct = 3},
	{.name = "dec", .opcode = 5,.funct = 4},
	{.name = "jmp", .opcode = 9,.funct = 1},
	{.name = "bne", .opcode = 9,.funct = 2},
	{.name = "jsr", .opcode = 9,.funct = 3},
	{.name = "red", .opcode = 12},
	{.name = "prn", .opcode = 13},
	{.name = "rts", .opcode = 14},
	{.name = "stop", .opcode = 15},
};

unsigned int getOpcodeAndFunct(const char *instruction) {
	int i;
	for (i = 0; i < 16; i++) {
		if (0 == strcmp(instructions[i].name, instruction))
			return (instructions[i].opcode << 18) | (instructions[i].funct << 3) | 4;
	}
	return -1;
}

void processLine(char line[], int pass)
{
	char *p = line;
	Token token;

	p += gettoken(p, &token);

	if (token.type == LABELDEF) {
		Token token2;
		p += gettoken(p, &token2);
		if (token2.type == INSTRUCTION) {
			symbolInsert(token.string, ic, ATTR_CODE);
			token = token2;
		}
		else if (token2.type == DIRECTIVE && 0 == strcmp("data", token2.string) || 0 == strcmp("string", token2.string)) {
			symbolInsert(token.string, dc, ATTR_DATA);
			token = token2;
		}
	}

	if (token.type == DIRECTIVE) {
		if (0 == strcmp("string", token.string)) {
			Token token2;
			p += gettoken(p, &token2);
			int k;
			for (k = 0; k < strlen(token2.string); k++) {
				data[dc++] = token2.string[k];
			}
			data[dc++] = 0;
			p += gettoken(p, &token2);
			while (token2.type == COMMA)
				p += gettoken(p, &token2);

		}
		else if (0 == strcmp("data", token.string)) {
			Token token2;
			p += gettoken(p, &token2);
			while (token2.type == NUMBER) {
				data[dc++] = token2.number & 0xFFFFFF;
				p += gettoken(p, &token2);

				while (token2.type == COMMA)
					p += gettoken(p, &token2);
			}
		}
		else if (0 == strcmp("entry", token.string)) {
			// TODO: Handle .entry directive.
		}
		else if (0 == strcmp("extern", token.string)) {
			Token token2;
			p += gettoken(p, &token2);
			symbolInsert(token2.string, 1, ATTR_EXTERNAL);
			
		}
		else {
			// TODO: Error: unknown directive.
		}

	}
	else if (token.type == INSTRUCTION) {
		int i,nwords = 1;

		code[ic-100] = getOpcodeAndFunct(token.string);
		ic++;
		if (pass == 1) printf("%07d %08o\n", ic, code[ic - 100]);

		if (icCount(token) == 1) {
			Token token2;
			p += gettoken(p, &token2);
			if (token2.type == IMMEDIATE) {

				code[ic-100] = (token2.number << 3)|4;
				ic++;
			}  
			if (token2.type == LABELREF|| token2.type == RELATIVE) {
				int k = 1;
				SymbolAttributes b = 1;
				if (symbolLookup(token2.string, &k, &b)) {
					if (b == 1) {
						code[ic - 100] = k;
						ic++;
					}
					if (b == ATTR_DATA) {
						data[dc++] = k;
					}
				}
			
				
			}  
			
		}
		else if (icCount(token) == 2) {
			int i = 2;
			Token token2;
			while (i) {
				p += gettoken(p, &token2);
				while (token2.type == COMMA)
					p += gettoken(p, &token2);
				if (token2.type == IMMEDIATE) {
					code[ic - 100] = (token2.number << 3) | 4;
					ic++;
				}
				if (token2.type == LABELREF || token2.type == RELATIVE) {
					int k = 1;
					SymbolAttributes b = 1;
					if (symbolLookup(token2.string, &k, &b)) {
						//if (b == 1) {
							code[ic - 100] = k;
							ic++;
						//}
						//if (b == ATTR_DATA) {
							//code[ic - 100] = k;
							//ic++;
						//}
					}
				}

				i--;
			}

		}

		if (pass == 1) {
			for (i =0; i+100< ic-1; i++) {
				printf("%07d %08o\n", ic + i, code[i]);
			}
		}

	}

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

					processLine(line, pass);

					//printTokenList(line);
				}
			}

			icf = ic-1;
			dcf = dc;

			printf("------ Symbols ------\n");
			printSymbolTable();

			printf("------ Object Code ------\n");
			printf("%7d %d\n", icf, dcf);
			for (ic = 100; ic < icf; ic++) {
				printf("%07d %06x\n", ic, code[ic - 100]);
			}
			for (dc = 0; dc < dcf; dc++) {
				printf("%07d %06x\n", icf + dc, data[dc]);
			}

			fclose(pFile);
		}
	}
}


