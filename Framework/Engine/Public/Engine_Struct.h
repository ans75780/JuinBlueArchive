#pragma once

namespace Engine
{
	enum MOUSEBUTTON { MBS_LBUTTON, MBS_RBUTTON, MBS_WHEEL, MBS_X, MBS_END };
	enum MOUSEMOVE { MMS_X, MMS_Y, MMS_WHEEL, MMS_END };

	enum UNIT_CLASS{ UNIT_CLASS_FRONT, UNIT_CLASS_MIDDLE, UNIT_CLASS_BACK, UNIT_CLASS_END};
	enum UNIT_TYPE { UNIT_TYPE_STUDENT, UNIT_TYPE_ENEMY, UNIT_TYPE_BOSS, UNIT_TYPE_PROP, UNIT_TYPE_END };

	typedef struct tagRayDesc
	{
		_vector	vRayOrigin;
		_vector	vRayDir;
	}RAYDESC;

	typedef struct tagKeyFrame
	{
		XMFLOAT3		vScale;
		XMFLOAT4		vRotation;
		XMFLOAT3		vPosition;
		float			fTime;
	} KEYFRAME;

	//UI타입에 따라 업데이트 순서가 결정됨
	enum  UI_TYPE { UI_POST, UI_DIALOG_BUTTON, UI_DIALOG, UI_BUTTTON, UI_BACKGROUND, UI_END};


	typedef struct tagLightDesc
	{
		enum TYPE { TYPE_DIRECTIONAL, TYPE_POINT, TYPE_END };

		TYPE			eType = TYPE_END;

		XMFLOAT4		vDirection;

		XMFLOAT4		vPosition;
		float			fRange;

		XMFLOAT4		vDiffuse;
		XMFLOAT4		vAmbient;
		XMFLOAT4		vSpecular;

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
		class	CTexture*		pTextures[AI_TEXTURE_TYPE_MAX];
		char	szTextureNames	[MAX_PATH];

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


	typedef struct tagVertex_Position
	{
		XMFLOAT3		vPosition;
	}VTXPOS;

	typedef struct ENGINE_DLL tagVertex_Position_Declaration
	{
		static const unsigned int		iNumElements = 1;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXPOS_DECLARATION;

	

	typedef struct tagVertex_Instance
	{
		XMFLOAT4			vRight;
		XMFLOAT4			vUp;
		XMFLOAT4			vLook;
		XMFLOAT4			vTranslation;
	}VTXINSTANCE;

	typedef struct ENGINE_DLL tagVertex_Texture_Instance_Declaration
	{
		static const unsigned int		iNumElements = 6;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXTEX_INSTANCE_DECLARATION;


	typedef struct tagVertex_Point
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vPSize;
	}VTXPOINT;

	typedef struct ENGINE_DLL tagVertex_Point_Instance_Declaration
	{
		static const unsigned int		iNumElements = 6;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	}VTXPOINT_INSTANCE_DECLARATION;




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
