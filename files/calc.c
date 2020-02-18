#include <stdio.h>
#define MAX 100
#define CLEAR 0
#define NUM 1
#define OPER 2

int opp = 0;
float ops[2][MAX];
char line[MAX];

int gline();
int digit(char c);
int compat(char c);
int calc();
int getops();
void squeeze(int i);

int main()
{
  int i, l;

  ops[0][opp++] = NUM;
  while((l = gline())) {
    if(l > 0) {
      if(getops() < 0 || calc() < 0) {
        for(i = 1; i <= opp; i++) {
          ops[0][i] = CLEAR;
          ops[1][i] = CLEAR;
        }
        opp = 1;
      }
    }
    printf("%g\n", ops[1][0]);
  }
}

int calc()
{
  int i;
  for(i = 1; i < opp; i++) {
    if(ops[0][i] == ops[0][i + 1]) {
      printf("error: non arithmetic expression\n");
      return -1;
    }
    if(ops[1][i] == '/' && ops[1][i + 1] == 0) {
      printf("error: division by zero\n");
      return -1;
    }
  }
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
        if(ops[1][i] == '*' || ops[1][i] == '/') {
	  if(ops[1][i] == '*')
            ops[1][i - 1] = ops[1][i - 1] * ops[1][i + 1];
	  if(ops[1][i] == '/')
            ops[1][i - 1] = ops[1][i - 1] / ops[1][i + 1];
          squeeze(i);
        }
	else
	  ++i;
      }
      else
        ++i;
    }
    for(i = 1; i < opp; ) {
      if(ops[0][i] == OPER) {
        if(ops[1][i] == '+' || ops[1][i] == '-') {
	  if(ops[1][i] == '+')
            ops[1][i - 1] = ops[1][i - 1] + ops[1][i + 1];
	  if(ops[1][i] == '-')
            ops[1][i - 1] = ops[1][i - 1] - ops[1][i + 1];
          squeeze(i);
        }
	else
	  ++i;
      }
      else
        ++i;
    }
  }
  return 0;
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
    printf("error: unrecognized char in line\n");
    return -1;
  }
  return i;
}

int getops()
{
  int i, sign, power, point, firstinline;
  float num;

  firstinline = 1;
  for(i = 0; line[i] != '\0'; i++) {
    while(line[i] == ' ' || line[i] == '\t')
      i++;
    if(digit(line[i]) || line[i] == '.' ||
          (line[i] == '-' && digit(line[i + 1]) &&
              (ops[0][opp - 1] == OPER || (firstinline) ))) {
      num = 0;
      point = 0;
      power = 1;
      sign = 1;
      if(line[i] == '-') {
        sign = -1;
        ++i;
      }
      while(digit(line[i]) || line[i] == '.') {
        if(point > 1) {
          printf("error: too many points in number\n");
          return -1;
        }
        if(digit(line[i])) {
          num = num * 10.0 + (line[i++] - '0');
          if(point > 0)
            power *= 10.0;
        }
        else {
	  i++;
          point++;
	}
      }
      ops[0][opp] = NUM;
      ops[1][opp++] = sign * num / power;
      --i;
    }
    else if(line[i] == '-' ||  line[i] == '+'
            || line[i] == '*' || line[i] == '/') {
      ops[0][opp] = OPER;
      ops[1][opp++] = line[i];
    }
    firstinline = 0;
  }
  return 0;
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
  if(digit(c) || c == '-' || c == '+' || c == '*' || c == '.'
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
