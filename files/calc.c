#include <stdio.h>
#define MAX 100
#define CLEAR 0
#define NUM 1
#define OPER 2

int opp = 0;
int ops[2][MAX];
char line[MAX];

int gline();
int digit(char c);
int compat(char c);
void getops();
void squeeze(int i);

int main()
{
  int i, l, error;

  ops[0][opp++] = NUM;
  while((l = gline())) {
    error = 0;
    if(l > 0) {
      getops();
      for(i = 1; i < opp; i++) {
	if(ops[0][i] == ops[0][i + 1])
	  error = 1;
      }
      if(!(error)) {
        if(ops[0][1] == NUM) {
          --opp;
	  for(i = 0; i < opp; i++) {
            ops[0][i] = ops[0][i + 1];
	    ops[1][i] = ops[1][i + 1];
          }
	  ops[0][opp] = CLEAR;
	  ops[1][opp] = CLEAR;
        }
        if(ops[0][opp - 1] == NUM) {
          for(i = 1; i < opp; ) {
	    if(ops[0][i] == OPER) {
	      switch(ops[1][i]) {
	        case '*':
	          ops[1][i - 1] = ops[1][i - 1] * ops[1][i + 1];
	          squeeze(i);
	          break;
	        case '/':
	          ops[1][i - 1] = ops[1][i - 1] / ops[1][i + 1];
	          squeeze(i);
	          break;
	        default:
	          ++i;
	      }
	    }
	    else
	      ++i;
          }
          for(i = 1; i < opp; ) {
	    if(ops[0][i] == OPER) {
	      switch(ops[1][i]) {
	        case '+':
	          ops[1][i - 1] = ops[1][i - 1] + ops[1][i + 1];
	          squeeze(i);
	          break;
	        case '-':
	          ops[1][i - 1] = ops[1][i - 1] - ops[1][i + 1];
	          squeeze(i);
	          break;
	        default:
	          ++i;
	      }
	    }
	    else
	      ++i;
          }
	}
      }
      else {
	for(i = 1; i <= opp; i++) {
	  ops[0][i] = CLEAR;
	  ops[1][i] = CLEAR;
	}
	opp = 1;
        printf("wat?\n");
      }
    }
    else if(l < 0) {
      printf("wat?\n");
    }
    printf("%d\n", ops[1][0]);
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
    while(getchar() != '\n')
      ;
    return -1;
  }
  return i;
}

void getops()
{
  int i, num, sign, firstinline;

  firstinline = 1;
  for(i = 0; line[i] != '\0'; i++) {
    while(line[i] == ' ' || line[i] == '\t')
      i++;
    if(digit(line[i]) ||
	  (line[i] == '-' && digit(line[i + 1]) &&
	      (ops[0][opp - 1] == OPER || (firstinline) ))) {
      num = 0;
      sign = 1;
      if(line[i] == '-') {
	sign = -1;
	++i;
      }
      while(digit(line[i]))
	num = num * 10 + (line[i++] - '0');
      ops[0][opp] = NUM;
      ops[1][opp++] = sign * num;
      --i;
    }
    else if(line[i] == '-' ||  line[i] == '+'
            || line[i] == '*' || line[i] == '/') {
      ops[0][opp] = OPER;
      ops[1][opp++] = line[i];
    }
    firstinline = 0;
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
  if(digit(c) || c == '-' || c == '+' || c == '*'
              || c == '/' || c == ' ' || c == '\t')
    return 1;
  else
    return 0;
}

void squeeze(int i)
{
  opp -= 2;
  for( ; i < opp; i++) {
    ops[0][i] = ops[0][i + 2];
    ops[1][i] = ops[1][i + 2];
  }
  ops[0][i] = CLEAR;
  ops[1][i] = CLEAR;
  ops[0][i + 1] = CLEAR;
  ops[1][i + 1] = CLEAR;
}
