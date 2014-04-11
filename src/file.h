#include <stdio.h>

void *getHost(char host[BUFSIZ]){
  FILE *parsefile;
  parsefile = fopen("settings.dat", "r");
  fscanf(parsefile, "host:%s", host);
  fclose(parsefile);
}
void *getChatPassword(char password[256]){
  FILE *parsefile;
  parsefile = fopen("settings.dat", "r");
  fscanf(parsefile, "password:%s", password);
  fclose(parsefile);
}
