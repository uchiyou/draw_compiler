#ifndef GRAMMER_UTIL_H
#define GRAMMER_UTIL_H
#include<stdio.h>
#include"word_scanner.h"

typedef double(*FuncPtr)(double);

struct ExprNode
{
	Token_Type opCode;	// �Ǻ�(���)����
	union{
		struct {
			struct ExprNode *Left, *Right; //2������
		} CaseOperator;	// ��Ԫ����
		struct {
			struct ExprNode * Child;  //1������
			FuncPtr MathFuncPtr;
		} CaseFunc;	// ��������

		double CaseConst; 	// ����������ֵ
		//double * CaseParmPtr; 	// ����T������ֵ
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
