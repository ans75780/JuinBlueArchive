#include "stdafx.h"
#include "Json_Utility.h"

#define	JSONFOLDER_PATH	L"../Json/"

HRESULT CJson_Utility::Load_Json(const wchar_t * _strPath, json * pOut)
{
	json JsonTemp;

	ifstream fin;

	fin.open(_strPath);

	if (fin.is_open())
	{
		fin >> (JsonTemp);
	}
	else
	{
		return E_FAIL;
	}

	fin.close();
	*pOut = JsonTemp;

	return S_OK;
}

HRESULT CJson_Utility::Save_Json(const wchar_t * _strPath, json _json)
{
	ofstream fout;

	fout.open(_strPath);

	if (fout.is_open())
	{
		fout << _json << endl;
	}
	else
		return E_FAIL;

	fout.close();

	return S_OK;
}

XMFLOAT4 CJson_Utility::Get_VectorFromJson(json _json)
{
	if (_json.empty())
		return XMFLOAT4(0.f, 0.f, 0.f, 1.f);

	return XMFLOAT4(_json[0], _json[1], _json[2], _json[3]);
}

XMFLOAT4X4 CJson_Utility::Get_MatrixFromJson(json _json)
{
	XMFLOAT4X4 matResult;

	if (_json.empty())
	{
		XMMATRIX	matTemp = XMMatrixIdentity();
		XMStoreFloat4x4(&matResult, matTemp);
	}
	else
	{
		for (int i = 0; i < 4; ++i)
			(*((XMFLOAT4*)&matResult.m[i])) = Get_VectorFromJson(_json[i]);
	}

	return matResult;
}

wstring CJson_Utility::Complete_Path(const wchar_t * _strFileName)
{
	wstring strFullPath = JSONFOLDER_PATH;
	strFullPath += _strFileName;
	strFullPath += L".json";

	return strFullPath;
}
