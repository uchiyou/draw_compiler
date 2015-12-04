#include<stdio.h> 
#include<stdlib.h>
#include<string.h>
#include"myprint.h"
#include"grammer_util.h" 

int print_tree(struct ExprNode* header){
	
    
	if(header==NULL){
		printf("--the expression is null（ in function print_tree) \n");
		system("PAUSE");
		return -1;
	}
	if(header->opCode==NONTOKEN || header->opCode==ERRTOKEN){
		printf("--NONToke or error token in print_tree\n");
		system("PAUSE");
		return -2;
	}

	struct ExprNode* node=header;
	
			
	switch (node->opCode){
	case CONST_ID:	
		
		printf("---const id %12f \n",node->Content.CaseConst);
		break;
	case T:		
	
		printf("--- T %ld \n",node->Content.CaseParmPtr);
		break;
	case FUNC:	       // 函数调用结点
		printf("--- function %x \n",node->Content.CaseFunc.MathFuncPtr);
		print_tree(node->Content.CaseFunc.Child);
		break; 
	case PLUS: 
	case MINUS:
		case MUL: 
		case DIV:
		case POWER:	       // 二元运算结点
		
	        print_tree(node->Content.CaseOperator.Left);
			printf("--- operator character %d \n",node->opCode);
			print_tree(node->Content.CaseOperator.Right);
		break;
	default:
		printf("--error token in print_tree\n");
	}
		
	
	return 0;
}

int print_state(Token token){// used in word_scanner.c
	 
	 	static char state_tmp[20]="";
	  //  static int const_flag=0;
	 
   /*	ORIGIN, SCALE, ROT, IS,	  // 保留字（一字一码）*/
   switch(token.type){
   	case ROT:
   	case ORIGIN:
   	case SCALE:
   	case FOR:
   		strcpy(state_tmp,token.lexeme);
   		printf("--enter %s\n",token.lexeme);
   		break;
   	case SEMICO:
   		printf("--exit %s \n",state_tmp);
   		break;
   	case CONST_ID:
   	case L_BRACKET:
   	case R_BRACKET:
   	case COMMA:
   	case PLUS:
   	case MINUS:
   	case DIV:
   	case MUL:
   
   	// 过滤掉
   		break;
   	default:
   	    printf("-- %s \n",token.lexeme);
   }
   
	return 0;
}
