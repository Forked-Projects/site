#include <stdio.h>
#include <string.h>

char *pattern;
void grep(char *fname, char *pattern);
FILE *fp;

int main(int argc, char *argv[])
{
  if(argc == 1) {
    fprintf(stderr, "Usage: grep PATTERN [FILES]\n");
    return 0;
  }
  else {
    pattern = *++argv;
    if(argc == 2)
      grep("stdin", pattern);
    else {
      while(argc-- > 2)
        grep(*++argv, pattern);
    }
  }
}

void grep(char *fname, char *pattern)
{
  char *line = NULL;
  size_t linecap = 32;
  size_t nline = 0;

  fp = fopen(fname, "r");
  while(getline(&line, &linecap, fp) != EOF) {
    ++nline;
    if(strstr(line, pattern))
      printf("%s: %zu: %s", fname, nline, line);
  }
}
