#include <stdio.h>

int ngetline(char *line);

int main(int argc, char argv[])
{
  char *line = NULL;

  while(ngetline(*line) >= 0)
    if(strocc(line, argv[1]))
      printf("%s", line);
  return 0;
}

int ngetline(char *line)
{
  char c;
  int i;

  for(i = 0; (c = getchar()) != EOF && c != '\n'; i++)
    *line++ = c;
  *line = '\0';
  line = line - i;
  return i;
}

int strcmp()
{
}
