/*
输入 :　来自词法分析器的记号流
输出　：　构造号的　分析树　或者　语法树
错误处理：　检查并报告语法错误
*/

#include <stdio.h>
#include <stdlib.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

extern void Parser(char * SrcFilePtr);
int main(int argc, char *argv[])
{
/*	if (argc<2){
		printf("Input Source! not gave source file path\n");
		system("PAUSE");
		return -1;
	}
	
	*/ 
	Parser("testsource.txt");// the grammer analyse fucntion
	system("PAUSE");
	return 0;
}
