#include "..\Public\Collider.h"
#include "Transform.h"

#include "DebugDraw.h"
#include "PipeLine.h"
#include "DebugDraw.h"

CCollider::CCollider(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{

}

CCollider::CCollider(const CCollider & rhs)
	: CComponent(rhs)
	, mBaseEffect(rhs.mBaseEffect)
	, mBatch(rhs.mBatch)
	, meType(rhs.meType)
	, mInputLayout(rhs.mInputLayout)
{
	// 복사시 새로 만들어지고 위치를 설정해준다.
	if (nullptr != rhs.mAABB)
		mAABB = new BoundingBox(*rhs.mAABB);
	if (nullptr != rhs.mOBB)
		mOBB = new BoundingOrientedBox(*rhs.mOBB);
	if (nullptr != rhs.mSphere)
		mSphere = new BoundingSphere(*rhs.mSphere);

	Safe_AddRef(mInputLayout);
	
}

HRESULT CCollider::NativeConstruct_Prototype(E_COLLIDER_TYPE eType)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	// 각 타입별로 크기 1인 충돌체 생성
	meType = eType;
	
	switch (meType)
	{
	case COL_AABB: 
		mAABB = new BoundingBox(_float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f));
		break;
	case COL_OBB:
		mOBB = new BoundingOrientedBox(_float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float4(0.f, 0.f, 0.f, 1.f));
		break;
	case COL_SPHERE:
		mSphere = new BoundingSphere(_float3(0.f, 0.f, 0.f), 1.f);
		break;
	}

	// 기본 셰이더와 Layout / Batch 생성
#ifdef _DEBUG
	mBaseEffect = new BasicEffect(m_pDevice);
	mBaseEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCode = nullptr;
	size_t		iShaderByteCodeLength = 0;

	mBaseEffect->GetVertexShaderBytecode(&pShaderByteCode, &iShaderByteCodeLength);

	FAILED_CHECK(m_pDevice->CreateInputLayout(DirectX::VertexPositionColor::InputElements, DirectX::VertexPositionColor::InputElementCount,
		pShaderByteCode, iShaderByteCodeLength, &mInputLayout));

	mBatch = new PrimitiveBatch<DirectX::VertexPositionColor>(m_pDeviceContext);
	
#endif // _DEBUG



	return S_OK;
}
HRESULT CCollider::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	_matrix		TransformMatrix;

	if (nullptr != pArg)
	{
		memcpy(&mColliderDesc, pArg, sizeof(COLLIDERDESC));
		TransformMatrix = XMMatrixAffineTransformation(
			XMLoadFloat3(&mColliderDesc.vScale), 
			XMVectorSet(0.f, 0.f, 0.f, 1.f),
			XMLoadFloat4(&mColliderDesc.vRotation),
			XMLoadFloat4(&mColliderDesc.vTranslation));
	}
	else
		TransformMatrix = XMMatrixIdentity();

	// 위치 설정
	if (nullptr != mAABB)
		mAABB->Transform(*mAABB, TransformMatrix);
	if (nullptr != mOBB)
		mOBB->Transform(*mOBB, TransformMatrix);
	if (nullptr != mSphere)
		mSphere->Transform(*mSphere, TransformMatrix);

	return S_OK;
}
void CCollider::SetScale(_float3 size)
{
	// 일단 대입방식
	if (nullptr != mAABB)
		mAABB->Extents = size;
	if (nullptr != mOBB)
		mOBB->Extents = size;
	if (nullptr != mSphere)
		mSphere->Radius = size.x;

}

#ifdef _DEBUG

HRESULT CCollider::Render(CTransform* trans)
{

	// 랜더링시 처리
	m_pDeviceContext->IASetInputLayout(mInputLayout);
	mBaseEffect->SetWorld(trans->GetWorldFloat4x4());

	CPipeLine*		pPipeLine = GetSingle(CPipeLine);

	mBaseEffect->SetView(XMLoadFloat4x4(&pPipeLine->GetTransformFloat4x4(CPipeLine::D3DTS_VIEW)));
	mBaseEffect->SetProjection(XMLoadFloat4x4(&pPipeLine->GetTransformFloat4x4(CPipeLine::D3DTS_PROJ)));

	mBaseEffect->Apply(m_pDeviceContext);

	// 배치 클래스의 Begin ~ End 까지 물체를 그린다.
	mBatch->Begin();

	if(nullptr != mAABB)
		DX::Draw(mBatch, *mAABB);
	if (nullptr != mOBB)
		DX::Draw(mBatch, *mOBB);
	if (nullptr != mSphere)		
		DX::Draw(mBatch, *mSphere);

	mBatch->End();

	return S_OK;
}
#endif // _DEBUG

CCollider * CCollider::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, E_COLLIDER_TYPE eType)
{
	CCollider*	pInstance = new CCollider(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(eType)))
	{
		MSGBOX("Failed to Creating CCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCollider::Clone(void * pArg)
{
	CCollider*	pInstance = new CCollider(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider::Free()
{
	__super::Free();

	Safe_Delete(mAABB);
	Safe_Delete(mOBB);
	Safe_Delete(mSphere);

	Safe_Release(mInputLayout);

#ifdef _DEBUG
	if (false == m_isCloned)
	{
		Safe_Delete(mBaseEffect);
		Safe_Delete(mBatch);
	}	
#endif // _DEBUG
}
