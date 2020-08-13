#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "gettoken.h"


#define settokentype(ptok, typ) ((ptok)->type = (typ), strcpy((ptok)->_typename, #typ))
char *instruct[] = { "mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "jsr", "red", "prn", "rts", "stop" };
int index, l;
char *oneOperand[] = { "clr", "not", "inc", "dec", "jmp", "bne", "jsr", "red", "prn" };
char *twoOperand[] = { "lea", "sub", "add", "cmp", "mov" };
char *zeroOperand[] = { "rts","stop" };
int icCount(Token *token) {
	int len = strlen(token->string);
	int index = 0, i = 0;
	while (index < 2) {

		if ((*token).string[i] == zeroOperand[index][i]) {
			i++;
			if (i == len)
				return 0;
		}

		else if ((*token).string[i] != zeroOperand[index][i]) {
			i = 0; index++;
		}

	}
	index = i = 0;
	while (index < 9) {
		if ((*token).string[i] == oneOperand[index][i]) {
			i++;
			if (i == len)
				return 1;
		}

		else if ((*token).string[i] != oneOperand[index][i]) {
			i = 0; index++;
		}
	}
	index = i = 0;
	while (index < 9) {
		if ((*token).string[i] == twoOperand[index][i]) {
			i++;
			if (i == len)
				return 2;
		}

		else if ((*token).string[i] != twoOperand[index][i]) {
			i = 0; index++;
		}
	}
	return -1;
}

int gettoken(const char *input, Token *token)
{
	int nchars;
	settokentype(token, UNIDENTIFIED);

	if (0 < sscanf(input, " #%d%n", &token->number, &nchars)) {
		settokentype(token, IMMEDIATE);
		return nchars;
	}
	else if (0 < sscanf(input, " r%d %n", &token->number, &nchars)) {
		settokentype(token, REGISTER);
		return nchars;
	}
	else if (0 < sscanf(input, "%d%n", &token->number, &nchars)) {
		settokentype(token, NUMBER);
		return nchars;
	}
	else if (0 < sscanf(input, " .%[A-Za-z0-9:]%n", token->string, &nchars)) {
		settokentype(token, DIRECTIVE);
		return nchars;
	}
	else if (0 < sscanf(input, " %[,;]%n", token->string, &nchars)) {
		settokentype(token, COMMA);
		return nchars;
	}
	else if (0 < sscanf(input, " &%[A-Za-z0-9:]%n", token->string, &nchars)) {
		settokentype(token, RELATIVE);
		return nchars;
	}
	else if (0 < sscanf(input, " \"%[^\"]\"%n", token->string, &nchars)) {
		settokentype(token, STRING);
		return nchars;
	}
	else if (0 < sscanf(input, " %[A-Za-z0-9:]%n", token->string, &nchars)) {
		index = l = 0;
		while (index < 16) {

			if ((*token).string[l] == instruct[index][l]) {
				l++;
				if (l == nchars) {
					settokentype(token, INSTRUCTION);
					return nchars;
				}
			}
			else if (input[l] != instruct[index][l]) {
				index++;
				l = 0;
			}
		}

		if (input[--nchars] == ':')
			settokentype(token, LABELDEF);
		else
			settokentype(token, LABELREF);
		return ++nchars;
	}
	else if (0 < sscanf(input, " %s%n", token->string, &nchars)) {
		settokentype(token, UNIDENTIFIED);
		return nchars;
	}
	else {
		return 0;
	}
}

