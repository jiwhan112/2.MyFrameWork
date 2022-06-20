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
	mIsMousePick_Object = false;
	mIsMousePick_Terrain = false;
	return S_OK;
}

HRESULT CColliderManager::Tick_ColliderCheck(_double Timer)
{
	// 0. UI 충돌체크
	if (ColCheck_MOUSE_Object_UI())
	{

	}
	else
	{
		// 1. 마우스 충돌체크
		ColCheck_MOUSE_Object();
		ColCheck_MOUSE_Terrain();

		// 2. 객체들의 충돌 체크
		ColCheck_OBJECTS(Timer);

	}


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

HRESULT CColliderManager::ColCheck_OBJECTS(_double timer)
{
	// 던전에서 충돌
	// 움직이는 오브젝트끼리 충돌

	for (auto& obj1 : mListColliders[COLLIDEROBJ_DYNAMIC])
	{
		CGameObject_3D_Dynamic* oobj1 = static_cast<CGameObject_3D_Dynamic*>(obj1);
		if (oobj1 == nullptr)
			continue;

		for (auto& obj2 : mListColliders[COLLIDEROBJ_DYNAMIC])
		{
			if (obj1 == obj2)
				continue;

			CGameObject_3D_Dynamic* oobj2 = static_cast<CGameObject_3D_Dynamic*>(obj2);
			if (oobj2 == nullptr)
				continue;

			if (oobj1->Get_CurrentMap() == oobj2->Get_CurrentMap())
			{
				auto objectList1 = oobj1->Get_ComListCollider();
				auto objectList2 = oobj2->Get_ComListCollider();
				if (objectList1 == nullptr || objectList2 == nullptr)
					continue;

				if (ColCheck_Dynamic(*objectList1->begin(), *objectList2->begin()))
				{
					oobj1->CollisionFunc(oobj2, timer);
				}
			}
		}
	}
	return S_OK;
}

bool CColliderManager::ColCheck_MOUSE_Object_UI()
{

	mWindowPos = GetSingle(CGameInstance)->Get_WindowPos();

	for (auto& obj : mListColliders[COLLIDEROBJ_UI])
	{
		CGameObject_2D* oobj1 = static_cast<CGameObject_2D*>(obj);
		if (oobj1 == nullptr)
			continue;
		if (oobj1->Get_UIType() == CGameObject_2D::UITYPE_BUTTON1 ||
			oobj1->Get_UIType() == CGameObject_2D::UITYPE_BUTTON2_UNIT)
		{
			_rect newrect = oobj1->Get_UIDesc().mUIRECT;
			newrect.x -= newrect.width*0.5;
			newrect.y -= newrect.height*0.5;

			if (newrect.Contains(mWindowPos))
			{
				oobj1->CollisionFunc(nullptr);
				return true;
			}
		}

	}

	return false;
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

bool CColliderManager::ColCheck_Dynamic(CCollider * a, CCollider * b)
{
	if (a->ColliderCheck(b))
		return true;

	return false;
}

bool CColliderManager::Check_Mouse_Terrain()
{
	for (auto& obj : mListColliders[COLLIDEROBJ_TERRAIN])
	{
		CGameObject_MyTerrain* terrain = static_cast<CGameObject_MyTerrain*>(obj);
		if (terrain == nullptr)
			continue;

		// 지형 체크
		auto mapType = terrain->Get_MapType();
		auto gameModeType = GetSingle(CGameManager)->Get_DaungonManager()->Get_CurrentGameMode();
		if (gameModeType == CDungeon_Manager::GAMEMODE_DUNGEON)
		{
			if ((mapType == CGameObject_MyTerrain::MAPTYPE_DUNGEON) == false)
			{
				continue;
			}
		}
		else if (gameModeType == CDungeon_Manager::GAMEMODE_WORLD)
		{
			if ((mapType == CGameObject_MyTerrain::MAPTYPE_WORLD) == false)
			{
				continue;
			}
		}

		

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

bool CColliderManager::ColCheck_MOUSE_Object()
{
	// 모든 충돌체랑 마우스위치랑 비교
	mWorldRay = GetSingle(CGameInstance)->Get_Ray_World();
	mIsMousePick_Object = false;

	// Static / Dynamic
	if (mIsMousePick_Object = Check_Mouse_Object(mWorldRay))
	{
		return mIsMousePick_Object;
	}
	return mIsMousePick_Object;
}

bool CColliderManager::ColCheck_MOUSE_Terrain()
{
	// 모든 충돌체랑 마우스위치랑 비교
	mWorldRay = GetSingle(CGameInstance)->Get_Ray_World();
	mIsMousePick_Terrain = false;

	// 현재 게임 모드에 따라 특정 네비 메시와 비교
	if (mIsMousePick_Terrain = Check_Mouse_Terrain())
	{
		return mIsMousePick_Terrain;
	}

	return mIsMousePick_Terrain;
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
