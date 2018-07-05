/*
 *  convert stdin to hex output
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BYTES_PER_LINE 16

int main()
{
  int   c;
  int   col;

  col = 0;
  while ((c = getchar()) != EOF) {
    if (col >= BYTES_PER_LINE) {
      printf("\n");
      col = 0;
    }
    printf("%02X", c);
    col++;
  }

  putchar('\n');

  exit(0);
}

/* end of file */
