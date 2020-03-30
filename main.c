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
int nextToken = -2;
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


// function prototypes for just expr()
void expr();
void stmt();

// define the grammar
/*
<expr> → <term> {(+ | -) <term>}
<term> → <factor> {(* | /) <factor>}
<factor> → id | int_constant | ( <expr> )
*/
/******************************************************/
/* main driver */
int main() {
/* Open the input data file and process its contents */
  if ((in_fp = fopen("front.in", "r")) == NULL)
    // open the file
    printf("ERROR - cannot open front.in \n");
  else {
    // get characters and lex()?
    getChar();
    do {
      //printf("[FROM main()] before lex() %d \n", nextToken);

      lex();

      //printf("[FROM main()] after lex() %d \n\n", nextToken);

      stmt();
    } while (nextToken != EOF);
  }

  // seems like the tokenizer is already implemented for us -> will need to parse as tokens come in


}

void error(){
  printf("ERROR: \n");
}

// the stmt function
void stmt() {
  printf("Enter <stmt>\n");

  if (nextToken == IDENT){
    //printf("[FROM stmt()] found identifier. \n");
    //printf("[FROM stmt()] nextChar before lex() |%d| \n", nextChar);
    lex(); //error in here

    if (nextToken == ASSIGN_OP){
      lex();
      expr();
    }
    //printf("[FROM stmt()] after lex() %d \n\n", nextToken);
  }

  printf("Exit <stmt>\n");
  
  //printf("[FROM stmt()] nxt token %d\n\n", nextToken);
}
void factor(){
  // must choose between two RHSs
  printf("Enter <factor>\n");

  // determine the RHSs
  if(nextToken == IDENT || nextToken == INT_LIT){
    lex(); // get next token
  }
  // if the RHS is <expr> tell the lex to pass over the left parenthesis and call expr and then check for right parentheses
  else {
    if(nextToken == LEFT_PAREN){
      lex();
      expr();
      if(nextToken == RIGHT_PAREN){
        lex();
      }else {
        error(); // should exist at the very top
      }
    }
    else {
      // it wasnt an id or int or left parentheses
      error();
    }
  }

  printf("Exit <factor>\n");
}

// parse term  
void term() {
  printf("Enter <term>\n");

  // parse the first factor
  factor();

  // as long as the next is * or / get the next token and parse the next factor
  while (nextToken == MULT_OP || nextToken == DIV_OP){
    lex(); // get the next token
    factor(); // parse the next factor
  }
  printf("Enter <expr> \n");
}

// parse expression
void expr() {
  printf("Enter <expr> \n");

  // parse first term
  term();

  // as long as the next token is + or -, get the next token and parse the next term
  while(nextToken == ADD_OP || nextToken == SUB_OP){
    // updates next token
    lex();

    // proocesses the term
    term();
  }
  printf("Exit <expr> \n");
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
    case '=':
    // if the equal sign then set as assign op -> not sure what add char does
      //printf("[FROM lookup()] found the = \n");
      addChar();
      nextToken = ASSIGN_OP;
      break;

    default:
      addChar();
      nextToken = EOF;
      break;
  }
  return nextToken;
}
/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
void addChar() {
  if (lexLen <= 98) {
    lexeme[lexLen++] = nextChar;
    lexeme[lexLen] = 0;
  }
  else
    printf("Error - lexeme is too long \n");
}
/*****************************************************/
/* getChar - a function to get the next character of
 input and determine its character class */
void getChar() {
  if ((nextChar = getc(in_fp)) != EOF){
    //printf("\n[FROM getChar()] nextChar = %c \n\n",nextChar);
    if (isalpha(nextChar))
      charClass = LETTER;
    else if (isdigit(nextChar))
      charClass = DIGIT;
  else charClass = UNKNOWN;
 }
 else
 charClass = EOF;
}
/*****************************************************/
/* getNonBlank - a function to call getChar until it
 returns a non-whitespace character */
void getNonBlank() {
 while (isspace(nextChar))
  getChar();
}
/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic
 expressions */
int lex() {
 lexLen = 0;
 getNonBlank();
 switch (charClass) {

/* Parse identifiers */
 case LETTER:
  addChar();
  getChar();

  while (charClass == LETTER || charClass == DIGIT) {
    addChar();
    getChar();
  }

  nextToken = IDENT;
  break;
  
/* Parse integer literals */
 case DIGIT:
  addChar();
  getChar();

  while (charClass == DIGIT) {
    addChar();
    getChar();
  }

  nextToken = INT_LIT;
  break;

/* Parentheses and operators */
 case UNKNOWN:
  //printf("[FROM lex()] nextChar %c\n",nextChar);
  lookup(nextChar);
  getChar();
  break;

/* EOF */
 case EOF:
  nextToken = EOF;
  lexeme[0] = 'E';
  lexeme[1] = 'O';
  lexeme[2] = 'F';
  lexeme[3] = 0;
  break;
 } /* End of switch */

 printf("Next token is: %d, Next lexeme is %s\n", nextToken, lexeme);


 return nextToken;
} /* End of function lex */
