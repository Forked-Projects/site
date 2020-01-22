#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void error();

int main(int argc, char *argv[])
{
  char c, *line[10000], *sline, *bline;
  int i = 0;

  bline = sline = malloc(1);
  while((c = getchar()) != EOF) {
    if(c != '\n')
      *sline++ = c; 
    else {
      *sline++ = '\0';
      line[i++] = bline;
      bline = sline;
    }
  }

  if(argc > 1) {
    if(*argv[1] == '-') {
      switch(*(argv[1] + 1)) {
      case 'n':
        if(argc > 2 && isdigit(*argv[2]))
          i -= atoi(argv[2]); 
        else
	  error();
        break;
      default:
        error();
        break;
      }
    }
    else
      error();
  }
  else if(i > 9)
    i -= 10;
  else
    i = 0;
  while(line[i]) {
    printf("%5d  %s\n", i, line[i]);
    i++;
  }
  return 0;
}

void error()
{
  printf("Usage: tail [ -n number of last lines]\n");
}
