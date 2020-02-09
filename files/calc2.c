#include <stdio.h>
#define MAX 100

int opp = 0;
int ops[MAX];
char line[MAX];

int gline();
void getops();
void calc();
int digit(char c);
int compat(char c);

int main()
{
  while(gline() > 0) {
    getops();
    calc();
  }
}

int gline()
{
  char c;
  int i;

  for(i = 0; (c = getchar()) != '\n' && c != EOF && compat(c) && i < MAX; i++) {
    line[i] = c;
  }
  line[i] = '\0';
  if(c == EOF)
    return 0;
  if(!compat(c) && c != '\n') {
    printf("wat?\n");
    return -1;
  }
  return i;

}

void getops()
{
  int i;

  for(i = 0; line[i] != '\0'; i++) {
    
  }
}

void calc()
{
  int i;

  for(i = 0; opp > 0; i++, opp--)
    printf("%d\n", ops[i]);
}

int digit(char c)
{
  if(c >= '0' && c <= '9')
    return 1;
  else
    return 0;
}

int compat(char c)
{
  if(digit(c) || c == '-' || c == '+' || c == '*' \
              || c == '/' || c == ' ' || c == '\t')
    return 1;
  else
    return 0;
}
