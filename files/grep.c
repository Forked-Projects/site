#include <stdio.h>
#include <stdlib.h>

int ngetline(char *line);
void strocc();

int main(int argc, char *argv[])
{
  char *line = NULL;
  int l;

  while((l = ngetline(line)) > 0)
    printf("%s\n", line);
  return 0;
}

int ngetline(char *line)
{
  char c, *start;
  int i;
  start = line = malloc(1);

  for(i = 0; (c = getchar()) != EOF && c != '\n'; i++)
    *line++ = c;
  *line = '\0';
  line = start;
  if(c >= 0)
    return i;
}

void strocc()
{
}
