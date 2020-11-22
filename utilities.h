#ifndef __UTILITIES_H
#define __UTILITIES_H

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_WORDS 20
#define MAX_STRING_LEN 20

void errore(char* s, int n);
char** split(char* s, char* seps);
char* readCSV (FILE* fp, char sep);
char* inputStr();
int str2int(char*s);
char* int2str(int n);
char* strrev(char* sx);
char* mystrdup(char* s);

void errore(char* s, int n) {
	printf("ERROR in %s - %d - %s\n", s, errno, strerror(errno));
	printf("return code: %d\n", n);
	exit(n);
}

char** split(char* s, char* seps) {
	char* temp[MAX_WORDS+1];
	char* s1 = strdup(s);
	char* token = strtok(s1, seps);
	
	int i = 0;
	for(i=0; token != NULL ; i++) { 
   		temp[i] = strdup(token);
   		token = strtok(NULL, seps);
   	}
   	temp[i] = NULL;
   	free(s1);
   	
   	char** ret = (char**) malloc(sizeof(char*) * i);
   	for( i=0; *(ret+i) = temp[i]; i++)  { }
   	return ret;
}

char* readCSV (FILE* fp, char sep) {
	char temp[MAX_STRING_LEN +1];
	int i = 0;
	for(; 	((temp[i] = fgetc(fp)) != sep) &&
			(temp[i] != '\n') &&
			(temp[i] != EOF) ;
			i++) { }
	if (i==0) return NULL;
	temp[i] = '\0';
	return strdup(temp);
}

char* inputStr() {
	char temp[MAX_STRING_LEN +1];
	int i = 0;
	for(; (temp[i] = getchar()) != '\n' ; i++) { }
	temp[i] = '\0';
	return strdup(temp);	
}

int str2int(char*s) {
	int ret = 0;
	for(;*s; s++)
		ret = *s -'0' + ret * 10;
	return ret;
}

char* int2str(int n) {
	char buffer[MAX_STRING_LEN];
	int i;
	for(i=0; n; i++) {
		int resto = n % 10;
		buffer[i] = '0' + resto;
		n = n / 10; /* la divisione tra interi restituisce un intero */
	}
	buffer[i] = '\0';
	strrev(buffer);
	return strdup(buffer);
}

char* strrev(char* sx) {
	char* saved = sx;
	char* dx = sx + strlen(sx) -1;
	for(; sx < dx; sx++, dx--) {
		char temp = *sx;
		*sx = *dx;
		*dx = temp;
	}
	return saved;
}

char* mystrdup(char* s) {
	char* s2 = (char*) malloc(sizeof(char) * (strlen(s) +1));
	char* ret = s2;
	for(; *s2 = *s; s++, s2++) { }
	return ret;
}

#endif  /* __UTILITIES_H */
