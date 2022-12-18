#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse.tab.h"
#include "types.h"

#define CUTCR(str) (str[strlen(str) - 1] = '\0')

// 読み込み中の文字列バッファ
#define BLEN 20
char b[BLEN];
char *bp;

// yylex用の文字列バッファ
static char *tokenbuf = NULL;
static int tokidx, toksiz = 0;

#define tokfix() (tokenbuf[tokidx] = '\0')
#define tok() tokenbuf
#define toklen() tokidx
#define toklast() (tokidx > 0 ? tokenbuf[tokidx - 1] : 0)
#define tokfree()                                                              \
  {                                                                            \
    free(tokenbuf);                                                            \
    tokenbuf = NULL;                                                           \
  }
static char *newtok() {
  tokidx = 0;
  if (!tokenbuf) {
    toksiz = 60;
    tokenbuf = (char *)calloc(60, sizeof(char));
  }
  if (toksiz > 4096) {
    toksiz = 60;
    tokenbuf = (char *)realloc(tokenbuf, 60);
  }
  tokfix();
  return tokenbuf;
}
static void tokadd(char c) {
  tokenbuf[tokidx++] = c;
  if (tokidx >= toksiz) {
    toksiz *= 2;
    tokenbuf = (char *)realloc(tokenbuf, 60);
  }
  tokfix();
}

extern int yylex() {
  char t;

  while (*(bp) == ' ')
    bp++;

  t = *bp;
  if (t == '\0')
    return YYEOF;

  int toktype;
  switch (t) {
  case '+':
  case '-':
  case '*':
  case '/':
  case '(':
  case ')':
    toktype = t;
    break;

  case '\n':
    toktype = CR;
    break;

  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
    newtok();
    tokadd(t);
    while (isdigit(*(bp + 1))) { // 先読み
      tokadd(*(++bp));
    }
    yylval.num = atof(tok());
    toktype = NUMBER;
    tokfree();
    break;

  default:
    toktype = YYUNDEF;
    break;
  }

  bp++;
  return toktype;
}

void run() {
  while (1) {
    printf(">> ");
    fgets(b, BLEN, stdin);
    if (feof(stdin))
      break;

    bp = b;
    yyparse();
  }
}

int main() { run(); }
