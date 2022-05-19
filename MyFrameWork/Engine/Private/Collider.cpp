#include "../Public/Collider.h"

#include "Transform.h"

#include "DebugDraw.h"
#include "PipeLine.h"
#include "DebugDraw.h"

#include "GameInstance.h"


CCollider::CCollider(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{
}

CCollider::CCollider(const CCollider & rhs)
	: CComponent(rhs)
#ifdef _DEBUG
	, mBaseEffect(rhs.mBaseEffect)
	, mBatch(rhs.mBatch)
	, mInputLayout(rhs.mInputLayout)
#endif // _DEBUG
	, meType(rhs.meType)
{
	// 복사시 새로 만들어지고 위치를 설정해준다.
	if (nullptr != rhs.mAABB)
		mAABB = new BoundingBox(*rhs.mAABB);
	if (nullptr != rhs.mOBB)
		mOBB = new BoundingOrientedBox(*rhs.mOBB);
	if (nullptr != rhs.mSphere)
		mSphere = new BoundingSphere(*rhs.mSphere);

#ifdef _DEBUG
	Safe_AddRef(mInputLayout);
#endif // _DEBUG
}

HRESULT CCollider::NativeConstruct_Prototype(E_COLLIDER_TYPE eType)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	// 각 타입별로 크기 1인 충돌체 생성
	meType = eType;
	const _float fDefault = 0.5f;
	switch (meType)
	{
	case COL_AABB:
		mAABB = new BoundingBox(_float3(0.f, 0.f, 0.f), _float3(fDefault, fDefault, fDefault));
		break;
	case COL_OBB:
		mOBB = new BoundingOrientedBox(_float3(0.f, 0.f, 0.f), _float3(fDefault, fDefault, fDefault), _float4(0.f, 0.f, 0.f, 1.f));
		break;
	case COL_SPHERE:
		mSphere = new BoundingSphere(_float3(0.f, 0.f, 0.f), fDefault);
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

HRESULT CCollider::Update_Transform(_float4x4 TransformMatrix)
{
	// 해당 컴포넌트 위치
	_float3 tasnData = TransformMatrix.Translation();
	tasnData += mOffset;
	if (nullptr != mAABB)
		mAABB->Center = tasnData;
	if (nullptr != mOBB)
		mOBB->Transform(*mOBB, TransformMatrix);
	if (nullptr != mSphere)
		mSphere->Center = tasnData;

	return S_OK;
}
bool CCollider::ColliderCheck(CCollider* TargetCollider)
{
	if (TargetCollider == nullptr || meType == CCollider::COL_END)
		return false;

	// 현재 들고있는 타입에 대해 충돌처리
	if (meType == CCollider::COL_AABB)
		return Update_AABB(TargetCollider);
	else if (meType == CCollider::COL_OBB)
		return Update_OBB(TargetCollider);
	else if (meType == CCollider::COL_SPHERE)
		return Update_SPHERE(TargetCollider);

	return false;
}
bool CCollider::ColliderCheck(_ray worldDIr, _float& dist)
{
	if (meType == CCollider::COL_AABB)
		return Update_AABB(worldDIr, dist);
	else if (meType == CCollider::COL_OBB)
		return Update_OBB(worldDIr, dist);
	else if (meType == CCollider::COL_SPHERE)
		return Update_SPHERE(worldDIr, dist);
}
void CCollider::Set_Scale(_float3 size)
{
	// 일단 대입방식
	if (nullptr != mAABB)
		mAABB->Extents = size;
	if (nullptr != mOBB)
		mOBB->Extents = size;
	if (nullptr != mSphere)
		mSphere->Radius = size.x;
}
void CCollider::Set_Offset(_float3 offset)
{
	mOffset = offset;
}


CCollider::OBBDESC CCollider::Get_Compute_OBBDesc()
{
	OBBDESC		OBBDesc;
	ZeroMemory(&OBBDesc, sizeof(OBBDESC));
	if (mOBB == nullptr)
		return OBBDesc;

	_float3		vPoint[8];
	mOBB->GetCorners(vPoint);

	// XYZ에 대한 방향 벡터
	XMStoreFloat3(&OBBDesc.vAlignAxis[0], XMVector3Normalize(XMLoadFloat3(&vPoint[2]) - XMLoadFloat3(&vPoint[3])));
	XMStoreFloat3(&OBBDesc.vAlignAxis[1], XMVector3Normalize(XMLoadFloat3(&vPoint[2]) - XMLoadFloat3(&vPoint[1])));
	XMStoreFloat3(&OBBDesc.vAlignAxis[2], XMVector3Normalize(XMLoadFloat3(&vPoint[2]) - XMLoadFloat3(&vPoint[6])));

	// 중심에서 각 면을 향하는 벡터를 구한다.
	XMStoreFloat3(&OBBDesc.vCenterAxis[0], (XMLoadFloat3(&vPoint[2]) + XMLoadFloat3(&vPoint[5])) * 0.5f - XMLoadFloat3(&mOBB->Center));
	XMStoreFloat3(&OBBDesc.vCenterAxis[1], (XMLoadFloat3(&vPoint[2]) + XMLoadFloat3(&vPoint[7])) * 0.5f - XMLoadFloat3(&mOBB->Center));
	XMStoreFloat3(&OBBDesc.vCenterAxis[2], (XMLoadFloat3(&vPoint[2]) + XMLoadFloat3(&vPoint[0])) * 0.5f - XMLoadFloat3(&mOBB->Center));

	return OBBDesc;
}

#ifdef _DEBUG
HRESULT CCollider::Render()
{
	if (GetSingle(CGameInstance)->Get_IsColliderRender() == false)
		return S_OK;

	// 랜더링시 처리
	m_pDeviceContext->IASetInputLayout(mInputLayout);
	//	mBaseEffect->SetWorld(trans->GetWorldFloat4x4());

	CPipeLine*		pPipeLine = GetSingle(CPipeLine);
	mBaseEffect->SetWorld(_float4x4::Identity);
	mBaseEffect->SetView(XMLoadFloat4x4(&pPipeLine->GetTransformFloat4x4(CPipeLine::D3DTS_VIEW)));
	mBaseEffect->SetProjection(XMLoadFloat4x4(&pPipeLine->GetTransformFloat4x4(CPipeLine::D3DTS_PROJ)));

	mBaseEffect->Apply(m_pDeviceContext);

	// 배치 클래스의 Begin ~ End 까지 물체를 그린다.
	mBatch->Begin();

	if (nullptr != mAABB)
		DX::Draw(mBatch, *mAABB, DirectX::Colors::Green);
	if (nullptr != mOBB)
		DX::Draw(mBatch, *mOBB, DirectX::Colors::Green);
	if (nullptr != mSphere)
		DX::Draw(mBatch, *mSphere, DirectX::Colors::Blue);

	mBatch->End();

	return S_OK;
}
#endif // _DEBUG

bool CCollider::Update_AABB(CCollider * TargetCollider)
{
	// Intersects
	BoundingBox* targetAABB = TargetCollider->Get_Collider_AABB();
	BoundingOrientedBox* targetOBB = TargetCollider->Get_Collider_OBB();
	BoundingSphere* targetSPHERE = TargetCollider->Get_Collider_SPHERE();

	// 각 타입에 따라 AABB와 충돌연산
	if (targetAABB != nullptr)
		return  mAABB->Intersects(*targetAABB);

	else if (targetOBB != nullptr)
		return  mAABB->Intersects(*targetOBB);

	else if (targetSPHERE != nullptr)
		return  mAABB->Intersects(*targetSPHERE);

	return false;
}
bool CCollider::Update_OBB(CCollider * TargetCollider)
{
	// Intersects
	BoundingBox* targetAABB = TargetCollider->Get_Collider_AABB();
	BoundingOrientedBox* targetOBB = TargetCollider->Get_Collider_OBB();
	BoundingSphere* targetSPHERE = TargetCollider->Get_Collider_SPHERE();

	if (targetAABB != nullptr)
		return  mOBB->Intersects(*targetAABB);

	else if (targetOBB != nullptr)
		return  mOBB->Intersects(*targetOBB);

	else if (targetSPHERE != nullptr)
		return  mOBB->Intersects(*targetSPHERE);

	return false;

	return false;
}
bool CCollider::Update_SPHERE(CCollider * TargetCollider)
{
	// Intersects
	BoundingBox* targetAABB = TargetCollider->Get_Collider_AABB();
	BoundingOrientedBox* targetOBB = TargetCollider->Get_Collider_OBB();
	BoundingSphere* targetSPHERE = TargetCollider->Get_Collider_SPHERE();

	//	mSphere->Intersects(rr.position, rr.direction, dist);

	if (targetAABB != nullptr)
		return  mSphere->Intersects(*targetAABB);

	else if (targetOBB != nullptr)
		return  mSphere->Intersects(*targetOBB);

	else if (targetSPHERE != nullptr)
		return  mSphere->Intersects(*targetSPHERE);

	return false;
}
bool CCollider::Update_AABB(_ray RatDIr,_float& dist)
{

	return mAABB->Intersects(RatDIr.position, RatDIr.direction, dist);

}
bool CCollider::Update_OBB(_ray RatDIr, _float& dist)
{
	return mOBB->Intersects(RatDIr.position, RatDIr.direction, dist);

}
bool CCollider::Update_SPHERE(_ray RatDIr, _float& dist)
{
	return mSphere->Intersects(RatDIr.position, RatDIr.direction, dist);

}
bool CCollider::Update_MY_AABB(CCollider * TargetCollider)
{
	BoundingBox* Target_AABB = TargetCollider->Get_Collider_AABB();
	if (nullptr == mAABB ||
		nullptr == Target_AABB)
		return false;

	_float3		vSourMin, vSourMax;
	_float3		vDestMin, vDestMax;

	// A의 Min Max
	vSourMin = _float3(mAABB->Center.x - mAABB->Extents.x,
		mAABB->Center.y - mAABB->Extents.y,
		mAABB->Center.z - mAABB->Extents.z
	);

	vSourMax = _float3(mAABB->Center.x + mAABB->Extents.x,
		mAABB->Center.y + mAABB->Extents.y,
		mAABB->Center.z + mAABB->Extents.z
	);

	// B의 Min Max
	vDestMin = _float3(Target_AABB->Center.x - Target_AABB->Extents.x,
		Target_AABB->Center.y - Target_AABB->Extents.y,
		Target_AABB->Center.z - Target_AABB->Extents.z
	);

	vDestMax = _float3(Target_AABB->Center.x + Target_AABB->Extents.x,
		Target_AABB->Center.y + Target_AABB->Extents.y,
		Target_AABB->Center.z + Target_AABB->Extents.z
	);

	bool mCol = true;

	// 충돌이 아닌 영역 비교
	if (max(vSourMin.x, vDestMin.x) > min(vSourMax.x, vDestMax.x))
		mCol = false;

	if (max(vSourMin.y, vDestMin.y) > min(vSourMax.y, vDestMax.y))
		mCol = false;

	if (max(vSourMin.z, vDestMin.z) > min(vSourMax.z, vDestMax.z))
		mCol = false;

	return mCol;
}
bool CCollider::Update_MY_OBB(CCollider * TargetCollider)
{
	OBBDESC obbdesc[2];
	obbdesc[0] = Get_Compute_OBBDesc();
	obbdesc[1] = TargetCollider->Get_Compute_OBBDesc();

	bool bCol = true;

	_float distance[3];

	for (_uint i = 0; i < 2; ++i)
	{
		for (_uint j = 0; j < 3; ++j)
		{
			// #TODO OBB 구현
			//distance[0] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterAxis[0]), XMLoadFloat3(&OBBDesc[i].vAlignAxis[j])))) +
			//	fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterAxis[1]), XMLoadFloat3(&OBBDesc[i].vAlignAxis[j])))) +
			//	fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterAxis[2]), XMLoadFloat3(&OBBDesc[i].vAlignAxis[j]))));

			//distance[1] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterAxis[0]), XMLoadFloat3(&OBBDesc[i].vAlignAxis[j])))) +
			//	fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterAxis[1]), XMLoadFloat3(&OBBDesc[i].vAlignAxis[j])))) +
			//	fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterAxis[2]), XMLoadFloat3(&OBBDesc[i].vAlignAxis[j]))));

			//distance[2] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&pTargetCollider->m_pOBB[BOUNDING_TRANSFORM]->Center) -
			//	XMLoadFloat3(&m_pOBB[BOUNDING_TRANSFORM]->Center), XMLoadFloat3(&OBBDesc[i].vAlignAxis[j]))));

			if (distance[0] + distance[1] < distance[2])
			{
				bCol = false;
				break;
			}
		}
		if (false == bCol)
			break;
	}
	return bCol;
}

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

#ifdef _DEBUG
	Safe_Release(mInputLayout);

	if (false == m_isCloned)
	{
		Safe_Delete(mBaseEffect);
		Safe_Delete(mBatch);
	}
#endif // _DEBUG
}