#include <stdio.h>

main()
{
  int c, i, word, length;
  int gisto[20];

  c = i = word = length = 0;
  while((c = getchar()) != EOF) {
    if (c != ' ' && c != '\t' && c != '\n') {
      ++length;
      word = 1;
    }
    if((c == ' ' || c == '\t' || c == '\n') && word == 1) {
      gisto[length - '0'] = gisto[length - '0'] + 1;
      length = 0;
      word = 0;
    }
  }
  for(i = 0; i <= 20; ++i) {
    printf("%d\n", gisto[i]);
  }
}
