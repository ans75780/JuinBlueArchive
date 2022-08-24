#pragma once
#include "GameInstance.h"

BEGIN(Client)

class CStrUtil
{
private:
	CStrUtil();
	~CStrUtil();

public:
	static char* ConvertWCtoC(const wchar_t* str);
	static wchar_t* ConvertCtoWC(const char* str);

};

END

