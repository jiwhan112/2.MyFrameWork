#include "stdafx.h"
#include "System/ColliderManager.h"
#include "GameObject/Client_Object.h"
#include "Collider.h"

CColliderManager::CColliderManager()
{
}

HRESULT CColliderManager::NativeConstruct_Prototype()
{
	mWorldPickPos = _float3(0, 0, 0);
	mIsMousePick = false;
	return S_OK;
}

HRESULT CColliderManager::Tick_ColliderCheck(_double Timer)
{
	// 1. 마우스 충돌체크
	ColCheck_MOUSE();

	// 2. 객체들의 충돌 체크

	// 오브젝트 지우기
	ReleaseObjects();

	return S_OK;
}

HRESULT CColliderManager::Add_ColliderObject(E_COLLIDEROBJ_TYPE e, CGameObject_Base * col)
{
	Safe_AddRef(col);
	mListColliders[e].push_back(col);

	return S_OK;
}

HRESULT CColliderManager::ReleaseObjects()
{
	for (auto& colliderObjs : mListColliders)
	{
		for (auto& pGameObject : colliderObjs)
			Safe_Release(pGameObject);
		colliderObjs.clear();

	}
	return S_OK;
}

HRESULT CColliderManager::ColCheck_OBJECTS()
{
	// 오브젝트 끼리의 충돌
	return S_OK;
}

bool CColliderManager::Check_Navi(_float4x4 WorldMatrixInverse, CNavigation * navi)
{
	if (navi == nullptr)
		return false;

	CGameInstance* pInsance = GetSingle(CGameInstance);


	// 네비게이션 메시와 충돌체크.
	pInsance->Transform_ToLocalSpace(WorldMatrixInverse);
	_bool		isPick = false;

	// 네비 메시 정점이랑 충돌
	for (auto cell : *(navi->Get_CellVector()))
	{
		_float3		vPoint[3] = {
			cell->Get_Point(CCell::POINT_A),
			cell->Get_Point(CCell::POINT_B),
			cell->Get_Point(CCell::POINT_C)
		};

		if (isPick = pInsance->isPick(vPoint, &mWorldPickPos))
		{
			_float4x4		WorldMatrix;
			WorldMatrix = WorldMatrixInverse.Invert();

			// 월드 변환
			_float3::Transform(mWorldPickPos, WorldMatrix, mWorldPickPos);
			return isPick;
		}
	}

	return false;
}

bool CColliderManager::Check_Mouse_Object(_ray worldRay)
{

	for (auto& obj : mListColliders[COLLIDEROBJ_STATIC])
	{
		CGameObject_3D_Static* staticobj = static_cast<CGameObject_3D_Static*>(obj);
		if (staticobj == nullptr)
			continue;

		// 충돌체랑 체크
		CCollider* ComCollider = staticobj->Get_ComCollider();
		_float dist = -1;
		if (ComCollider->ColliderCheck(worldRay, dist))
		{
			staticobj->CollisionFunc(mWorldPickPos, dist);
			return true;
		}
	}

	for (auto& obj : mListColliders[COLLIDEROBJ_DYNAMIC])
	{
		CGameObject_3D_Dynamic* dynamicobj = static_cast<CGameObject_3D_Dynamic*>(obj);
		if (dynamicobj == nullptr)
			continue;

		// 콜라이더 리스트와 충돌체크
		auto listCollider =  dynamicobj->Get_ComListCollider();
		if (listCollider == nullptr)
			continue;

		
		for (auto& col : *listCollider)
		{
			_float dist = -1;
			_uint ColIndex = 0;
			if (col->ColliderCheck(worldRay, dist))
			{
				dynamicobj->CollisionFunc(mWorldPickPos, dist, ColIndex);
				ColIndex++;
				return true;
			}
		}
	}
	return false;
}

bool CColliderManager::Check_Mouse_Terrain()
{
	for (auto& obj : mListColliders[COLLIDEROBJ_TERRAIN])
	{
		CGameObject_MyTerrain* terrain = static_cast<CGameObject_MyTerrain*>(obj);
		if (terrain == nullptr)
			continue;

		_float4x4 WorldMatrixInverse = terrain->Get_ComTransform()->GetInvert();
		CNavigation* navi = terrain->Get_ComNavimesh();

		if (Check_Navi(WorldMatrixInverse, navi))
		{
			// 피킹시 mWorldPickPos에 정보 저장
			return true;
		}
	}

	return false;
}

bool CColliderManager::ColCheck_MOUSE()
{
	mIsMousePick = false;

	// 모든 충돌체랑 마우스위치랑 비교
	_ray worldRay =  GetSingle(CGameInstance)->Get_Ray_World();
	
	// 1차적으로 월드의 모든 오브젝트와 비교한다.
	// 후 구현

	// Static / Dynamic
	if (mIsMousePick = Check_Mouse_Object(worldRay))
	{
		return mIsMousePick;
	}

	// 2차로 지형과 비교한다.
	// 네비메시와 비교
	if (mIsMousePick = Check_Mouse_Terrain())
	{
		return mIsMousePick;
	}

	return mIsMousePick;
	
}

CColliderManager * CColliderManager::Create()
{
	CColliderManager*	pInstance = NEW CColliderManager();

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CColliderManager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CColliderManager::Free()
{
	ReleaseObjects();
}
