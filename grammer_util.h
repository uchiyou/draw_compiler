#ifndef GRAMMER_UTIL_H
#define GRAMMER_UTIL_H
#include<stdio.h>
#include"word_scanner.h"

typedef double(*FuncPtr)(double);

struct ExprNode
{
	Token_Type opCode;	// 记号(算符)种类
	union{
		struct {
			struct ExprNode *Left, *Right; //2个孩子
		} CaseOperator;	// 二元运算
		struct {
			struct ExprNode * Child;  //1个孩子
			FuncPtr MathFuncPtr;
		} CaseFunc;	// 函数调用

		double CaseConst; 	// 常数，绑定右值
		//double * CaseParmPtr; 	// 参数T，绑定左值
		double  CaseParmPtr;

	} Content;
};
struct ExprNode*  MakeExprNode( Token_Type, void*, void*);


void Parser(char * SrcFilePtr);
void Program();
void Statement();
void OriginStatement();
void RotStatement();
void ScaleStatement();
void ForStatement();

struct ExprNode * Expression( );
struct ExprNode * Term( );
struct ExprNode * Factor( );
struct ExprNode * Component( );
struct ExprNode * Atom();

#endif
