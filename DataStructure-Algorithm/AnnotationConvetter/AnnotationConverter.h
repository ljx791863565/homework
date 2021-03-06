/*************************************************************************
    > File Name: AnnotationConverter.h
    > Author: ljx
    > Mail: liaojia138797@163.com 
    > Created Time: Tue 17 Dec 2019 10:22:46 AM CST
 ************************************************************************/
#ifndef __ANNOTATION_CONCERTER_H___
#define __ANNOTATION_CONCERTER_H__

#include <stdio.h>
#include <stdlib.h>

#define INPUT_FILE_NAME "input.c"
#define OUTPUT_FILE_NAME "output.c"

typedef enum state
{
	NULL_STATE,
	C_STATE,
	CPP_STATE,
	END_STATE
}STATE_TYPE;

void converter(void);
//核心转换函数
void converter_core(FILE *read, FILE *write);
//无状态处理
void converter_NULL(FILE *read, FILE *write, STATE_TYPE *pstate);
//C状态处理
void converter_c(FILE *read, FILE *write, STATE_TYPE *pstate);
//CPP状态处理
void converter_cpp(FILE *read, FILE *write, STATE_TYPE *pstate);



#endif

