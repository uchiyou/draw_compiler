#include"word_scanner.h"
#include"grammer_analyse_util.h"
#include"grammer_util.h"

/*      Parser ��������һһ��Դ�ļ� �� Program �������� Դ�ļ���ĳ�������,�����ﴦ������Դ�ļ��ĳ���    */
void Parser(char * SrcFilePtr) // the key function for the grammer analyser
{
	Token token;
	FILE* file;

	if (!(file = InitScanner(SrcFilePtr))){// ��ʼ���ʷ�������{ printf("Open Source File Error ! \n");
		printf("init word scanner failed\n");
		system("PAUSE");
		return;
	}

	token= FetchToken(file);//��ȡ��һ���Ǻ� 
	Program(token, file);		// ����ݹ��½�����, �������ڴ˺����н��� �����ط������ĸ��ڵ� 
	CloseScanner(file);	// �رմʷ�������


}

