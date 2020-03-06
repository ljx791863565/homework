#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

//词汇分析器
int KeyWord(char *ch)
{
	int i;
	//定义关键字集
	char ch2[32][9] = {
		"auto", "double", "int", "struct", "break", "else", "long", "switch", "case", "emum",
		"register", "typedef", "char", "extern", "return", "union", "const", "float", "short",
		"unsigned", "continue", "for", "signed", "void", "default", "goto", "sizeof", "volatile",
		"do", "while", "static", "if"
	};
	for (i=0; i<32; ++i){
		if (!strcmp(ch, ch2[i])){
			return i+1;
		}
	}
	return 47;
}

int main(int argc, char *argv[])
{
	if (argc != 3){
		printf("输入参数有误\n");
		return -1;
	}
	FILE *fpFrom, *fpTo;
	if ((fpFrom = fopen(argv[1], "r")) == NULL){
		perror("fpFrom:fopen");
		return -1;
	}
	if ((fpTo = fopen(argv[2], "w")) == NULL){
		perror("fpTo:fopen");
		return -1;
	}
	int ch;		//保存读取到的字符
	int rowCount = 1;//保存行数,从1开始
	while (!feof(fpFrom)){
		ch = fgetc(fpFrom);
		if (ch == 10){
			rowCount++;
		}
		if (isalpha(ch) || ch == '_'){
			int i = 0;
			char ch1[30] = {0};
			ch1[i++] = ch;
			while (!feof(fpFrom)){
				ch = fgetc(fpFrom);
				if (ch == 10){
					rowCount++;
				}
				if (isalpha(ch) || isdigit(ch) || ch == '_'){
					ch1[i++] = ch;
				}
				if (ch == '.'){
					if ((ch = fgetc(fpFrom)) == 'h'){
						if (ch == 10){
							rowCount++;
							ch1[i++] = '.';
							ch1[i++] = 'h';
							ch1[i] = '\0';
							printf("line :%d	%s	83\n",rowCount, ch1);
							fprintf(fpTo, "line :%d		%s	83\n", rowCount, ch1);
							break;
						}
					}else{
						fseek(fpFrom, -1, 1);
						ch1[i] = '\0';
						printf("line :%d	%s	%d\n", rowCount, ch1, KeyWord(ch1));
						fprintf(fpTo, "line :%d	%s	%d\n", rowCount, ch1, KeyWord(ch1));
						break;
					}
				}
				if (!isdigit(ch) && !isalpha(ch) && ch != '_' && ch != '.'){
					ch1[i] = '\0';
					printf("line :%d	%s	%d\n", rowCount, ch1, KeyWord(ch1));
					fprintf(fpTo, "line :%d	%s	%d\n", rowCount, ch1, KeyWord(ch1));
					break;
				}
			}
		}

		if (isdigit(ch) || ch == '-'){
			if (isdigit(ch)){
				printf("line :%d	%c", rowCount, ch);
				fprintf(fpTo, "line :%d	%c", rowCount, ch);
				while (!feof(fpFrom)){
					ch = fgetc(fpFrom);
					if (isdigit(ch) || ch == '.'){
						printf("%c", ch);
						fprintf(fpFrom, "%c", ch);
					}else{
						printf("	46\n");
						fprintf(fpFrom, "	46\n");
						fseek(fpFrom, -1, 1);
						ch = '0';
						break;
					}
				}
			}
			if (ch == '-'){
				ch = fgetc(fpFrom);
				if (ch == '-'){
					printf("line :%d	--	80\n", rowCount);
					fprintf(fpFrom, "line :%d	--	80\n", rowCount);
				}
				if (ch == '>'){
					printf("line :%d	->	81\n", rowCount);
					fprintf(fpFrom, "line :%d	->	81\n", rowCount);
				}
				if (isdigit(ch)){
					fseek(fpFrom, -3, 1);
					ch = fgetc(fpFrom);
					if (isdigit(ch)){
						ch = fgetc(fpFrom);
						printf("line :%d	%c	79\n", rowCount, ch);
						fprintf(fpFrom, "line :%d	%c	79\n", rowCount, ch);
					}else{
						ch = fgetc(fpFrom);
						printf("line :%d	%c\n", rowCount, ch);
						fprintf(fpFrom, "line :%d	%c\n", rowCount, ch);
						while (!feof(fpFrom)){
							ch = fgetc(fpFrom);
							if (isdigit(ch) || ch == '.'){
								printf("%c", ch);
								fprintf(fpFrom, "%c", ch);
							}else{
								printf("			49\n");
								fprintf(fpTo, "			49\n");
								fseek(fpFrom, -1, 1);
								break;
							}
						}
					}
				}
			}
		}

		if (ch == '/')
		{
			ch = fgetc(fpFrom);
			if (ch == 10){
				rowCount++;
			}
			if (ch == '/'){
				while (fgetc(fpFrom) != 10){
					if (ch == 10){
						rowCount++;
					}
				}
			}
			if (ch == '*'){
				while (!feof(fpFrom)){
					ch = fgetc(fpFrom);
					if (ch == 10){
						rowCount++;
					}
					if (ch == '*'){
						ch = fgetc(fpFrom);
						if (ch == '/'){
							break;
						}
					}
				}
			}else{
				printf("line :%d	/	83\n", rowCount);
				fprintf(fpTo, "line :%d	/	83\n", rowCount);
				fseek(fpFrom, -1, 1);
				break;
			}
		}

		if (ch == '"'){
			int i = 0;
			printf("line :%d	%c	82\n", rowCount, ch);
			fprintf(fpTo, "line :%d	%c	82\n", rowCount, ch);
			printf("line :%d		\n", rowCount);
			fprintf(fpTo, "line :%d		\n", rowCount);

			while (!feof(fpFrom)){
				ch = fgetc(fpFrom);
				i++;
				if (ch == 10){
					rowCount++;
				}
				if (ch != '"'){
					if (ch != 32){
						printf("%c", ch);
						fprintf(fpTo, "%c", ch);
					}
				}else{
					break;
				}
			}
			printf("	99\n");
			fprintf(fpTo, "		99\n");
			fseek(fpFrom, -i, 1);
			int k;
			for (; i>0; --i){
				ch = fgetc(fpFrom);
				if (ch == 92){
					char ch5[12] = {
						"abfntv\\\?\'\"\0"
					};
					ch = fgetc(fpFrom);
					for (k=0; k<11; k++){
						if (ch == ch5[k]){
							printf("line :%d	\\%c	%d\n", rowCount, ch, k+33);
							fprintf(fpTo, "line :%d	\\%c	%d\n", rowCount, ch, k+33);
						}
					}
					if (ch == 'd' && isdigit(fgetc(fpFrom)) && isdigit(fgetc(fpFrom))){
						fseek(fpFrom, -2, 1);
						printf("line :%d	%c%c	44\n", rowCount, fgetc(fpFrom), fgetc(fpFrom));
						fprintf(fpTo, "line :%d	%c%c	44\n", rowCount, fgetc(fpFrom), fgetc(fpFrom));
					}
					if (ch == 'x' && isdigit(fgetc(fpFrom)) && isdigit(fgetc(fpFrom))){
						fseek(fpFrom, -2, 1);
						printf("line :%d	%c%c	45\n", rowCount, fgetc(fpFrom), fgetc(fpFrom));
						fprintf(fpTo, "line :%d	%c%c	45\n", rowCount, fgetc(fpFrom), fgetc(fpFrom));
					}
				}
				if (ch == '%'){
					ch = fgetc(fpFrom);
					char bfh[4] = {
						"dcs"
					};
					for (i = 0; i<3; ++i){
						printf("line :%d	%%%c	83\n", rowCount, ch);
						fprintf(fpTo, "line :%d	%%%c	83\n", rowCount, ch);
					}
				}
			}
		}

		if (!isdigit(ch) && !isalpha(ch) && ch != '/' && ch != '_' && ch != '"'){
			char ch2[14] = {
				"#()[]{}'*:~^%"
			};
			char ch3[9] = {
				"+?=|&!<>"
			};
			char ch4[9] = {
				"+==|&==="
			};
			int i, j;
			for (i=0; i<13; ++i){
				if (ch == ch2[i]){
					printf("line :%d	%c	%d\n", rowCount, ch, i+48);
					fprintf(fpTo, "line :%d	%c	%d\n", rowCount, ch, i+48);
				}
			}
			for (j=0; j<8; ++j){
				if (ch == ch3[j]){
					ch = fgetc(fpFrom);
					if (ch == ch4[j]){
						printf("line :%d	%c%c	%d\n", rowCount, ch3[j], ch4[j], i+69);
						fprintf(fpTo, "line :%d	%c%c	%d\n", rowCount, ch3[j], ch4[j], i+69);
					}
					if (ch == '<' && ch3[j] == '<'){
						printf("line :%d	<<	77\n", rowCount);
						fprintf(fpTo, "line :%d	<<	77\n", rowCount);
					}
					if (ch == '>' && ch3[j] == '>'){
						printf("line :%d	>>	78\n", rowCount);
						fprintf(fpTo, "line :%d	>>	78\n", rowCount);
					}else{
						printf("line :%d	%c	%d\n", rowCount, ch3[j], j+61);
						fprintf(fpTo, "line :%d	%c	%d\n", rowCount, ch3[j], j+61);
						fseek(fpFrom, -1, 1);
					}
				}
			}
		}
	}

	fclose(fpFrom);
	fclose(fpTo);
	return 0;
}
