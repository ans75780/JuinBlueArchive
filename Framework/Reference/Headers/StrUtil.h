#pragma once

#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CStrUtil
{
public:
	CStrUtil();
	~CStrUtil();

public:
	static char* ConvertWCtoC(const wchar_t* str);
	static wchar_t* ConvertCtoWC(const char* str);

};

END