#pragma once

namespace Engine
{
	enum MOUSEBUTTON { MBS_LBUTTON, MBS_RBUTTON, MBS_WHEEL, MBS_X, MBS_END };
	enum MOUSEMOVE { MMS_X, MMS_Y, MMS_WHEEL, MMS_END };

	enum  UI_TYPE { CANVAS_POST, CANVAS_END };
	typedef struct tagLightDesc
	{
		enum TYPE {TYPE_DIRECTIONAL, TYPE_POINT, TYPE_END};

		TYPE		eType = TYPE_END;

		XMFLOAT3	vDirection;//�𷺼ų� �����ÿ��� �ʿ���.

		XMFLOAT3	vPoint;//����Ʈ �����ÿ� �ʿ���.
		float		fRange;

		XMFLOAT4	vDiffuse;//�� ��ü�� ������ �ǹ���.(��� ������ ���ٸ� 1,1,1,1)
		XMFLOAT4	vAmbient;

	}LIGHTDESC;
	
	typedef struct tagMaterialDesc
	{
		XMFLOAT4		vDiffuse;//Ư�� ������ ����.
		XMFLOAT4		vAmbient;

	}MTRLDESC;

	typedef struct tagLineIndices32
	{
		unsigned long _1, _2;
	}LINEINDICES32;

	typedef struct tagLineIndices16
	{
		unsigned short _1, _2;
	}LINEINDICES16;

	typedef struct tagFaceIndices32
	{
		unsigned long _1, _2, _3;
	}FACEINDICES32;

	typedef struct tagFaceIndices16
	{
		unsigned short _1, _2, _3;
	}FACEINDICES16;


	typedef struct tagVertex_Texture
	{		
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexUV;				
	}VTXTEX;	

	typedef struct ENGINE_DLL tagVertex_Texture_Declaration
	{
		static const unsigned int		iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXTEX_DECLARATION;
	
	typedef struct tagVertex_Normal_Texture
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexUV;
	}VTXNORTEX;

	typedef struct ENGINE_DLL tagVertex_Normal_Texture_Declaration
	{
		static const unsigned int		iNumElements = 3;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXNORTEX_DECLARATION;

	typedef struct tagVertex_Cube_Texture
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vTexUV;
	}VTXCUBETEX;

	typedef struct ENGINE_DLL tagVertex_Cube_Texture_Declaration
	{
		static const unsigned int		iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXCUBETEX_DECLARATION;

	typedef struct tagGraphicDesc
	{
		enum WINMODE { MODE_FULL, MODE_WIN, MODE_END };

		unsigned int iWinCX, iWinCY; 
		HWND	hWnd;
		WINMODE	isWindowMode;
	}GRAPHICDESC;
}
