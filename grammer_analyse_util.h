#ifndef GRAMMER_ANALYSE_UTIL
#define GRAMMER_ANALYSE_UTIL

#include<stdio.h>
#include"word_scanner.h"
#include"grammer_util.h"

#define ERROR_INFO_NUMBER 7


typedef struct {
	int id;
	char* info;
}Error_info;

static Error_info Util_Error_all[]={//
	{ 1, "meet ERRTOKEN type, words illegel in function FetchToken  " },
	{ 2, "vary not exit" },
	{ 3, "do not recongined error" },
	{ 4, "not a legel Statement(in fucntion)" },
	{ 5, "not such a atom,(function Atom in grammer_util.c file)" },
	{ 6, "the error happen before call Component in function Factor in grammer_util.c \n" },
	{ 7, " there are not ' ; ' in function Program in grammer_util.c " }

};

Token FetchToken(FILE*);
int MatchToken(Token*, Token_Type, FILE*);
void SyntaxError(int);



#endif
