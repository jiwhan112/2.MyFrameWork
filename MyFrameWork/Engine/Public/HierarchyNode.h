#pragma once

#include "Base.h"

BEGIN(Engine)

class CHierarchyNode final : public CBase
{
public:
	CHierarchyNode(); 
	virtual ~CHierarchyNode() = default;

public:
	virtual HRESULT NativeConstruct(const char* szName, _float4x4 mat, _uint depth, CHierarchyNode* parent);
	void	Update_CombinedTransformMatrix();
public:
	_uint Get_Depth() const{ return mDepth; }
	const char* Get_Name() const { return mszName; }
	
	_fmatrix	Get_OffsetMat() const { return XMLoadFloat4x4(&mOffsetMatrix); }
	_fmatrix	Get_CombinedTransformMat() const { return XMLoadFloat4x4(&mCombinedTransformationMatrix); }

	
	void		Set_OffsetMat(_float4x4 offsetMat)
	{
		// �ִϸ��̼� �����Ϳ��� �Ѱ��ִ� offsetMat�� ��ġ�ؼ� �Ѱ��ֱ� ������ �ٽ� ��ġ�ϰ� ����
		XMStoreFloat4x4(&mOffsetMatrix, XMMatrixTranspose(XMLoadFloat4x4(&offsetMat)));
	}
	void		Set_TransformMat(_fmatrix transformMat)
	{
		// ���� �θ���� ���� ������� ����
		XMStoreFloat4x4(&mTransformationMatrix, transformMat);
	}
private:
	// �ڱ� �ڽ��� ����
	CHierarchyNode*			mParent = nullptr;
	char					mszName[MAX_PATH] = "";

	// ���� ��� ����
	_float4x4				mOffsetMatrix;					// ���� ���Ͽ� ���ǵ� �⺻ ���
	_float4x4				mTransformationMatrix;			// �������
	_float4x4				mCombinedTransformationMatrix;	// �θ��������� ���� ���� ���

	// ������ ����
	_uint					mDepth;

public:
	static CHierarchyNode* Create(const char* szName,
		_float4x4 mat, _uint depth, CHierarchyNode* parent);

	virtual void Free() override;
};

END