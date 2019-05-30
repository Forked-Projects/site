#include <stdio.h>

//   /*      //    ''    ""    ()    {}    []
int comm1, comm2, quo1, quo2, bra1, bra2, bra3, bbra1, bbra2, bbra3;
int last, before, bbefore, line;

int main()
{
  last = before = bbefore = 0;
  comm1 = comm2 = quo1 = quo2 = bra1 = bra2 = bra3 = bbra1 = bbra2 = bbra3 = 0;
  line = 1;

  while((last = getchar()) != EOF) {
    if(last == '\\') {
      last = getchar();
      last = getchar();
    }
    if(last == '\n')
      ++line;
    if(comm1 == 0 && comm2 == 0) {
      if(quo2 == 0 && last == '\'') {
        if(quo1 == 0)
	  quo1 = line;
	else
	  quo1 = 0;
      }
      if(last == '\"') {
        if(quo2 == 0)
	  quo2 = line;
	else
	  quo2 = 0;
      }
      if(quo1 == 0 && quo2 == 0) {
        if(last == '*' && before == '/')
          comm1 = line;
        if(last == '/' && before == '/')
          comm2 = line;
        if(last == '(') {
	  ++bra1;
	  bbra1 = line;
	}
        if(last == '{') {
	  ++bra2;
	  bbra2 = line;
	}
        if(last == '[') {
	  ++bra3;
	  bbra3 = line;
	}
        if(last == ')')
	  --bra1;
        if(last == '}')
	  --bra2;
        if(last == ']')
	  --bra3;
      }
    }
    else if(comm1 > 0 && last == '/' && before == '*' && bbefore != '/')
      comm1 = 0;
    else if(comm2 > 0 && last == '\n')
      comm2 = 0;

  bbefore = before;
  before = last;
  }

  if(comm1 > 0)
    printf("You haven't closed commentary /**/ started at line %d\n", comm1);
  if(quo1 > 0)
    printf("You have nonpaired quote ' in line %d\n", quo1);
  if(quo2 > 0)
    printf("You have nonpaired quote \" in line %d\n", quo2);
  if(bra1 > 0)
    printf("You have nonpaired bracket '(' near the line %d\n", bbra2);
  if(bra1 < 0)
    printf("You have nonpaired bracket ')' near the line %d\n", bbra2);
  if(bra2 > 0)
    printf("You have nonpaired bracket '{' near the line %d\n", bbra2);
  if(bra2 < 0)
    printf("You have nonpaired bracket '}' near the line %d\n", bbra2);

printf("%d %d %d %d %d %d %d %d %d %d\n", comm1, comm2, quo1, quo2,\
	  bra1, bra2, bra3, bbra1, bbra2, bbra3);
}
