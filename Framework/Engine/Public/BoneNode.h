#pragma once
#include "Base.h"

BEGIN(Engine)

class CBoneNode : public CBase
{
private:
	CBoneNode();
	virtual ~CBoneNode()  = default;

public:
	_uint Get_Depth() const {
		return m_iDepth;
	}

	const char* Get_Name() const {
		return m_szName;
	}

	void Set_OffsetMatrix(_fmatrix OffsetMatrix) {
		XMStoreFloat4x4(&m_OffsetMatrix, OffsetMatrix);
	}

	void Set_TransformationMatrix(_fmatrix TransformationMatrix) {
		XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);
	}

public:
	HRESULT Initialize(aiNode* pAINode, class CBoneNode* pParent, _uint iDepth);
private:
	_uint			m_iDepth = 0;
	char			m_szName[MAX_PATH] = "";

	_float4x4		m_OffsetMatrix;
	_float4x4		m_TransformationMatrix;
	_float4x4		m_CombinedTransformationMatrix;

	CBoneNode *	m_pParent = nullptr;


public:
	static CBoneNode* Create(aiNode* pAINode, CBoneNode *pParent, _uint iDepth);
	virtual void Free() override;

};

END
