/*
���� :�����Դʷ��������ļǺ���
�������������ŵġ������������ߡ��﷨��
����������鲢�����﷨����
*/

#include <stdio.h>
#include <stdlib.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

extern void Parser(char * SrcFilePtr);
int main(int argc, char *argv[])
{
/*	if (argc<2){
		printf("Input Source! not gave source file path\n");
		system("PAUSE");
		return -1;
	}
	
	*/ 
	Parser("testsource.txt");// the grammer analyse fucntion
	system("PAUSE");
	return 0;
}
