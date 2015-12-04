/*
用于构造表达式的节点， （确定化 ExprNode 中的联合体）
*/
#include"word_scanner.h"
#include"grammer_util.h"
#include"grammer_analyse_util.h"

struct ExprNode*  MakeExprNode(Token_Type opcode, void* arg1, void* arg2){ // ***构造子树时， 参数顺序不应该传递错误*** 

	struct ExprNode* node = (struct ExprNode*)malloc(sizeof(struct ExprNode));
	node->opCode = opcode;
	double *tdouble;
	switch (opcode){
	case CONST_ID:	// 常数结点
		tdouble = (double*)(arg1);
		node->Content.CaseConst = *tdouble;
		break;
	case T:		// 参数结点
		//	node->Content.CaseParmPtr = &Parameter; //全局量
		tdouble = (double*)(arg1);
		node->Content.CaseParmPtr = *tdouble;
		break;
	case FUNC:	       // 函数调用结点
		node->Content.CaseFunc.MathFuncPtr = (FuncPtr)arg1;
		node->Content.CaseFunc.Child = (struct ExprNode *)arg2;
		break;
	default:	       // 二元运算结点
		node->Content.CaseOperator.Left = (struct ExprNode *)arg1;
		node->Content.CaseOperator.Right = (struct ExprNode *)arg2;// 如果 arg2 为 NULL 意味着为单操作数 
		break;
	}
	return node;
}

