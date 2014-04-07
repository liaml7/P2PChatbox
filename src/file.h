#include <stdio.h>

void *fileRead(char host[BUFSIZ]){
  FILE *parsefile;
  parsefile = fopen("tracker.txt", "r");

  fscanf(parsefile, " host: %s", host);
  fclose(parsefile);
}