#include <stdio.h>
#include <string.h>

#define CUTCR(str) (str[strlen(str) - 1] = '\0')

#define BLEN 256
char b[BLEN];
char *bp;

int yylex() {
  int tok;
  tok = *bp;
  if (tok == '\0')
    return 0;
  bp++;
  return tok;
}

void lex() {
  printf(">> ");
  fgets(b, BLEN, stdin);
  CUTCR(b);

  bp = b;
  int tok;
  while ((tok = yylex()) != 0) {
    printf("%d\n", tok);
  }
}

int main() { lex(); }
