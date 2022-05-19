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



	return UPDATENONE;
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
	mSizeX = TileXZ[0];
	mSizeZ = TileXZ[1];

	// #DEBUG DebugCode
//	mSizeX = mSizeZ = 5;
//	static _int TileCount = CGameObject_3D_Tile::TILETYPE_TOP;

	if (mSizeX <= 0)
		return E_FAIL;

	if (mVecTiles == nullptr)
		mVecTiles = NEW vector<CGameObject_3D_Tile*>;

	CGameObject_Creater* Create_Manager = GetSingle(CGameManager)->Get_CreaterManager();
	_uint levelindex = GetSingle(CGameInstance)->Get_CurrentLevelIndex();
	for (_int z = 0; z < mSizeZ-1; ++z)
	{
		for (_int x = 0; x < mSizeX-1; ++x)
		{

			CGameObject_3D_Tile* tileObj = (CGameObject_3D_Tile*)Create_Manager->CreateEmptyObject(GAMEOBJECT_3D_TILE);
			_float3 CreatePosition = mDaungonTerrain->Get_TerrainBuffer()->Get_TileWorldPos(x, z);
			_int iIndex = mDaungonTerrain->Get_TerrainBuffer()->Get_TilIndex(x, z);

			CreatePosition.y += 0.01f;
			
			tileObj->Set_Position(CreatePosition);
			tileObj->Set_LoadNewFBX((CGameObject_3D_Tile::TILETYPE_TOP));
			tileObj->Set_TileIndex(iIndex);

			Create_Manager->PushObject((CGameObject_Base**)&tileObj, levelindex, TAGLAY(LAY_CUBETILE));
			mVecTiles->push_back(tileObj);
			Safe_AddRef(tileObj);
		}
	}

	// 이웃 타일 넣기
	Set_Neigbor_Tile();

	// 초기 세팅
	Update_TileState();
	return S_OK;
}

HRESULT CDaungon_Manager::Set_Neigbor_Tile()
{
	// 타일의 이웃설정하기
	// 1. 타일 인덱스로 찾기 

	// 전체를 돌리면서 해당 타일의 인덱스가 있는지 확인
	// 있다면 넣고 없다면 -1;

	int sizeXZ = mSizeX * mSizeZ;

	for (auto& obj: *mVecTiles)
	{
		_int currentIndex = obj->Get_TileIndex();
		
		// 상하좌우 끝의 타일은 무조건 -1을 받아둔다.
		// 해당 인덱스가 유효한지 검사 // 없는곳에 -1을 넣고 나머지는 그대로 한다.
			// 0 ~ SizeX-1 // 아래쪽
			// index%=SizeX == 0 // 왼쪽
			// index%=SizeX-1 == 0 // 오른쪽
			// (SizeX*(SizeZ-1)) ~((SizeX*SizeZ)-1) // 위쪽
		
		// 특정 위치의 인덱스에는 유호한 타일 값을 무조건 -1을 넣는다.

		_int iIndex[CGameObject_3D_Tile::NEIGHBOR_TILE_END] =
		{
			currentIndex - 1,
			currentIndex + mSizeX,
			currentIndex + 1,
			currentIndex - mSizeX
		};

		//bool bIndex[CGameObject_3D_Tile::NEIGHBOR_TILE_END] =
		//{
		//	false,false,false,false
		//};

		// 해당 인덱스여 따라 예외처리

		// (index % SizeX) == 0 // 왼쪽
		if ((currentIndex % mSizeX) == 0)
		{
			if (iIndex[CGameObject_3D_Tile::NEIGHBOR_TILE_LEFT] > 0)
				iIndex[CGameObject_3D_Tile::NEIGHBOR_TILE_LEFT] = -1;
			else
				iIndex[CGameObject_3D_Tile::NEIGHBOR_TILE_LEFT] = -1;

		}


		// (SizeX*(SizeZ-1)) ~((SizeX*SizeZ)-1) // 위쪽
		if ( ((mSizeX*(mSizeZ - 1)) <= currentIndex) && (currentIndex <= sizeXZ - 1))
		{

			if (iIndex[CGameObject_3D_Tile::NEIGHBOR_TILE_TOP] >= sizeXZ)
				iIndex[CGameObject_3D_Tile::NEIGHBOR_TILE_TOP] = -1;
			else
				iIndex[CGameObject_3D_Tile::NEIGHBOR_TILE_TOP] = -1;

		}
		// (index % (SizeX-1) == 0 // 오른쪽

		int h = currentIndex * mSizeX;
		if ((currentIndex!=0) && ((currentIndex % ((mSizeX * h)-1)) == 0) )
		{
			iIndex[CGameObject_3D_Tile::NEIGHBOR_TILE_RIGHT] = -1;

		}

		// 0 ~ SizeX-1 // 아래쪽
		if (0 <= currentIndex && currentIndex <= mSizeX - 1)
		{
			if (iIndex[CGameObject_3D_Tile::NEIGHBOR_TILE_BOTTOM] < 0)
				iIndex[CGameObject_3D_Tile::NEIGHBOR_TILE_BOTTOM] = -1;
			else
				iIndex[CGameObject_3D_Tile::NEIGHBOR_TILE_BOTTOM] = -1;
		}

		// 다음 루프에서는 유효한 인덱스인지 확인
		for (auto& searchobj : *mVecTiles)
		{
			
			_int SearchIndex = searchobj->Get_TileIndex();
			for (int i = 0; i< CGameObject_3D_Tile::NEIGHBOR_TILE_END;++i)
			{
				if (iIndex[i] < 0 || iIndex[i] >= sizeXZ)
				{
					obj->Set_TileIndex((CGameObject_3D_Tile::E_NEIGHBOR_TILE)i, -1);
				}

				if ( iIndex[i] == SearchIndex)
				{
					obj->Set_TileIndex((CGameObject_3D_Tile::E_NEIGHBOR_TILE)i, SearchIndex);
				}
			}
		}

		
	}
	return S_OK;
}




HRESULT CDaungon_Manager::Update_TileState(_int tileIndex)
{
	// 타일 인덱스가 없다면 전체 타일 업데이트
	if (mVecTiles == nullptr)
		return E_FAIL;

	bool isFind = false;
	if (tileIndex == -1)
	{
		isFind = true;
		for (auto& tile : *mVecTiles)
		{
			// 타일 정보로 
			tile->Update_NeighborTile();
			
		}
	}
	else
	{
		for (auto& tile : *mVecTiles)
		{
			// 타일 정보로 
			if (tileIndex == tile->Get_TileIndex())
			{
				tile->Update_NeighborTile();
				isFind = true;
				return S_OK;
			}
		}
	}



	if (isFind)
		return S_OK;
	else
		return E_FAIL;
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


