#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINES 5000
#define MAXLEN 1000
char *lineptr[MAXLINES];

int readlines(char *lineptr[], int maxlines)
{
	int len, nlines;
	char *p, line[MAXLEN];
	
	nlines = 0;
	while ((len = getline(line, MAXLEN)) > 0){
		if (nlines >= MAXLINES || (p = malloc(len)) == NULL){
			return -1;
		}else{
			line[len-1] = '\0'; //delete new line
			strcpy(p, line);
			lineptr[nlines++] = p;
		}
		return nlines;
	}
}


void writelines(char *lineptr[], int nlines)
{
	int i;
	for (i=0; i<nlines; ++i){
		printf("%s\n", lineptr[i]);
	}
}

void swap(char *v[], int i, int j)
{
	char *tmp;
	tmp = v[i];
	v[i] = v[j];
	v[j] = tmp;
}

void qsort(char *V[], int left, int right)
{
	int i, last;

	if (left >= right){ //do nothing if array contains
		return ;		//fewer than two elements
	}
	swap(V, left, (left + right)/2);
	last = left;

	for (i=left+1; i<=right; i++){
		if (strcmp(V[i], V[left]) < 0){
			swap(V, ++last, i);
		}
	}
	swap(V, left, last);
	qsort(V, left, last-1);
	qsort(V, last+1, right);
}

int main()
{
	int nlines; //number of input lines read

	if ((nlines = readlines(lineptr, MAXLINES)) >= 0){
		qsort(lineptr, 0, nlines-1);
		writelines(lineptr, nlines);
	}else{
		printf("error: input too big to sort\n");
		return 1;
	}

	return 0;

