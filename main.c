  // THIS IS ALL TEXTBOOK CODE


/* front.c - a lexical analyzer system for simple
 arithmetic expressions */
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

/* Global declarations */
/* Variables */
int charClass;
char lexeme [100];
char nextChar;
int lexLen;
int token;
int nextToken = -2;
off_t fileSize = 0;
off_t getCharCount = 0;

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

// utility functions
off_t fsize(const char *filename) {
    struct stat st; 

    if (stat(filename, &st) == 0)
        return st.st_size;

    return -1; 
}

// define the grammar
/*
<expr> → <term> {(+ | -) <term>}
<term> → <factor> {(* | /) <factor>}
<factor> → id | int_constant | ( <expr> )
*/
/******************************************************/
/* main driver */
int main(int argc, char *argv[]) {

// determine the file size:
fileSize = fsize(argv[1]);

//printf("SIZE OF FILE: %ld \n", fileSize);
/* Open the input data file and process its contents */
  if ((in_fp = fopen(argv[1], "r")) == NULL)
    // open the file
    printf("ERROR - cannot open front.in \n");
  else {
    // get characters and lex()?
    getChar();
    do {

      lex();
      stmt();
    } while (getCharCount < fileSize);

    //printf("GET CHAR COUNT: %ld\n", getCharCount);
    //printf("Execution stopped.");
  }

  // seems like the tokenizer is already implemented for us -> will need to parse as tokens come in


}

void error(const char * msg){
  // printf("ERROR: %s: %d %s \n", msg, nextToken, lexeme);
  printf("Error");
  exit(0);

}

void expect_error(int EXPECTED_CODE){
  // this handles parsing expect errors
  //printf("Parsing Error: Expected %d got token %d, lexeme: %s \n", EXPECTED_CODE, nextToken, lexeme);
  printf("Error");
  exit(0);

}

// the stmt function
void stmt() {
  printf("Enter <stmt>\n");

  if (nextToken == IDENT){
    lex(); // gets the next token

    if (nextToken == ASSIGN_OP){
      lex();
      expr();
    } else {
      // throw an expect error
      expect_error(ASSIGN_OP);
    }
  } 

  printf("Exit <stmt>\n");
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
      expr(); // parse the expr 
      if(nextToken == RIGHT_PAREN){
        lex();
      }else {
        expect_error(RIGHT_PAREN); // should exist at the very top
      }
    }
    else {
      // it wasnt an id or int or left parentheses -- will just call left paren for now.
      expect_error(LEFT_PAREN);
    }
  }

  printf("Exit <factor>\n");
}

// parse term  
void term() {
  printf("Enter <term>\n");

  // parse the first factor
  factor();

  // zero or more
  // as long as the next is * or / get the next token and parse the next factor
  while (nextToken == MULT_OP || nextToken == DIV_OP){
    lex(); // get the next token
    factor(); // parse the next factor
  }
  printf("Exit <term> \n");
}

// parse expression
void expr() {
  printf("Enter <expr> \n");

  // parse first term
  term();

  if(nextToken == IDENT){
    error("Found an Identifier where it was not expected");
  }
  // 0 or more occurences
    // as long as the next token is + or -, get the next token and parse the next term
    while(nextToken == ADD_OP || nextToken == SUB_OP){
      // updates next token
      lex();

      // proocesses the term
      term();

      
    }
  

  // if the next token after the term is not +, - or 
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
      addChar();
      nextToken = ASSIGN_OP;
      break;
    case '\n':
      lexeme[0] = 'E';
      lexeme[1] = 'O';
      lexeme[2] = 'F';
      lexeme[3] = 0;
      nextToken = EOF;
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
    if (isalpha(nextChar))
      charClass = LETTER;
    else if (isdigit(nextChar))
      charClass = DIGIT;
    else 
      charClass = UNKNOWN;
 } else{
   charClass = EOF;
 }
 // anytime this function processes increment the counter
 getCharCount++;
 
}


/*****************************************************/
/* getNonBlank - a function to call getChar until it
 returns a non-whitespace character */
void getNonBlank() {
 while (isspace(nextChar) && nextChar != '\n')
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
