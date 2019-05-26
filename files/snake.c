#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int height = 23;
int width  = 60;

int apple_x, apple_y;

int snake_n = 3;
int snake_x[25];
int snake_y[25];
int snake_tail_x;
int snake_tail_y;

void apple()
{
  srand(time(NULL));
  apple_x = rand()%width;
  apple_y = (rand()/3)%height;
  if(apple_x == 0 || apple_y == 0)
    apple();
}

int area()
{
  int h, w, n;

  for(w = 1; w <= width; ++w)
    printf("-");
  printf("+\n");
  for(h = height; h >= 1; --h) {
    for(w = 1; w <= width; ++w) {
      int snake = 0;
      for(n = 0; n <= snake_n; ++n) {
        if(h == snake_y[n] && w == snake_x[n])
	  snake = 1;
      }
      if (snake == 1)
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

void snake_move()
{
  int n;

  snake_tail_x = snake_x[snake_n];
  snake_tail_y = snake_y[snake_n];

  for(n = snake_n; n >= 1; --n) {
    snake_x[n] = snake_x[n - 1];
    snake_y[n] = snake_y[n - 1];
  }
}

void check_food()
{
  int n;

  if(apple_x == snake_x[0] && apple_y == snake_y[0]) {
    ++snake_n;
    snake_x[snake_n] = snake_tail_x;
    snake_y[snake_n] = snake_tail_y;
    apple();
    for(n = 0; n <= snake_n; ++n) {
      if(apple_x == snake_x[n] && apple_y == snake_y[n])
        apple();
    }
  }
}

int main()
{
  int c, i;
  snake_x[0] = 10;
  snake_y[0] = 10;
  for(i = 1; i <= snake_n; ++i) {
    snake_x[i] = snake_x[0];
    snake_y[i] = snake_y[i - 1] - 1;
  }

  apple();

  while(c != 'q') {
    area();
    system("stty raw");
    c = getchar();
    system("stty cooked");
    if(c == 65 || c == 'k') {
      snake_move();
      if(snake_y[0] == height)
	snake_y[0] = 1;
      else
        snake_y[0] = snake_y[0] + 1;
      check_food();
    }
    if(c == 66 || c == 'j') {
      snake_move();
      if(snake_y[0] == 1)
	snake_y[0] = height;
      else
        snake_y[0] = snake_y[0] - 1;
      check_food();
    }
    if(c == 67 || c == 'l') {
      snake_move();
      if(snake_x[0] == width)
	snake_x[0] = 1;
      else
        snake_x[0] = snake_x[0] + 1;
      check_food();
    }
    if(c == 68 || c == 'h') {
      snake_move();
      if(snake_x[0] == 1)
	snake_x[0] = width;
      else
        snake_x[0] = snake_x[0] - 1;
      check_food();
    }
  }

  return 0;
}
