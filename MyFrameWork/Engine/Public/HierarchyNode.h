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
		// 애니메이션 데이터에서 넘겨주는 offsetMat는 전치해서 넘겨주기 때문에 다시 전치하고 세팅
		XMStoreFloat4x4(&mOffsetMatrix, XMMatrixTranspose(XMLoadFloat4x4(&offsetMat)));
	}
	void		Set_TransformMat(_fmatrix transformMat)
	{
		// 이전 부모까지 곱한 상태행렬 세팅
		XMStoreFloat4x4(&mTransformationMatrix, transformMat);
	}
private:
	// 자기 자신을 저장
	CHierarchyNode*			mParent = nullptr;
	char					mszName[MAX_PATH] = "";

	// 상태 행렬 정보
	_float4x4				mOffsetMatrix;					// 정점 파일에 정의된 기본 행렬
	_float4x4				mTransformationMatrix;			// 상태행렬
	_float4x4				mCombinedTransformationMatrix;	// 부모상태행렬을 곱한 최종 행렬

	// 계층의 깊이
	_uint					mDepth;

public:
	static CHierarchyNode* Create(const char* szName,
		_float4x4 mat, _uint depth, CHierarchyNode* parent);

	virtual void Free() override;
};

END