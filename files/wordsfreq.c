#include <stdio.h>

int main()
{
  int c, i, n, word, length;
  int gisto[21];
  float cor, height;

  c = i = n = word = length = cor = 0;
  for(i = 0; i <= 20; ++i)
    gisto[i] = 0;

  while((c = getchar()) != EOF) {
    if(c == ' ' || c == '\t' || c == '\n') {
      if(word == 1) {
        gisto[length] = gisto[length] + 1;
        length = 0;
        word = 0;
      }
    }
    else {
      if(length < 20)
        ++length;
      word = 1;
    }
  }
  for(i = 1; i <= 20; ++i) {
    if(gisto[i] > cor)
      cor = gisto[i];
  }
  cor = cor / 22.0;
  for(n = 22; n >= 1; --n) {
    for(i = 1; i <= 20; ++i) {
      height = gisto[i] / cor;
      if(height >= n || (n == 1 && height > 0))
	printf(" - ");
      else
	printf("   ");
    }
  printf("\n");
  }
  for(i = 1; i <= 20; ++i)
    printf("%2d ", i);
  printf("\n");
}
