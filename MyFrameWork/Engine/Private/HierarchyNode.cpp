#include "..\Public\HierarchyNode.h"

CHierarchyNode::CHierarchyNode()
{
}

HRESULT CHierarchyNode::NativeConstruct(const char* pName, _float4x4 TransformationMatrix, _uint iDepth, CHierarchyNode* pParent)
{
	m_pParent = pParent;
	Safe_AddRef(m_pParent);

	strcpy_s(m_szName, pName);
	XMStoreFloat4x4(&m_OffsetMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&TransformationMatrix)));
	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());
	m_iDepth = iDepth;

	return S_OK;
}

void CHierarchyNode::Update_CombinedTransformationMatrix()
{
	_matrix TransformationMatrix = XMLoadFloat4x4(&m_TransformationMatrix);
	_matrix CombinedTransformationMatrix;

	if (nullptr == m_pParent)
		CombinedTransformationMatrix = TransformationMatrix;
	else
		CombinedTransformationMatrix = TransformationMatrix * XMLoadFloat4x4(&m_pParent->m_CombinedTransformationMatrix);

	XMStoreFloat4x4(&m_CombinedTransformationMatrix, CombinedTransformationMatrix);
}

CHierarchyNode * CHierarchyNode::Create(const char* pName, _float4x4 TransformationMatrix, _uint iDepth, CHierarchyNode* pParent)
{
	CHierarchyNode*	pInstance = NEW CHierarchyNode();

	if (FAILED(pInstance->NativeConstruct(pName, TransformationMatrix, iDepth, pParent)))
	{
		MSGBOX("Failed to Created CHierarchyNode");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHierarchyNode::Free()
{
	Safe_Release(m_pParent);
}