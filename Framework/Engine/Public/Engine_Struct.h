#pragma once

namespace Engine
{
	enum MOUSEBUTTON { MBS_LBUTTON, MBS_RBUTTON, MBS_WHEEL, MBS_X, MBS_END };
	enum MOUSEMOVE { MMS_X, MMS_Y, MMS_WHEEL, MMS_END };

	//UI타입에 따라 업데이트 순서가 결정됨
	enum  UI_TYPE { UI_POST, UI_DIALOG_BUTTON, UI_DIALOG, UI_BUTTTON, UI_BACKGROUND, UI_END};
	typedef struct tagLightDesc
	{
		enum TYPE {TYPE_DIRECTIONAL, TYPE_POINT, TYPE_END};

		TYPE		eType = TYPE_END;

		XMFLOAT3	vDirection;//디렉셔널 라이팅에만 필요함.

		XMFLOAT3	vPoint;//포인트 라이팅에 필요함.
		float		fRange;

		XMFLOAT4	vDiffuse;//빛 자체의 색상을 의미함.(흰색 광원이 들어갔다면 1,1,1,1)
		XMFLOAT4	vAmbient;

	}LIGHTDESC;
	
	typedef struct tagMaterialDesc
	{
		XMFLOAT4		vDiffuse;//특정 색상의 재질.
		XMFLOAT4		vAmbient;
		XMFLOAT4		vSpecular;
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

	typedef struct tagModelMaterial
	{
		class CTexture*		pTextures[AI_TEXTURE_TYPE_MAX];
	}MODEL_MATERIAL;

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

	typedef struct ENGINE_DLL tagVertex_Normal_Texture_Declaration
	{
		static const unsigned int		iNumElements = 3;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXNORTEX_DECLARATION;


	
	typedef struct tagVertex_AnimModel	//8.10 추가
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexUV;
		XMFLOAT3		vTangent;
		XMUINT4			vBlendIndex;
		XMFLOAT4		vBlendWeight;
	}VTXANIM;

	typedef struct ENGINE_DLL tagVertex_AnimModel_Declaration  //8.10 추가
	{
		static const unsigned int		iNumElements = 6;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXANIM_DECLARATION;

	typedef struct tagVertex_Model	//8.10 추가
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexUV;
		XMFLOAT3		vTangent;
	}VTXMODEL;

	typedef struct ENGINE_DLL tagtagVertex_Model_Declaration	//8.10 추가
	{
		static const unsigned int		iNumElements = 4;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXMODEL_DECLARATION;

	typedef struct tagVertex_Normal_Texture
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexUV;
	}VTXNORTEX;



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
