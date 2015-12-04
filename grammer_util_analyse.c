
#include"grammer_analyse_util.h"
#include"word_scanner.h"
#include"myprint.h"
//#include"grammer_util.h"


Token FetchToken(FILE* file){	// 获取第一个记号
	/*
	其中：token是存放记号的全局量；
	GetToken()是词法分析器接口；
	SyntaxError(case_of_error)是出错处理。

	*/ 

Token token = { NONTOKEN, "", 0, NULL };
token = GetToken(file); //调用词法分析器接口
  
  //printf("--%s(in FetchToken)\n",token.lexeme);
  

if (token.type == ERRTOKEN){
	SyntaxError(1);
}
return token;


}


int MatchToken(Token *token, Token_Type AToken, FILE* file){ // if match successful, return 0
	/* 用于匹配当前记号，若匹配失败,指出一个语法错误*/
	if (token->type != AToken) {
		SyntaxError(2);
		return 2;
	}
	//匹配完成后自动向后移位
	(*token) = FetchToken(file);
	return 0;
}

void SyntaxError(int error_id){
	int i = 0;
	for (; i<=ERROR_INFO_NUMBER; ++i){
		if (error_id == (Util_Error_all[i].id)){
			printf(" Error %d --- %s\n", i+1, Util_Error_all[i].info);
			return;
		}
	}
}

