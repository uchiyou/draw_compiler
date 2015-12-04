/*
���ڹ�����ʽ�Ľڵ㣬 ��ȷ���� ExprNode �е������壩
*/
#include"word_scanner.h"
#include"grammer_util.h"
#include"grammer_analyse_util.h"

struct ExprNode*  MakeExprNode(Token_Type opcode, void* arg1, void* arg2){ // ***��������ʱ�� ����˳��Ӧ�ô��ݴ���*** 

	struct ExprNode* node = (struct ExprNode*)malloc(sizeof(struct ExprNode));
	node->opCode = opcode;
	double *tdouble;
	switch (opcode){
	case CONST_ID:	// �������
		tdouble = (double*)(arg1);
		node->Content.CaseConst = *tdouble;
		break;
	case T:		// �������
		//	node->Content.CaseParmPtr = &Parameter; //ȫ����
		tdouble = (double*)(arg1);
		node->Content.CaseParmPtr = *tdouble;
		break;
	case FUNC:	       // �������ý��
		node->Content.CaseFunc.MathFuncPtr = (FuncPtr)arg1;
		node->Content.CaseFunc.Child = (struct ExprNode *)arg2;
		break;
	default:	       // ��Ԫ������
		node->Content.CaseOperator.Left = (struct ExprNode *)arg1;
		node->Content.CaseOperator.Right = (struct ExprNode *)arg2;// ��� arg2 Ϊ NULL ��ζ��Ϊ�������� 
		break;
	}
	return node;
}

