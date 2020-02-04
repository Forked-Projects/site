#include <stdio.h>

#define MAX 100;

int ops[MAX];
char line[MAX];

int main()
{
}

void getops()
{
  char c;
  int i, length, sign;

  for(i = 0; (c = getchar()) != '\n' && c != EOF; i++)
    if(c == EOF)
      return 0;
    else
      line[i] = c;
  line[i] = '\0';
  length = i;

  for(i = 0; line[i] != '\0'; i++)
    while(line[i] != ' ' || line[i] != '\t')
      ;
    if(line[i] == '-' && line[i + 1] >= '0' && line[i + 1] <= '9')
      sign = -1;
    else
      sign = 1;
    
}

