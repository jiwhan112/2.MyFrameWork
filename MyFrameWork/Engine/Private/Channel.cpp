#include "..\Public\Channel.h"
#include "HierarchyNode.h"

CChannel::CChannel()
{
}


HRESULT CChannel::NativeConstruct(const char* pName/*, CHierarchyNode* pNode*/)
{
	strcpy_s(m_szName, pName);

	//m_pNode = pNode;

	//Safe_AddRef(m_pNode);

	return S_OK;
}

void CChannel::Set_TransformationMatrix(_fmatrix TransformationMatrix)
{
	if (nullptr == m_pNode)
		return;

	m_pNode->Set_TransformationMatrix(TransformationMatrix);
}

void CChannel::Set_HierarchyNodePtr(CHierarchyNode * pNode)
{
	m_pNode = pNode;

	Safe_AddRef(m_pNode);
}



CChannel * CChannel::Create(const char* pName/*, CHierarchyNode* pNode*/)
{
	CChannel*	pInstance = new CChannel();

	if (FAILED(pInstance->NativeConstruct(pName/*, pNode*/)))
	{
		MSGBOX("Failed to Created CChannel");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CChannel::Free()
{
	for (auto& pKeyFrame : m_KeyFrames)
		Safe_Delete(pKeyFrame);

	m_KeyFrames.clear();

	Safe_Release(m_pNode);
}

