#include "..\Public\Channel.h"
#include "HierarchyNode.h"

CChannel::CChannel()
{

}

HRESULT CChannel::NativeConstruct(const char * pName, CHierarchyNode * pNode)
{
	strcpy_s(mSzName, pName);
	mHierNode = pNode;
	Safe_AddRef(mHierNode);

	return S_OK;
}

void CChannel::Set_TransformationMat(_fmatrix transform)
{
	if (mHierNode == nullptr)
		return;

	mHierNode->Set_TransformMat(transform);
}


CChannel * CChannel::Create(const char * pName, CHierarchyNode * pNode)
{
	CChannel*	pInstance = NEW CChannel();

	if (FAILED(pInstance->NativeConstruct(pName, pNode)))
	{
		MSGBOX("Failed to Creating CChannel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChannel::Free()
{
	for (auto& pKey: mVectorKeyFrame)
	{
		Safe_Delete(pKey);
	}
	mVectorKeyFrame.clear();
	Safe_Release(mHierNode);

}
