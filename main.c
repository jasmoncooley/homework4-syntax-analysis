// THIS IS ALL TEXTBOOK CODE


/* front.c - a lexical analyzer system for simple
 arithmetic expressions */
#include <stdio.h>
#include <ctype.h>
/* Global declarations */
/* Variables */
int charClass;
char lexeme [100];
char nextChar;
int lexLen;
int token;
int nextToken;
FILE *in_fp, *fopen();

/* Function declarations */
void addChar();
void getChar();
void getNonBlank();
int lex();
/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99
/* Token codes */
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26

/******************************************************/
/* main driver */
main() {
/* Open the input data file and process its contents */
 if ((in_fp = fopen("front.in", "r")) == NULL)
 printf("ERROR - cannot open front.in \n");
 else {
 getChar();
 do {
 lex();
 } while (nextToken != EOF);
 }
}

/*****************************************************/
/* lookup - a function to lookup operators and parentheses
 and return the token */
int lookup(char ch) {
 switch (ch) {
 case '(':
 addChar();
 nextToken = LEFT_PAREN;
 break;
 case ')':
 addChar();
 nextToken = RIGHT_PAREN;
 break;
 case '+':
 addChar();
 nextToken = ADD_OP;
 break;
 case '-':
 addChar();
 nextToken = SUB_OP;
 break;
 case '*':
 addChar();
 nextToken = MULT_OP;
 break;
 case '/':
 addChar();
 nextToken = DIV_OP;
 break;