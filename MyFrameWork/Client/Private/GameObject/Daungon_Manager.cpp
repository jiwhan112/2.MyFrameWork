#include "stdafx.h"
#include "GameObject/Daungon_Manager.h"
#include "GameObject/GameObject_MyTerrain.h"
#include "GameObject/GameObject_3D_Tile.h"

CDaungon_Manager::CDaungon_Manager()
{
}

HRESULT CDaungon_Manager::NativeConstruct_Prototype()
{
	return S_OK;
}

_int CDaungon_Manager::Tick(_double TimeDelta)
{
	return _int();
}

HRESULT CDaungon_Manager::Init_TileSet()
{
	// 이웃한 타일 기준으로 현재 타일의 상태를 결정한다.


	// 타일이 변경되면 재실행
	return S_OK;
}



HRESULT CDaungon_Manager::Setup_Terrain(CGameObject_MyTerrain* terrain)
{

	mDaungonTerrain = terrain;
	Safe_AddRef(mDaungonTerrain);
	
	if (terrain == nullptr)
		return  E_FAIL;

	FAILED_CHECK(Init_Tile());
	FAILED_CHECK(Set_Neigbor_Tile());

	return S_OK;
}

void CDaungon_Manager::Release_DaungonData()
{
	Safe_Release(mDaungonTerrain);
	mDaungonTerrain = nullptr;

	if (mVecTiles != nullptr)
	{
		for (auto& obj: *mVecTiles) 
		{
			Safe_Release(obj);
		}
		mVecTiles->clear();
		Safe_Delete(mVecTiles);
	}
}

HRESULT CDaungon_Manager::Init_Tile()
{
	_uint* TileXZ = mDaungonTerrain->Get_TerrainBuffer()->Get_XZ();
	_uint TileX = TileXZ[0];
	_uint TileZ = TileXZ[1];

	if (TileX <= 0)
		return E_FAIL;

	if (mVecTiles == nullptr)
		mVecTiles = NEW vector<CGameObject_3D_Tile*>;



	CGameObject_Creater* Create_Manager = GetSingle(CGameManager)->Get_CreaterManager();
	_uint levelindex = GetSingle(CGameInstance)->Get_CurrentLevelIndex();

	for (int z = 0; z < TileZ; ++z)
	{
		for (int x = 0; x < TileX; ++x)
		{
			int iIndex = z * TileX + x;
			CGameObject_3D_Tile* tileObj = (CGameObject_3D_Tile*)Create_Manager->CreateEmptyObject(GAMEOBJECT_3D_TILE);
			_float3 CreatePosition = mDaungonTerrain->Get_TileWorld(iIndex);
			tileObj->Set_Position(CreatePosition);
			tileObj->Set_LoadNewFBX(CGameObject_3D_Tile::TILETYPE_TOP);
			Create_Manager->PushObject((CGameObject_Base**)&tileObj, levelindex, TAGLAY(LAY_CUBETILE));
			mVecTiles->push_back(tileObj);
			Safe_AddRef(tileObj);
		}
	}

	// 이웃 타일 넣기
	Set_Neigbor_Tile();

	return S_OK;
}

HRESULT CDaungon_Manager::Set_Neigbor_Tile()
{
	return S_OK;
}


HRESULT CDaungon_Manager::Update_TerrainTile()
{
	// _uint levelIndex = GetSingle(CGameInstance)->Get_CurrentLevelIndex();
	// 
	// auto baselist = GetSingle(CGameManager)->Get_LevelObjectList(TAGLAY(LAY_CUBETILE));

	// 이웃하는 타일을 기준으로 자신의 상태를 결정한다.


	return S_OK;
}

CDaungon_Manager * CDaungon_Manager::Create()
{
	CDaungon_Manager* pInstance = NEW CDaungon_Manager();

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CDaungon_Manager");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDaungon_Manager::Free()
{
	
	Release_DaungonData();
}


