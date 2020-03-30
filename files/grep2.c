#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINE 500

char *pattern;
void grep(char *fname, FILE *fp, char *pattern);
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
      grep("input stream", stdin, pattern);
    else {
      while(argc-- > 2)
	if((fp = fopen(*++argv, "r")) != NULL)
          grep(*argv, fp, pattern);
        else {
	  fprintf(stderr, "grep: %s: no such file or directory\n", *argv);
          exit(2);
	}
    }
  }
}

void grep(char *fname, FILE *fp, char *pattern)
{
  char *line;
  size_t nline = 0;
  line = malloc(1);

  while(fgets(line, MAXLINE, fp) != NULL) {
    ++nline;
    if(strstr(line, pattern))
      printf("%s: %zu: %s", fname, nline, line);
  }
  fclose(fp);
}
