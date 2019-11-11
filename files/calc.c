#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXOP 100
#define NUMBER 1

char getop();
char s[MAXOP];
void push(double f);
double pop();

int sp = 0;
double val[MAXOP];

int main()
{
  int type;
  double op1, op2;

  while ((type = getop()) != EOF) {
    switch(type) {
      case NUMBER:
	push(atof(s));
	break;
      case '+':
	push(pop() + pop());
	break;
      case '-':
	op2 = pop();
	push(pop() - op2);
	break;
      case '*':
	push(pop() * pop());
	break;
      case '/':
	op2 = pop();
	if(op2 != 0.0)
	  push(pop() / op2);
	else
	  printf("error: division by zero\n");
	break;
      case '%':
	op2 = pop();
	op1 = pop();
	if(op2 != 0.0)
	  push((op1 / op2) - ((int) (op1 / op2)));
	else
	  printf("error: division by zero\n");
	break;
      case '\n':
	printf("%g\n", pop());
	break;
      default:
	printf("error: unknown command %s\n", s);
	break;
    }
  }
}

char getop()
{
  int i, c;
  static int lastchar;
  i = 0;

  if(lastchar) {
    c = lastchar;
    lastchar = 0;
    return c;
  }
  else {
    while ((s[0] = c = getchar()) == ' ' || c == '\t')
      ;
    if(!isdigit(c) && c != '.' && c != '-')
      return c;
    if(c == '-')
      if(!isdigit(s[1] = c = getchar())) {
	lastchar = c;
        return s[0];
      }
    if(isdigit(c))
      while (isdigit(s[++i] = c = getchar()))
        ;
    if (c == '.')
      while (isdigit(s[++i] = c = getchar()))
        ;
    s[i] = '\0';
    return NUMBER;
  }
}

void push(double f)
{
  if(sp < MAXOP)
    val[sp++] = f;
  else
    printf("error: stack is full, can't push %f\n", f);
}

double pop()
{
  if(sp > 0)
    return val[--sp];
  else
    printf("error: stack is empty, nothing to pop\n");
    return 0.0;
}
