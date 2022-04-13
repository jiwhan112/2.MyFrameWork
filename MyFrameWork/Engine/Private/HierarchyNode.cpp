#include "..\Public\HierarchyNode.h"

CHierarchyNode::CHierarchyNode()
{

}



HRESULT CHierarchyNode::NativeConstruct(const char* szName, _float4x4 mat, _uint depth , CHierarchyNode* parent)
{
	// 초기화
	mParent = parent;
	Safe_AddRef(mParent);

	strcpy_s(mszName, szName);
	XMStoreFloat4x4(&mTransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&mat)));
	XMStoreFloat4x4(&mCombinedTransformationMatrix, XMMatrixIdentity());
	mDepth = depth;
	return S_OK;
}

void CHierarchyNode::Update_CombinedTransformMatrix()
{
	// 최종 행렬을 구한다.
	_matrix TransformMat = XMLoadFloat4x4(&mTransformationMatrix);
	_matrix Combinedmat ;

	if (mParent == nullptr)
	{
		// 최종행렬 = 자기자신
		Combinedmat = TransformMat;
	}
	else
	{
		// 최종행렬 = 부모의 행렬을 곱한 값
		Combinedmat = TransformMat * XMLoadFloat4x4(&mParent->mCombinedTransformationMatrix);
	}
}

CHierarchyNode * CHierarchyNode::Create(const char* szName,_float4x4 mat, _uint depth , CHierarchyNode* p)
{
	CHierarchyNode*	pInstance = NEW CHierarchyNode();

	if (FAILED(pInstance->NativeConstruct(szName, mat, depth,p)))
	{
		MSGBOX("Failed to Creating CHierarchyNode");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHierarchyNode::Free()
{
	Safe_Release(mParent);

}
