#include"word_scanner.h"
#include"grammer_analyse_util.h"
#include"grammer_util.h"

/*      Parser 函数处理单一一个源文件 ， Program 函数处理 源文件里的程序代码段,在这里处理整个源文件的程序    */
void Parser(char * SrcFilePtr) // the key function for the grammer analyser
{
	Token token;
	FILE* file;

	if (!(file = InitScanner(SrcFilePtr))){// 初始化词法分析器{ printf("Open Source File Error ! \n");
		printf("init word scanner failed\n");
		system("PAUSE");
		return;
	}

	token= FetchToken(file);//获取第一个记号 
	Program(token, file);		// 进入递归下降分析, 分析树在此函数中建立 ，返回分析树的根节点 
	CloseScanner(file);	// 关闭词法分析器


}

