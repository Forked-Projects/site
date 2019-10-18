#include <stdio.h>
#include <ctype.h>
#define MAXLINE 1000

char string[MAXLINE];
int readstring();
double atof(char string[]);

int main()
{
  readstring();
  printf("%f\n", atof(string));
}

int readstring()
{
  int i = 0;
  char c;

  while((c = getchar()) != '\n')
    string[i++] = c;
  string[i] = '\0';

  return i;
}

double atof(char string[])
{
  int i, sign, e, neg_e;
  double val, power, e10;
  sign = 1;
  e = 1;
  e10 = 1.0;
  neg_e = 0;

  for(i = 0; isspace(string[i]); i++)
    ;
  if(string[i] == '-')
    sign = -1;
  if(string[i] == '+' || string[i] == '-')
    i++;
  for(val = 0.0; isdigit(string[i]); i++)
    val = 10.0 * val + (string[i] - '0');
  if(string[i] == '.' || string[i] == ',') {
    i++;
    for (power = 1.0; isdigit(string[i]); i++) {
      val = 10.0 * val + (string[i] - '0');
      power = power * 10;
    }
  }
  if(string[i] == 'e') {
    i++;
    if(string[i] == '-')
      neg_e = 1;
    if(string[i] == '-' || string[i] == '+')
      i++;
    for(e = 0; isdigit(string[i]); i++)
      e = e * 10 + (string[i] - '0');
    for(i = 1; i <= e; i++)
      e10 *= 10.0;
    if(neg_e)
      e10 = 1 / e10;
  }
  return(sign * val * e10 / power);
}
