#pragma once
#include "Engine_Defines.h"

class CJson_Utility
{
public:
	static HRESULT		Load_Json(const wchar_t* _strPath, json* pOut);
	static HRESULT		Save_Json(const wchar_t* _strPath, json _json);

public:
	static XMFLOAT4		Get_VectorFromJson(json _json);
	static XMFLOAT4X4	Get_MatrixFromJson(json _json);

public:
	static wstring Complete_Path(const wchar_t* _strFileName);
};

