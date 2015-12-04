#include<string.h>
#include"grammer_util.h"
#include"grammer_analyse_util.h"
#include"word_scanner.h"
#include"myprint.h"
//void Parser(char * SrcFilePtr);
FILE* file=NULL;
Token token={NONTOKEN,"",0,NULL};
/*      Parser ��������һһ��Դ�ļ� �� Program �������� Դ�ļ���ĳ�������,�����ﴦ������Դ�ļ��ĳ���    */
void Parser(char * SrcFilePtr) // the key function for the grammer analyser
{
	

	if (!(file = InitScanner(SrcFilePtr))){// ��ʼ���ʷ�������{ printf("Open Source File Error ! \n");
		printf("init word scanner failed\n");
		system("PAUSE");
		return;
	}

	token= FetchToken(file);//��ȡ��һ���Ǻ� 
	Program();		// ����ݹ��½�����, �������ڴ˺����н��� �����ط������ĸ��ڵ� 
	CloseScanner(file);	// �رմʷ�������


}




struct ExprNode*  MakeExprNode(Token_Type opcode, void* arg1, void* arg2){ // ***��������ʱ�� ����˳��Ӧ�ô��ݴ���,��ֻ��һ��������Ч���� arg1 ��Ч*** 

	struct ExprNode* node = (struct ExprNode*)malloc(sizeof(struct ExprNode));
	node->opCode = opcode;
	double *tdouble;
	switch(opcode){
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





/*   Program  ��  { Statement SEMICO }
*/
void Program( ){
	/*
	����ݹ��½�����, �������ڴ˺����н��� �����ط������ĸ��ڵ� (��δʵ�֣�
	ÿ��ƥ�䵽�ս���� ���Զ���ȡ��һ���Ǻ�
	*/
	while (token.type != NONTOKEN){
		Statement();
		if (MatchToken(&token,SEMICO,file)){// match whether there are the end mark of statement  ' ; '
			SyntaxError(7);
			break;
		}
	}
   printf("source analyse successful\n");
}








/*  Statement ��  OriginStatment | ScaleStatment
|  RotStatment    | ForStatment
*/
void Statement( ){

	switch(token.type){
	case ORIGIN:
		OriginStatement();
		break;
	case ROT:
		RotStatement();
		break;
	case SCALE:
		ScaleStatement();
		break;
	case FOR:
		ForStatement();
		break;
	default:
		SyntaxError(4);// ������ʾ ������� 

	}

}


/*  OriginStatment �� ORIGIN IS
L_BRACKET Expression COMMA Expression R_BRACKET
*/
void OriginStatement( ){

	struct ExprNode *left=NULL, *right=NULL;

	MatchToken(&token, ORIGIN, file);
	MatchToken(&token, IS, file);
	MatchToken(&token, L_BRACKET, file);

	left = Expression();

	MatchToken(&token, COMMA, file);

	right = Expression();

   
  //  print_tree(right);
   
	MatchToken(&token, R_BRACKET, file);
}
/*  RotStatment �� ROT IS Expression
*/
void RotStatement( ){

	struct ExprNode* rot=NULL;

	MatchToken(&token, ROT, file);

	MatchToken(&token, IS, file);

	rot = Expression();
/*	if(rot!=NULL) 
	print_tree(rot); 
	else{
	printf("---Expression return NULL (in ROTStatement of grammer_util.c)\n");
    system("PAUSE");
}
*/
}
/*  ScaleStatment  �� SCALE IS
L_BRACKET Expression COMMA Expression R_BRACKET
*/
void ScaleStatement( ){

	struct ExprNode *left=NULL, *right=NULL;

	MatchToken(&token, SCALE, file);
	MatchToken(&token, IS, file);
	MatchToken(&token, L_BRACKET, file);

	left = Expression();

	MatchToken(&token, COMMA, file);

	right = Expression();

	MatchToken(&token, R_BRACKET, file);
}

/*   ForStatment
�� FOR T FROM Expression TO Expression STEP Expression
DRAW L_BRACKET Expression COMMA Expression R_BRACKET
*/
void ForStatement( ){
	struct ExprNode *start_ptr=NULL, *end_ptr=NULL,
		*step_ptr=NULL, *x_ptr=NULL, *y_ptr=NULL;

	MatchToken(&token, FOR, file);
	MatchToken(&token, T, file);
	

	MatchToken(&token, FROM, file);
	start_ptr = Expression();

	MatchToken(&token, TO, file);
	end_ptr = Expression();

	MatchToken(&token, STEP, file);
	step_ptr = Expression();

	MatchToken(&token, DRAW, file);

	MatchToken(&token, L_BRACKET, file);
	x_ptr = Expression();

	MatchToken(&token, COMMA, file);
	y_ptr = Expression();

	MatchToken(&token, R_BRACKET, file);
	// ����ͼ�� �� ��������

}
 
 







// token Ϊȫ�ֱ��������� ��ȡ��һ���Ǻ� �ܲ����� 






//--------ÿ��ƥ�䵽�ս���� ���Զ���ȡ��һ���Ǻ�--------expression and detail, the expression will return a tree
/* Expression 	�� Term  { ( PLUS | MINUS) Term }

*/
    struct ExprNode*  Expression( ){//Expression only deal with PLUS  or MINUS
	// pass a header for avoid error : when the function exected over , the vary in the function stack will be clear,
	// while the heap will not be clear automativate
 
 
  //  printf("--enter Expression(in function Expression)\n");
  
	struct ExprNode *left=NULL, *right=NULL;


	Token_Type token_tmp; //�Ȼ���Һ��ӣ����ܹ�������ֻ��һ�� ȫ��token, ��Ҫ��ʱ�������� ������� 
	left = Term(); // the left  Term

	while (token.type == PLUS || token.type == MINUS)
	{
		token_tmp = token.type;
		//  MatchToken(token_tmp);

        token=GetToken(file);//  Atom ��ʶ�� ������� 

		right = Term(); // the right Term,//չ���ұߵ� Term��ֱ����һ���Ӽ��� 
		left = MakeExprNode(token_tmp, left, right);//�����õ��µ���������Ϊ��һ�ε����ӣ���Ҳ��ӳ��+���������ϣ��������ҵ�����
      //  token=GetToken(file);//��ȡ��һ���Ǻţ����ж���һ���Ƿ��� PLUS ���� MINUS //ÿ��ƥ��һ���ս�����Զ���ȡ��һ���Ǻ�
	}

    if(left==NULL){
    	printf("--Expression return NULL (in Expression)\n");
    	system("PAUSE");
    }
    
    
   // printf("--exit Expression(in function Expression)\n");
  //  print_tree(left);//  ��ӡ�Ѿ�����õ� Expression �﷨�� 
	return left;
}



/* Term       	�� Factor { ( MUL | DIV ) Factor }
*/
struct ExprNode * Term( ){// Term only deal with MUL or DIV

	struct ExprNode *left=NULL, *right=NULL;
   Token_Type  tmp=NONTOKEN; 
	left = Factor();
   
	while (token.type == MUL || token.type == DIV)
	{
		//MatchToken(&token, token.type, file);// �˴������õĴ�ӡ�Ǻ���Ϣ ����ȡ��һ���Ǻ� 
	
        tmp=token.type;
        token=GetToken(file); 
		right = Factor();// Atom �в����� ������� 
		left = MakeExprNode(tmp, left, right);
	//	token=GetToken(file);//ÿ��ƥ��һ���ս�����Զ���ȡ��һ���Ǻ�
	

}

	return left;


}
/*  Factor  	�� ( PLUS | MINUS ) Factor | Component     ��������
*/
struct  ExprNode * Factor( ){

	struct ExprNode *factor=NULL;
    Token_Type tmp; 
	if (token.type == PLUS || token.type == MINUS){ 
       
     
       tmp=token.type;// atom ��ʶ�� ������� 
        token=GetToken(file);  //ÿ��ƥ��һ���ս�����Զ���ȡ��һ���Ǻ�
		factor = Factor();
		
		double td=0.00000000;
		struct ExprNode* zero=MakeExprNode(CONST_ID,(void*)&td,(void*)&td);// ��Ԫ����������ǰ�油0 
        factor = MakeExprNode(tmp, (void*)zero,(void*)factor);
        
		
	}else{// ����������©�� 
		if (token.type == NONTOKEN){
			SyntaxError(6);//  print meeting the file end in unexception
		}
		factor = Component();
			}
	return factor;
}
/*  Component 	�� Atom [ POWER Component ]
*/
struct  ExprNode * Component( ){
	struct ExprNode *atom=NULL, *component=NULL;
	atom = Atom();
	while(token.type==POWER){//if match successful ,
	    token=GetToken(file);//ÿ��ƥ��һ���ս�����Զ���ȡ��һ���Ǻ�,Atom ��ʶ�� ����� 
		component = Component();// 
		atom = MakeExprNode(POWER, atom, component);
	
	}
														 
	
	return atom;

}
/*  Atom �� CONST_ID
| T
| FUNC L_BRACKET Expression R_BRACKET
| L_BRACKET Expression R_BRACKET   Atom ������������� , ' ( ' ���� ' ) '  ���� Expression ��һ���� 
*/
struct  ExprNode * Atom(){
	struct ExprNode *atom=NULL;
	double td;
	Token tmp=token;
	atom = NULL;
	
	token=GetToken(file);//ÿ��ƥ��һ���ս�����Զ���ȡ��һ���Ǻ� ,����λ����Ϊ Atom �� ���ڵݹ���� �� �� case: FUNC 
	
	
	switch (tmp.type){
	case CONST_ID:
		//	atom->Content.CaseConst=atof(tmp.lexeme);
	//	td = toDouble(tmp.lexeme,strlen(tmp.lexeme));
	//	printf("--%12f (in Atom function grammer_util.c)\n",tmp.value); 
		atom = MakeExprNode(CONST_ID, (void *)&(tmp.value), (void*)0);
		break;
	case T:
		//	atom->Content.CaseParmPtr= parmeter// T parmeter
	
		atom = MakeExprNode(T, (void*)&(tmp.value), (void*) 0);
		break;
	case FUNC:
		//atom->Content
		//token=GetToken(file);
		atom = MakeExprNode(FUNC, (void*)tmp.FuncPtr, (void*)Expression());
		break;
	case L_BRACKET:// L_BRACKET and  R_BRACKET  is not included in the grammer tree 
	//	token=GetToken(file);
		atom = Expression( );
		MatchToken(&token,R_BRACKET,file); 
		break;
	default:
		SyntaxError(5);
	}

	return atom;

}

