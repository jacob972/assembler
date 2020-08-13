#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "gettoken.h"
#include "symbol.h"
int IC = 100, DC = 0;

void printTokenList( char line[])
{
    Token token;
    int tokenLength;
    printf("( ");
    for ( char *p = line; 0 < (tokenLength = gettoken(p, &token)); p += tokenLength) {
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
                IC++;
            else if (icCount(token) == 1) {
                Token token2; char *p1 = p;
                p1 += tokenLength;
                p1 += gettoken(p1, &token2);
                while (token2.type == COMMA)
                    p1 += gettoken(p1, &token2);
                if (token2.type == IMMEDIATE || token2.type == LABELREF|| token2.type == RELATIVE)
                    IC += 2;
                else
                    IC++;
            }
            else if (icCount(token) == 2) {
                IC ++; int i = 2;
                    Token token2; char *p1 = p;
                    p1 += tokenLength;
                    while (i) {
                    p1 += gettoken(p1, &token2);
                    while (token2.type == COMMA)
                        p1 += gettoken(p1, &token2);
                    if (token2.type == IMMEDIATE || token2.type == LABELREF|| token2.type == RELATIVE)
                        IC++;
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
                    DC++;
                    p1 += gettoken(p1, &token2);
                    while (token2.type == COMMA)
                        p1 += gettoken(p1, &token2);
               }
            
                while (token2.type == STRING) {
                    DC += strlen(token2.string )+1;
                    p1 += gettoken(p1, &token2);
                    while (token2.type == COMMA)
                        p1 += gettoken(p1, &token2);
                }
            }
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
                //if (token.type == INSTRUCTION) {
                   // IC++;
                  //  Token token2, token3;
                   // gettoken(p, &token2);
                 //   gettoken(p, &token3);
                 //   if (token2.type== LABELREF|| token2.type == IMMEDIATE)
                    //    IC++;
                    //if (token3.type == LABELREF || token3.type == IMMEDIATE)
                  //      IC++;
                //}
                if (token.type == DIRECTIVE) {
                   Token token2;
                   gettoken(p, &token2);
                   if (0 == strcmp("entry", token.string))
                       symbolInsert(token2.string, 0, ATTR_ENTRY);
                   
                   else if(0 == strcmp("extern", token.string))
                       symbolInsert(token2.string, 0, ATTR_EXTERNAL);

                }
                
                

                else if (token.type == LABELDEF) {
                    Token token2;
                    gettoken(p, &token2);
                    if (token2.type == INSTRUCTION) {
                        // TODO Fill in label address
                        symbolInsert(token.string, 0, ATTR_CODE);
                    }
                    else if (token2.type == DIRECTIVE && 0 == strcmp("data", token2.string)|| 0 == strcmp("string", token2.string)) {
                        // TODO Fill in label address
                        symbolInsert(token.string, 0, ATTR_DATA);
                    }

                }

                printf("%03d: [IC: %03d, DC: %03d] ", ++linenum, IC, DC);

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


