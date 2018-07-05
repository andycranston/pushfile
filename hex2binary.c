/*
 *  @(!--#) @(#) hex2binary.c, version 001, 04-july-2018
 *
 *  convert hex on stdin to byte output
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

int main(argc, argv)
  int   argc;
  char *argv[];
{
  char *progname;
  int   linenum;
  char  line[MAX_LINE_LENGTH + sizeof(char)];
  int   lenline;
  int   i;
  char  hex[3];
  int   byte;

  progname = argv[0];

  linenum = 0;
  
  while (fgets(line, MAX_LINE_LENGTH, stdin) != NULL) {
    linenum++;

    lenline = strlen(line);

    if (line[lenline - 1] == '\n') {
      lenline--;
    }

    for (i = 0; i < lenline; i = i + 2) {
      hex[0] = line[i];
      hex[1] = line[i+1];
      hex[2] = '\0';

      sscanf(hex, "%x", &byte);

      putchar(byte);
    }
  }

  exit(0);
}

/* end of file */
