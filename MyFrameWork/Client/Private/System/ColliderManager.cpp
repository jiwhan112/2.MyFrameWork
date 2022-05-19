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
	// 1. ���콺 �浹üũ
	ColCheck_MOUSE();

	// 2. ��ü���� �浹 üũ


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
	// ������Ʈ ������ �浹
	return S_OK;
}

bool CColliderManager::Check_Navi(_float4x4 WorldMatrixInverse, CNavigation * navi)
{
	if (navi == nullptr)
		return false;

	CGameInstance* pInsance = GetSingle(CGameInstance);


	// �׺���̼� �޽ÿ� �浹üũ.
	pInsance->Transform_ToLocalSpace(WorldMatrixInverse);
	_bool		isPick = false;

	// �׺� �޽� �����̶� �浹
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

			// ���� ��ȯ
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

		// �浹ü�� üũ
		CCollider* ComCollider = staticobj->Get_ComCollider();
		_float dist;
		if (ComCollider->ColliderCheck(worldRay, dist))
		{
			staticobj->CollisionFunc(mWorldPickPos, dist);
			return true;
		}
	}

	for (auto& obj : mListColliders[COLLIDEROBJ_STATIC])
	{
		CGameObject_3D_Dynamic* dynamicobj = static_cast<CGameObject_3D_Dynamic*>(obj);
		if (dynamicobj == nullptr)
			continue;

		return false;
		// ���� ������Ʈ�� �ݶ��̴� ����Ʈ�� ����
		/*CCollider* ComCollider = dynamicobj->Get_ComModel();
		_float dist;
		if (ComCollider->ColliderCheck(worldRay, dist))
		{
			return true;
		}*/
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

		_float4x4 WorldMatrixInverse = terrain->Get_TransformCom()->GetInvert();
		CNavigation* navi = terrain->Get_ComNavimesh();

		if (Check_Navi(WorldMatrixInverse, navi))
		{
			// ��ŷ�� mWorldPickPos�� ���� ����
			return true;
		}
	}

	return false;
}

bool CColliderManager::ColCheck_MOUSE()
{
	mIsMousePick = false;

	// ��� �浹ü�� ���콺��ġ�� ��
	_ray worldRay =  GetSingle(CGameInstance)->Get_Ray_World();
	
	// 1�������� ������ ��� ������Ʈ�� ���Ѵ�.
	// �� ����

	// Static  / Dynamic
	if (mIsMousePick = Check_Mouse_Object(worldRay))
	{
		return mIsMousePick;
	}

	// 2���� ������ ���Ѵ�.
	// �׺�޽ÿ� ��
	if (mIsMousePick = Check_Mouse_Terrain())
	{
		return mIsMousePick;
	}

	return mIsMousePick;
	
}

CColliderManager * CColliderManager::Create()
{
	CColliderManager*	pInstance = new CColliderManager();

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