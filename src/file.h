const char *fileRead(char *filename){
	char str[256], ch;
	int i;
	i=0;
	FILE *fp;
	fp = fopen(filename, "r");
	if(fp){
		while ((ch = getc(fp)) != EOF){
			str[i++] = ch;
		}
		fclose(fp);
		if(str[strlen(str)-1] == '\n')
			str[strlen(str)-1] = 0;
		return str;
	}
	else{
		return NULL;
	}
}
