#include "..\Public\Collider.h"
#include "DebugDraw.h"
#include "PipeLine.h"

CCollider::CCollider(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{

}

CCollider::CCollider(const CCollider & rhs)
	: CComponent(rhs)
	, m_pBasicEffect(rhs.m_pBasicEffect)
	, m_pBatch(rhs.m_pBatch)
	, m_eType(rhs.m_eType)	
	, m_pInputLayout(rhs.m_pInputLayout)
{
	if (nullptr != rhs.m_pAABB)
		m_pAABB = new BoundingBox(*rhs.m_pAABB);
	if (nullptr != rhs.m_pOBB)
		m_pOBB = new BoundingOrientedBox(*rhs.m_pOBB);
	if (nullptr != rhs.m_pSphere)
		m_pSphere = new BoundingSphere(*rhs.m_pSphere);

	Safe_AddRef(m_pInputLayout);
	
}

HRESULT CCollider::NativeConstruct_Prototype(TYPE eType)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	m_eType = eType;

	/* 중심점은 원점. 모든 사이즈는 1로. */

	switch (m_eType)
	{
	case TYPE_AABB:
		m_pAABB = new BoundingBox(_float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f));
		break;
	case TYPE_OBB:
		m_pOBB = new BoundingOrientedBox(_float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float4(0.f, 0.f, 0.f, 1.f));
		break;
	case TYPE_SPHERE:
		m_pSphere = new BoundingSphere(_float3(0.f, 0.f, 0.f), 1.f);
		break;
	}

#ifdef _DEBUG
	m_pBasicEffect = new BasicEffect(m_pDevice);
	m_pBasicEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCode = nullptr;
	size_t		iShaderByteCodeLength = 0;

	m_pBasicEffect->GetVertexShaderBytecode(&pShaderByteCode, &iShaderByteCodeLength);

	if (FAILED(m_pDevice->CreateInputLayout(DirectX::VertexPositionColor::InputElements, DirectX::VertexPositionColor::InputElementCount,
		pShaderByteCode, iShaderByteCodeLength, &m_pInputLayout)))
		return E_FAIL;

	m_pBatch = new PrimitiveBatch<DirectX::VertexPositionColor>(m_pDeviceContext);
	
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
		memcpy(&m_ColliderDesc, pArg, sizeof(COLLIDERDESC));
		TransformMatrix = XMMatrixAffineTransformation(XMLoadFloat3(&m_ColliderDesc.vScale), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMLoadFloat4(&m_ColliderDesc.vRotation), XMLoadFloat4(&m_ColliderDesc.vTranslation));
	}
	else
		TransformMatrix = XMMatrixIdentity();

	 

	if (nullptr != m_pAABB)
		m_pAABB->Transform(*m_pAABB, TransformMatrix);
	if (nullptr != m_pOBB)
		m_pOBB->Transform(*m_pOBB, TransformMatrix);
	if (nullptr != m_pSphere)
		m_pSphere->Transform(*m_pSphere, TransformMatrix);

	return S_OK;
}
#ifdef _DEBUG

HRESULT CCollider::Render()
{
	m_pDeviceContext->IASetInputLayout(m_pInputLayout);

	m_pBasicEffect->SetWorld(XMMatrixIdentity());

	CPipeLine*		pPipeLine = GET_INSTANCE(CPipeLine);

	m_pBasicEffect->SetView(XMLoadFloat4x4(&pPipeLine->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW)));
	m_pBasicEffect->SetProjection(XMLoadFloat4x4(&pPipeLine->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ)));

	RELEASE_INSTANCE(CPipeLine);

	m_pBasicEffect->Apply(m_pDeviceContext);

	m_pBatch->Begin();

	if(nullptr != m_pAABB)
		DX::Draw(m_pBatch, *m_pAABB);
	if (nullptr != m_pOBB)
		DX::Draw(m_pBatch, *m_pOBB);
	if (nullptr != m_pSphere)		
		DX::Draw(m_pBatch, *m_pSphere);

	m_pBatch->End();

	return S_OK;
}
#endif // _DEBUG

CCollider * CCollider::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, TYPE eType)
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

	Safe_Delete(m_pAABB);
	Safe_Delete(m_pOBB);
	Safe_Delete(m_pSphere);

	Safe_Release(m_pInputLayout);

#ifdef _DEBUG
	if (false == m_isCloned)
	{
		Safe_Delete(m_pBasicEffect);
		Safe_Delete(m_pBatch);
	}	
#endif // _DEBUG
}
