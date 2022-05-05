#pragma once

#include "Base.h"

/* 0. 하나의 노드를 대표하는 클래스 .*/
/* 1. 뼈들의 상속 구조를 표현해주고있는 노드들이다. */
/* 2. 노드 != 무조건 뼈의정보 , 초기상태를 정의해주기위한 행렬. */

BEGIN(Engine)

class CHierarchyNode final : public CBase
{
public:
	CHierarchyNode();
	virtual ~CHierarchyNode() = default;

public:
	_uint Get_Depth() const
	{
		return m_iDepth;
	}

	const char* Get_Name()
	{
		return m_szName;
	}

	_fmatrix Get_OffsetMatrix()
	{
		return XMLoadFloat4x4(&m_OffsetMatrix);
	}

	_fmatrix Get_CombinedTransformationMatrix()
	{
		return XMLoadFloat4x4(&m_CombinedTransformationMatrix);
	}

public:
	void Set_OffsetMatrix(_float4x4 OffsetMatrix)
	{
		XMStoreFloat4x4(&m_OffsetMatrix, XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));
	}

	void Set_TransformationMatrix(_fmatrix TransformationMatrix)
	{
		XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);
	}

public:
	HRESULT NativeConstruct(const char* pName, _float4x4 TransformationMatrix, _uint iDepth, CHierarchyNode* pParent);
	void Update_CombinedTransformationMatrix();

private:
	CHierarchyNode*				m_pParent = nullptr;
	char						m_szName[MAX_PATH] = "";
	_float4x4					m_OffsetMatrix;
	_float4x4					m_TransformationMatrix;
	_float4x4					m_CombinedTransformationMatrix;

	_uint						m_iDepth;
public:
	static CHierarchyNode* Create(const char* pName, _float4x4 TransformationMatrix, _uint iDepth, CHierarchyNode* pParent);
	virtual void Free() override;
};

END