char *fileRead(char *filename, char *result){
	char str[1024], ch;
	int i=0;
	FILE *fp;
	fp = fopen(filename, "r");
	if(fp){
		fgets(str, 1024, fp);
		fclose(fp);
		if(str[strlen(str)-1] == '\n')
			str[strlen(str)-1] = 0;
		result = str;
		return result;
	}
	else{
		return NULL;
	}
}
