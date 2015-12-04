#ifndef SCANNER_H
#define SCANNER_H


#define TOKEN_LEN 64// token �ַ�������󳤶� 
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
typedef enum		  // �Ǻŵ���𣬹�23��
{
	ORIGIN, SCALE, ROT, IS,	  // �����֣�һ��һ�룩
	TO, STEP, DRAW, FOR, FROM, // ������
	T,				  // ����
	SEMICO, L_BRACKET, R_BRACKET, COMMA,// �ָ���
	PLUS, MINUS, MUL, DIV, POWER,		// �����
	FUNC,				  // ���������ã�
	CONST_ID,			  // ����
	COMMENT,               //ע�� 
	NONTOKEN,			  // �ռǺţ�Դ���������
	ERRTOKEN			  // ����Ǻţ��Ƿ����룩

}Token_Type;

typedef struct 		// �Ǻŵ����ݽṹ
{
	Token_Type type;	// ���
	char  lexeme[TOKEN_LEN];// ���ԣ�ԭʼ������ַ���
	double value;// ���ԣ����Ǻ��ǳ������ǳ�����ֵ
	double(*FuncPtr)(double);// ���ԣ����Ǻ��Ǻ������Ǻ���ָ��
}Token;




/*



letter	= [a-zA-Z]
digit	= [0-9]

COMMENT   	= "//"|"--"
WHITE_SPACE 	= (" "|\t|\n|\r)+
SEMICO    	= ";"
L_BRACKET 	= "("
R_BRACKET 	= ")"
COMMA     	= ","
PLUS      	= "+"
MINUS     	= "-"
MUL      	= "*"
DIV       	= "/"
POWER     	= "**"
CONST_ID    	= digit+("." digit*)?
ID        	= letter+ (letter|digit)*
��ȥ��ע����׿���11������ʽ��




*/

#define TOKENTAB_NUM 18
static Token TokenTab[] = 
{ 
{ CONST_ID, "PI", 3.1415926, NULL },
{ CONST_ID, "E", 2.71828, NULL },
{ T, "T", 0.0, NULL },
{ FUNC, "SIN", 0.0, sin },
{ FUNC, "COS", 0.0, cos },//ʹ����ѧ��صĺ�����ַ�� ��Ҫ math.h 
{ FUNC, "TAN", 0.0, tan },
{ FUNC, "LN", 0.0, log },
{ FUNC, "EXP", 0.0, exp },
{ FUNC, "SQRT", 0.0, sqrt },
{ ORIGIN, "ORIGIN", 0.0, NULL },
{ SCALE, "SCALE", 0.0, NULL },
{ ROT, "ROT", 0.0, NULL },
{ IS, "IS", 0.0, NULL },
{ FOR, "FOR", 0.0, NULL },
{ FROM, "FROM", 0.0, NULL },
{ TO, "TO", 0.0, NULL },
{ STEP, "STEP", 0.0, NULL },
{ DRAW, "DRAW", 0.0, NULL }
};


double toDouble(char*, int);
FILE* InitScanner(char*);
Token GetToken(FILE*);
void CloseScanner(FILE*);



#endif
