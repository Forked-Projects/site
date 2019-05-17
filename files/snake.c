#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int height = 28;
int width  = 60;

int snake_x = 10;
int snake_y = 10;

int apple_x, apple_y;

void apple()
{
  srand(time(NULL));
  apple_x = rand()%width;
  apple_y = (rand()/3)%height;
}

int area()
{
  int h, w;

  for(w = 1; w <= width; ++w)
    printf("-");
  printf("+\n");
  for(h = height; h >= 1; --h) {
    for(w = 1; w <= width; ++w) {
      if(h == snake_y && w == snake_x)
	printf("x");
      else if(h == apple_y && w == apple_x)
	printf("O");
      else
        printf(" ");
      }
    printf("|\n");
  }

  return 0;
}

int main()
{
  int c;

  apple();

  while(c != 10) {
    area();
    system("stty raw");
    c = getchar();
    system("stty cooked");
    if(c == 65) {
      if(snake_y == height)
	snake_y = 1;
      else
        snake_y = snake_y + 1;
    }
    if(c == 66) {
      if(snake_y == 1)
	snake_y = height;
      else
        snake_y = snake_y - 1;
    }
    if(c == 67) {
      if(snake_x == width)
	snake_x = 1;
      else
        snake_x = snake_x + 1;
    }
    if(c == 68) {
      if(snake_x == 1)
	snake_x = width;
      else
        snake_x = snake_x - 1;
    }
    if(apple_x == snake_x && apple_y == snake_y)
      apple();
  }

  return 0;
}
