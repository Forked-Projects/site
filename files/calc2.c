#include <stdio.h>
#define MAX 100;

int ops[MAX];
char line[MAX];

int gline();
int getop();

int main()
{

  while(gline() > 0) {
    for(i = 0; (op = getop()) != '\0' && i < MAX; i++)
      ops[i] = op;
  }
}

int gline()
{
  char c;
  int i, length;

  for(i = 0; (c = getchar()) != '\n' && c != EOF && i < MAX; i++)
    if(c == EOF)
      return 0;
    else
      line[i] = c;
  line[i] = '\0';
  length = i;
}

int getop()
{
    while(line[i] != ' ' || line[i] != '\t')
      ;
    if(line[i] == '-' && line[i + 1] >= '0' && line[i + 1] <= '9')
      sign = -1;
    else
      sign = 1;
    if(line[i] >= '0' && line [i] <= '9')
      ops[j] = ops[j] * 10 + line[i];
}
