#include "..\Public\StrUtil.h"

CStrUtil::CStrUtil()
{
}


CStrUtil::~CStrUtil()
{
}

//wchar_t ���� char ���� ����ȯ �Լ�
char * CStrUtil::ConvertWCtoC(const wchar_t * str)
{
	//��ȯ�� char* ���� ����
	char* pStr;
	//�Է¹��� wchar_t ������ ���̸� ����
	int strSize = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	//char* �޸� �Ҵ�
	pStr = new char[strSize];
	//�� ��ȯ
	WideCharToMultiByte(CP_ACP, 0, str, -1, pStr, strSize, 0, 0);
	return pStr;
}

//char ���� wchar_t ���� ����ȯ �Լ�
wchar_t * CStrUtil::ConvertCtoWC(const char * str)
{
	//wchar_t�� ���� ����
	wchar_t* pStr;
	//��Ƽ ����Ʈ ũ�� ��� ���� ��ȯ
	int strSize = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, NULL);
	//wchar_t �޸� �Ҵ�
	pStr = new WCHAR[strSize];
	//�� ��ȯ
	MultiByteToWideChar(CP_ACP, 0, str, (int)strlen(str) + 1, pStr, strSize);
	return pStr;
}

wchar_t * CStrUtil::ConvertUTF8toWC(const char * str)
{
	wchar_t* pStr;

	int nLen = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, NULL);

	pStr = new WCHAR[nLen];

	MultiByteToWideChar(CP_UTF8, 0, str, (int)strlen(str) + 1, pStr, nLen);

	return pStr;
}

char * CStrUtil::ConvertWCtoUTF8(const wchar_t * str)
{
	char* pStr;

	int strSize = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
	
	pStr = new char[strSize];
	
	WideCharToMultiByte(CP_UTF8, 0, str, -1, pStr, strSize, 0, 0);
	
	return pStr;
}

