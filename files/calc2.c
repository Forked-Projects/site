#include <stdio.h>
#define MAX 100
#define ERR 1
#define EOL 1

int ops[MAX];
char line[MAX];

int gline();
int getop();

int main()
{
  int i;

  while(gline() > 0) {
    for(i = 0; i < MAX; i++)
      ops[i] = getop();
  }
  for(i = 0; i < 10; i++)
    printf("%d\n", ops[i]);
}

int gline()
{
  char c;
  int i;

  for(i = 0; (c = getchar()) != '\n' && c != EOF && i < MAX; i++) {
    line[i] = c;
  }
  if(c == EOF)
    return 0;
  line[i] = '\0';
  return i;
}

int getop()
{
  static int i;
  int num, sign;
  sign = 1;
  num = 0;
  
  while(line[i] == ' ' || line[i] == '\t')
    i++;
  while(line[i] != ' ' && line[i] != '\t' && line[i] != '\0') {
    if(line[i] == '-' && line[i + 1] >= '0' && line[i + 1] <= '9')
      sign = -1;
    if(line[i] >= '0' && line [i] <= '9')
      num = num * 10 + (line[i] - '0');
    else {
      printf("illegal number\n");
    }
    i++;
  }
  num = sign * num;
  return num;
}
