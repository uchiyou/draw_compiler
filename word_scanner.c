#include"word_scanner.h"
#include<stdio.h>
#include<string.h>
#include<ctype.h>// judge a char is or not a english char or number, replace the next line header
//#include<bits/locale_facets.h>

void getTokenLexeme(FILE*, char*, int);
double getDigit(FILE*, char*, int);

//------------the child function


FILE* InitScanner(char* file_path){

	FILE* file = fopen(file_path, "r");
	//FILE* file;
	//fopen_s(&file, file_path, "r");
	return file;

}




Token GetToken(FILE* file){ // got a ken from a file

	Token token = { ERRTOKEN, "", 0.0, NULL }; // 用于返回记号
	int len = TOKEN_LEN;
	//token.lexeme = tokenBuffer;	// 记号的字符指针指向字符缓冲区


	char aChar = fgetc(file);	// 从源文件中读取一个字符


	while (aChar == ' ' || aChar == '\t' || aChar == '\n' || aChar == '\r'){		// 空格、TAB、回车等字符的过滤
		aChar = fgetc(file);
	}


	if (aChar == EOF){
		printf("\n---the end of source file (function GetToken in word_scanner.c)\n");
		token.type = NONTOKEN;
		return token;
	}
	// AddInTokenString (aChar);	// 将读入的字符放进缓冲区TokenBuffer中


	if (isalpha(aChar)){     //  识别ID, isalpha()  : judge a  char is or not a English char
		token.lexeme[0] = aChar;
		getTokenLexeme(file, token.lexeme, len);

		// fill the token accord token.lexeme
		int i = 0;
		for (; i<TOKENTAB_NUM; i++)
		if (strcasecmp((token.lexeme), (TokenTab[i].lexeme)) == 0){// 字符匹配时忽略大小写 
			token = TokenTab[i];
		}


	}
	else if (isdigit(aChar))  {  // 识别数字常量
		token.lexeme[0] = aChar;
		token.type = CONST_ID;
		token.value = getDigit(file, token.lexeme, len);
	}
	else {

		switch (aChar) {
		case ';':
			token.type = SEMICO;
			token.lexeme[0] = ';';
			token.lexeme[1] = '\0';
			break;
		case '(':

			token.type = L_BRACKET;
			token.lexeme[0] = '(';
			token.lexeme[1] = '\0';
			break;
		case ')':
			token.type = R_BRACKET;
			token.lexeme[0] = ')';
			token.lexeme[1] = '\0';
			break;
		case '+':
			token.type = PLUS;
			token.lexeme[0] = '+';
			token.lexeme[1] = '\0';
			break;
		case ',':
			token.type = COMMA;
			token.lexeme[0] = ',';
			token.lexeme[1] = '\0';
			break;
		case '*':
			// judge it is  * or **
			token.lexeme[0] = '*';
			if ((aChar = fgetc(file)) == '*'){
				token.type = POWER;
				token.lexeme[1] = '*';
				token.lexeme[2] = '\0';
				break;
			}
			else{
				token.type = MUL;
				token.lexeme[1] = '\0';
				fseek(file, -1, SEEK_CUR);
				break;
			}
		case '-':
			// judge it is - or --
		//	token.lexeme[0] = '-';     注释符号 不需要传递给 语法分析器 
			if ((aChar = fgetc(file)) == '-'){
			//	token.type = COMMENT;
			//	token.lexeme[1] = '-';
			//	token.lexeme[2] = '\0';
				while (fgetc(file) != '\n');// neglect the comment, move to next line
				token=GetToken(file);
				break;
			}
			else{
				token.lexeme[0] = '-';
				token.type = MINUS;
				token.lexeme[1] = '\0';
				fseek(file, -1, SEEK_CUR);
				break;
			}
		case '/':
			// 	judge it is / or //
			//token.lexeme[0] = '/';   the comment should not be a token to grammer
			if ((aChar = fgetc(file)) == '/'){
			//	token.type = COMMENT;
				//token.lexeme[1] = '/';
			//	token.lexeme[2] = '\0';
				while ((fgetc(file)) != '\n');// neglect the comment
				token=GetToken(file);
				break;
			}
			else{
				token.lexeme[0] = '/';
				token.type = DIV;
				token.lexeme[1] = '\0';
				fseek(file, -1, SEEK_CUR);
				break;
			}
		default:
			token.type = ERRTOKEN;

		}

	}

    print_state(token);
	return token;
}




void CloseScanner(FILE* file){
	fclose(file);
}
void getTokenLexeme(FILE* file, char* token_buf, int buf_len){// cut token lexeme from file into a token
	int i = 1;
	char achar = '\0';
	achar = fgetc(file);
	while (isalpha(achar) || isdigit(achar)){
		if (i<buf_len)
			token_buf[i++] = achar;
		achar = fgetc(file);
	}
	fseek(file, -1, SEEK_CUR);//  here coming back for miss next char , because the light icon slide a char len
	token_buf[i] = '\0';
}






double getDigit(FILE* file, char* token_buf, int buf_len){//  cut a string from the file into a double
	int i = 1, flag = 1;
	char achar;

	achar = fgetc(file);
	while (isdigit(achar) || achar == '.'){



		if (i<buf_len)
			token_buf[i++] = achar;

		if (achar == '.'){// there are only one point in a float number
			if (flag == 0){
				printf("here two ' . ' in number\n");
			}
			flag = 0;
		}

		achar = fgetc(file);
	}
	fseek(file, -1, SEEK_CUR);
	token_buf[i] = '\0';
	//	printf("-----%s\n",token_buf);
	return toDouble(token_buf, i);
}
double toDouble(char* str, int len){ // transfer string into double, here no check for string
	int i = 0, flag = 0;
	double temp_double = 1;
	double temp = 0;

	for (; i<len; i++){
		if (str[i] == '.'){
			flag = 1;
			continue;
		}

		if (flag == 0)
			temp = temp * 10 + str[i] - '0';
		else{
			temp_double *= 10;
			temp += ((double)(str[i] - '0')) / temp_double;
		}
	}
//	printf("--- %12f in toDouble word_scanner.c\n",temp);
	return temp;
}

