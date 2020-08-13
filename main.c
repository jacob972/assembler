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
        if (token.type == INSTRUCTION) {
            if (icCount(token) == 0)
                ic++;
            else if (icCount(token) == 1) {
                Token token2; char *p1 = p;
                p1 += tokenLength;
                p1 += gettoken(p1, &token2);
                while (token2.type == COMMA)
                    p1 += gettoken(p1, &token2);
                if (token2.type == IMMEDIATE || token2.type == LABELREF|| token2.type == RELATIVE)
                    ic += 2;
                else
                    ic++;
            }
            else if (icCount(token) == 2) {
                ic ++; int i = 2;
                    Token token2; char *p1 = p;
                    p1 += tokenLength;
                    while (i) {
                    p1 += gettoken(p1, &token2);
                    while (token2.type == COMMA)
                        p1 += gettoken(p1, &token2);
                    if (token2.type == IMMEDIATE || token2.type == LABELREF|| token2.type == RELATIVE)
                        ic++;
                    i--;
                }

            }
        }
                   
        if (token.type == DIRECTIVE) {
            if (0 == strcmp("string", token.string) || 0 == strcmp("data", token.string)) {
                Token token2; char *p1 = p;
                p1 += tokenLength;
                p1 += gettoken(p1, &token2);
              
                while (token2.type == NUMBER) {
                    dc++;
                    p1 += gettoken(p1, &token2);
                    while (token2.type == COMMA)
                        p1 += gettoken(p1, &token2);
               }
            
                while (token2.type == STRING) {
                    int k;
                    for (k = 0; k < strlen(token2.string); k++) {
                        data[dc++] = token2.string[k];
                    }
                    data[dc++] = 0;
                    //dc += strlen(token2.string )+1;
                    p1 += gettoken(p1, &token2);
                    while (token2.type == COMMA)
                        p1 += gettoken(p1, &token2);
                }
            }
       }
    }
    printf(")\n");
}

void ProcessLinePass1(char line[])
{
    Token token;
    const char *p = line;
    p += gettoken(p, &token);
    if (token.type == DIRECTIVE) {
        Token token2;
        gettoken(p, &token2);
        if (0 == strcmp("entry", token.string)) {
            int value;
            SymbolAttributes attr;
            if (symbolLookup(token2.string, &value, &attr)) {
                symbolUpdate(token2.string, dc, attr | ATTR_ENTRY);
            }
            else {
                symbolInsert(token2.string, dc, ATTR_ENTRY);
            }
        }
        else if (0 == strcmp("extern", token.string))
            symbolInsert(token2.string, dc, ATTR_EXTERNAL);
    }
    else if (token.type == LABELDEF) {
        Token token2;
        gettoken(p, &token2);
        if (token2.type == INSTRUCTION) {
            // TODO Fill in label address
            symbolInsert(token.string, ic, ATTR_CODE);
        }
        else if (token2.type == DIRECTIVE && 0 == strcmp("data", token2.string) || 0 == strcmp("string", token2.string)) {
            // TODO Fill in label address
            symbolInsert(token.string, dc, ATTR_DATA);
        }
    }
}

void ProcessLinePass2(char line[]) {

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
            
            for (int pass = 0; pass < 2; pass++)
            {
                fseek(pFile, 0, SEEK_SET);

                while (fgets(line, 1000, pFile)) {

                    if (pass == 0) {
                        ProcessLinePass1(line);
                    }
                    else {
                        ProcessLinePass2(line);
                    }

                    printf("%03d: [IC: %03d, DC: %03d] ", ++linenum, ic, dc);

                    if (line[0] == ';') {
                        printf("%s", line);
                        continue;
                    }

                    printTokenList(line);
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


