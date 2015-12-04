
#include"grammer_analyse_util.h"
#include"word_scanner.h"
#include"myprint.h"
//#include"grammer_util.h"


Token FetchToken(FILE* file){	// ��ȡ��һ���Ǻ�
	/*
	���У�token�Ǵ�żǺŵ�ȫ������
	GetToken()�Ǵʷ��������ӿڣ�
	SyntaxError(case_of_error)�ǳ�����

	*/ 

Token token = { NONTOKEN, "", 0, NULL };
token = GetToken(file); //���ôʷ��������ӿ�
  
  //printf("--%s(in FetchToken)\n",token.lexeme);
  

if (token.type == ERRTOKEN){
	SyntaxError(1);
}
return token;


}


int MatchToken(Token *token, Token_Type AToken, FILE* file){ // if match successful, return 0
	/* ����ƥ�䵱ǰ�Ǻţ���ƥ��ʧ��,ָ��һ���﷨����*/
	if (token->type != AToken) {
		SyntaxError(2);
		return 2;
	}
	//ƥ����ɺ��Զ������λ
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

