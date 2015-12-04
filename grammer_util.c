#include<string.h>
#include"grammer_util.h"
#include"grammer_analyse_util.h"
#include"word_scanner.h"
#include"myprint.h"
//void Parser(char * SrcFilePtr);
FILE* file=NULL;
Token token={NONTOKEN,"",0,NULL};
/*      Parser 函数处理单一一个源文件 ， Program 函数处理 源文件里的程序代码段,在这里处理整个源文件的程序    */
void Parser(char * SrcFilePtr) // the key function for the grammer analyser
{
	

	if (!(file = InitScanner(SrcFilePtr))){// 初始化词法分析器{ printf("Open Source File Error ! \n");
		printf("init word scanner failed\n");
		system("PAUSE");
		return;
	}

	token= FetchToken(file);//获取第一个记号 
	Program();		// 进入递归下降分析, 分析树在此函数中建立 ，返回分析树的根节点 
	CloseScanner(file);	// 关闭词法分析器


}




struct ExprNode*  MakeExprNode(Token_Type opcode, void* arg1, void* arg2){ // ***构造子树时， 参数顺序不应该传递错误,若只有一个参数有效，则 arg1 有效*** 

	struct ExprNode* node = (struct ExprNode*)malloc(sizeof(struct ExprNode));
	node->opCode = opcode;
	double *tdouble;
	switch(opcode){
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





/*   Program  →  { Statement SEMICO }
*/
void Program( ){
	/*
	进入递归下降分析, 分析树在此函数中建立 ，返回分析树的根节点 (暂未实现）
	每次匹配到终结符， 将自动获取下一个记号
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








/*  Statement →  OriginStatment | ScaleStatment
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
		SyntaxError(4);// 错误提示 不是语句 

	}

}


/*  OriginStatment → ORIGIN IS
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
/*  RotStatment → ROT IS Expression
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
/*  ScaleStatment  → SCALE IS
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
→ FOR T FROM Expression TO Expression STEP Expression
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
	// 绘制图形 或 其他处理

}
 
 







// token 为全局变量，控制 获取下一个记号 很不方便 






//--------每次匹配到终结符， 将自动获取下一个记号--------expression and detail, the expression will return a tree
/* Expression 	→ Term  { ( PLUS | MINUS) Term }

*/
    struct ExprNode*  Expression( ){//Expression only deal with PLUS  or MINUS
	// pass a header for avoid error : when the function exected over , the vary in the function stack will be clear,
	// while the heap will not be clear automativate
 
 
  //  printf("--enter Expression(in function Expression)\n");
  
	struct ExprNode *left=NULL, *right=NULL;


	Token_Type token_tmp; //先获得右孩子，才能构造树，只有一个 全局token, 需要临时变量保存 符号类别 
	left = Term(); // the left  Term

	while (token.type == PLUS || token.type == MINUS)
	{
		token_tmp = token.type;
		//  MatchToken(token_tmp);

        token=GetToken(file);//  Atom 不识别 运算符号 

		right = Term(); // the right Term,//展开右边的 Term，直到下一个加减号 
		left = MakeExprNode(token_tmp, left, right);//造树得到新的树，并作为下一次的左孩子－－也反映了+－具有左结合，自左向右的运算
      //  token=GetToken(file);//获取下一个记号，以判断下一个是否是 PLUS 或者 MINUS //每次匹配一个终结符，自动获取下一个记号
	}

    if(left==NULL){
    	printf("--Expression return NULL (in Expression)\n");
    	system("PAUSE");
    }
    
    
   // printf("--exit Expression(in function Expression)\n");
  //  print_tree(left);//  打印已经构造好的 Expression 语法树 
	return left;
}



/* Term       	→ Factor { ( MUL | DIV ) Factor }
*/
struct ExprNode * Term( ){// Term only deal with MUL or DIV

	struct ExprNode *left=NULL, *right=NULL;
   Token_Type  tmp=NONTOKEN; 
	left = Factor();
   
	while (token.type == MUL || token.type == DIV)
	{
		//MatchToken(&token, token.type, file);// 此处的作用的打印记号信息 并获取下一个记号 
	
        tmp=token.type;
        token=GetToken(file); 
		right = Factor();// Atom 中不包括 运算符号 
		left = MakeExprNode(tmp, left, right);
	//	token=GetToken(file);//每次匹配一个终结符，自动获取下一个记号
	

}

	return left;


}
/*  Factor  	→ ( PLUS | MINUS ) Factor | Component     单操作数
*/
struct  ExprNode * Factor( ){

	struct ExprNode *factor=NULL;
    Token_Type tmp; 
	if (token.type == PLUS || token.type == MINUS){ 
       
     
       tmp=token.type;// atom 不识别 运算符号 
        token=GetToken(file);  //每次匹配一个终结符，自动获取下一个记号
		factor = Factor();
		
		double td=0.00000000;
		struct ExprNode* zero=MakeExprNode(CONST_ID,(void*)&td,(void*)&td);// 单元操作符，在前面补0 
        factor = MakeExprNode(tmp, (void*)zero,(void*)factor);
        
		
	}else{// 此条件存在漏洞 
		if (token.type == NONTOKEN){
			SyntaxError(6);//  print meeting the file end in unexception
		}
		factor = Component();
			}
	return factor;
}
/*  Component 	→ Atom [ POWER Component ]
*/
struct  ExprNode * Component( ){
	struct ExprNode *atom=NULL, *component=NULL;
	atom = Atom();
	while(token.type==POWER){//if match successful ,
	    token=GetToken(file);//每次匹配一个终结符，自动获取下一个记号,Atom 不识别 运算符 
		component = Component();// 
		atom = MakeExprNode(POWER, atom, component);
	
	}
														 
	
	return atom;

}
/*  Atom → CONST_ID
| T
| FUNC L_BRACKET Expression R_BRACKET
| L_BRACKET Expression R_BRACKET   Atom 不包括运算符号 , ' ( ' 或者 ' ) '  不是 Expression 的一部分 
*/
struct  ExprNode * Atom(){
	struct ExprNode *atom=NULL;
	double td;
	Token tmp=token;
	atom = NULL;
	
	token=GetToken(file);//每次匹配一个终结符，自动获取下一个记号 ,先移位是因为 Atom 中 存在递归调用 ： 如 case: FUNC 
	
	
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

