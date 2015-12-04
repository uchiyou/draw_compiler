#ifndef SCANNER_H
#define SCANNER_H


#define TOKEN_LEN 64// token 字符串的最大长度 
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
typedef enum		  // 记号的类别，共23个
{
	ORIGIN, SCALE, ROT, IS,	  // 保留字（一字一码）
	TO, STEP, DRAW, FOR, FROM, // 保留字
	T,				  // 参数
	SEMICO, L_BRACKET, R_BRACKET, COMMA,// 分隔符
	PLUS, MINUS, MUL, DIV, POWER,		// 运算符
	FUNC,				  // 函数（调用）
	CONST_ID,			  // 常数
	COMMENT,               //注释 
	NONTOKEN,			  // 空记号（源程序结束）
	ERRTOKEN			  // 出错记号（非法输入）

}Token_Type;

typedef struct 		// 记号的数据结构
{
	Token_Type type;	// 类别
	char  lexeme[TOKEN_LEN];// 属性，原始输入的字符串
	double value;// 属性，若记号是常数则是常数的值
	double(*FuncPtr)(double);// 属性，若记号是函数则是函数指针
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
（去除注释与白空有11个正规式）




*/

#define TOKENTAB_NUM 18
static Token TokenTab[] = 
{ 
{ CONST_ID, "PI", 3.1415926, NULL },
{ CONST_ID, "E", 2.71828, NULL },
{ T, "T", 0.0, NULL },
{ FUNC, "SIN", 0.0, sin },
{ FUNC, "COS", 0.0, cos },//使用数学相关的函数地址， 需要 math.h 
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
