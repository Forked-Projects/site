#include <stdio.h>
#define MAX 100
#define NUM 1

int opp = 0;
int ops[2][MAX];
char line[MAX];

int gline();
void getops();
void calc();
void squeeze(int i);
int digit(char c);
int compat(char c);

int main()
{
  int i;

  while(gline() > 0) {
    getops();
    if(ops[0][opp - 1] == NUM) {
      for(i = 0; i < opp; i++) {
	switch(ops[1][i]) {
	  case '*':
	    ops[1][i - 1] = ops[1][i - 1] * ops[1][i + 1];
	    squeeze(i);
	    break;
	  case '/':
	    ops[1][i - 1] = ops[1][i - 1] / ops[1][i + 1];
	    squeeze(i);
	    break;
	}
      }
      for(i = 0; i < opp; i++) {
	switch(ops[1][i]) {
	  case '+':
	    ops[1][i - 1] = ops[1][i - 1] + ops[1][i + 1];
	    squeeze(i);
	    break;
	  case '-':
	    ops[1][i - 1] = ops[1][i - 1] - ops[1][i + 1];
	    squeeze(i);
	    break;
	}
      }
    }
    for(i = 0; i < opp; i++)
      printf("%d\n", ops[1][i]);
  }
}

int gline()
{
  char c;
  int i;

  for(i = 0; (c = getchar()) != '\n' && c != EOF && compat(c) && i < MAX; i++) {
    line[i] = c;
  }
  line[i++] = '\0';
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
  int i, num;

  for(i = 0; line[i] != '\0'; i++) {
    while(line[i] == ' ' || line[i] == '\t')
      i++;
    if(digit(line[i])) {
      num = 0;
      while(digit(line[i]))
	num = num * 10 + (line[i++] - '0');
      ops[0][opp] = NUM;
      ops[1][opp++] = num;
    }
    if(line[i] == '-' || line[i] == '+' || line[i] == '*' || line[i] == '/') {
      ops[0][opp] = 0;
      ops[1][opp++] = line[i];
    }
  }
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

void squeeze(int i)
{
  opp -= 2;
  while(i++ < opp)
    ops[1][i] = ops[1][i + 2];
}
