#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXOP 100
#define NUMBER 1

char s[MAXOP];
char buf[MAXOP];
int bufp = 0;

char getop();
void push(double f);
double pop();
int getch();
void ungetch();

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
  i = 0;

  while ((s[0] = c = getch()) == ' ' || c == '\t')
    ;
  if(!isdigit(c) && c != '.' && c != '-')
    return c;
  if(c == '-')
    if(!isdigit(s[++i] = c = getch())) {
      ungetch(c);
      return s[0];
    }
  if(isdigit(c))
    while (isdigit(s[++i] = c = getch()))
      ;
  if (c == '.')
    while (isdigit(s[++i] = c = getch()))
      ;
  s[i] = '\0';
  return NUMBER;
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

int getch()
{
  if(bufp > 0)
    return buf[--bufp];
  else
    return getchar();
}

void ungetch(int c)
{
  if(bufp < MAXOP)
    buf[bufp++] = c;
  else
    printf("ungetch buffer is full\n");
}
