#include "..\Public\HierarchyNode.h"

CHierarchyNode::CHierarchyNode()
{

}



HRESULT CHierarchyNode::NativeConstruct(const char* szName, _float4x4 mat, _uint depth , CHierarchyNode* parent)
{
	// �ʱ�ȭ
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
	// ���� ����� ���Ѵ�.
	_matrix TransformMat = XMLoadFloat4x4(&mTransformationMatrix);
	_matrix Combinedmat ;

	if (mParent == nullptr)
	{
		// ������� = �ڱ��ڽ�
		Combinedmat = TransformMat;
	}
	else
	{
		// ������� = �θ��� ����� ���� ��
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
