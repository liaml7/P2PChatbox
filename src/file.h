#include <stdio.h>

void *readFile(char *filename, char host[BUFSIZ]){
  FILE *parsefile;
  parsefile = fopen(filename, "r");
  fscanf(parsefile, "host:%s", host);
  fclose(parsefile);
}
