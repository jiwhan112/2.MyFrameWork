#include "stdafx.h"
#include "GameObject/Client_Object.h"

CDungeon_Manager::CDungeon_Manager()
{
}

HRESULT CDungeon_Manager::NativeConstruct_Prototype()
{
	return S_OK;
}

_int CDungeon_Manager::Tick(_double TimeDelta)
{

	return UPDATENONE;
}

_int CDungeon_Manager::LateTick(_double TimeDelta)
{
	return UPDATENONE;
}

HRESULT CDungeon_Manager::Init_TileSet()
{
	// �̿��� Ÿ�� �������� ���� Ÿ���� ���¸� �����Ѵ�.


	// Ÿ���� ����Ǹ� �����
	return S_OK;
}

HRESULT CDungeon_Manager::Update_NaviMesh_STOPSetting(_uint TileIndex)
{
	if (mListVecTiles == nullptr)
		return E_FAIL;

	// �׺� �޽ÿ� �浹ü �浹..
	CGameObject_3D_Tile* tile =  FInd_TIleForIndex(TileIndex);
	if (tile == nullptr)
		return E_FAIL;


	_float3 tilePos = tile->Get_ComTransform()->GetState(CTransform::STATE_POSITION);
	CNavigation* navi = mDaungonTerrain->Get_ComNavimesh();
	auto VecCell = navi->Get_CellVector();
	if (VecCell == nullptr)
		return E_FAIL;
	for (auto& cell : *VecCell)
	{
		_float3 point1 = cell->Get_Point(CCell::POINT_A);
		if (_float3::Distance(point1, tilePos) > 3)
			continue;

		_float3 point2 = cell->Get_Point(CCell::POINT_B);
		_float3 point3 = cell->Get_Point(CCell::POINT_C);

		CCollider* col = tile->Get_ComCollider();
		if (col == nullptr)
			return E_FAIL;
		if (col->ColliderCheck(point1, point2, point3))
		{
			cell->Set_TileType(CCell::CELLTYPE_NONE);
		}
	}


	return S_OK;
}



HRESULT CDungeon_Manager::Setup_Terrain(CGameObject_MyTerrain* terrain , E_LEVEL level)
{
	mDaungonTerrain = terrain;
	Safe_AddRef(mDaungonTerrain);
	
	if (terrain == nullptr)
		return  E_FAIL;

	FAILED_CHECK(Init_Tile(level));
	FAILED_CHECK(Set_Neigbor_Tile());
	FAILED_CHECK(Init_NaviMesh_STOPSetting());

	return S_OK;
}

HRESULT CDungeon_Manager::Init_GameLevel(E_LEVEL level)
{
	if (level == LEVEL_MYGAMEPLAY)
	{
		mGameAllObjects->Init_GameLevel(level);

		// 
		//// ������ ī�޶� ���콺 ����
		//FAILED_CHECK(Ready_Light());
		//FAILED_CHECK(Ready_Layer_Camera(TAGLAY(LAY_CAMERA)));
		//FAILED_CHECK(Ready_Layer_Mouse(TAGLAY(LAY_PLAYER)));


		//// ������ / ���潺 �� ���� �� �׺� ����
		//FAILED_CHECK(Ready_Layer_BackGround(TAGLAY(LAY_BACKGROUND)));
		//
		//// ��Ÿ ���ӿ� ���õ� �� ����
		//FAILED_CHECK(Ready_Level_BaseGame());
		return S_OK;
	}

	return E_FAIL;
}

void CDungeon_Manager::Release_DaungonData()
{
	Safe_Release(mDaungonTerrain);
	mDaungonTerrain = nullptr;

	if (mListVecTiles != nullptr)
	{
		for (auto& obj: *mListVecTiles) 
		{
			Safe_Release(obj);
		}
		mListVecTiles->clear();
		Safe_Delete(mListVecTiles);
	}
}

CGameObject_3D_Tile * CDungeon_Manager::FInd_TIleForIndex(_int TileIndex) const
{
	for (auto& obj: *mListVecTiles)
	{
		if (obj->Get_TileIndex() == TileIndex)
		{
			return obj;
		}
	}

	return nullptr;
}

HRESULT CDungeon_Manager::RemoveTile(CGameObject_3D_Tile * pTIle)
{
	if (mListVecTiles == nullptr)
		return E_FAIL;

	mListVecTiles->remove(pTIle);
	Safe_Release(pTIle);

	return S_OK;
}

HRESULT CDungeon_Manager::Init_Tile(E_LEVEL level)
{
	_uint* TileXZ = mDaungonTerrain->Get_TerrainBuffer()->Get_XZ();
	mSizeX = TileXZ[0];
	mSizeZ = TileXZ[1];


	if (mSizeX <= 0)
		return E_FAIL;

	if (mListVecTiles == nullptr)
		mListVecTiles = NEW list<CGameObject_3D_Tile*>;

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
			tileObj->Set_ColliderPosition();

			Create_Manager->PushObject((CGameObject_Base**)&tileObj, level, TAGLAY(LAY_CUBETILE));
			mListVecTiles->push_back(tileObj);
			Safe_AddRef(tileObj);
		}
	}

	// �̿� Ÿ�� �ֱ�
	Set_Neigbor_Tile();

	// �ʱ� ����
	Update_TileState();
	return S_OK;
}

HRESULT CDungeon_Manager::Init_NaviMesh_STOPSetting()
{
	if (mListVecTiles == nullptr)
		return E_FAIL;


	CNavigation* navi =  mDaungonTerrain->Get_ComNavimesh();
	auto VecCell =  navi->Get_CellVector();

	for (auto& tile : *mListVecTiles)
	{
		_float3 tileworldPos = tile->Get_WorldPostition();

		for (auto& cell : *VecCell)
		{
			_float3 point1 = cell->Get_Point(CCell::POINT_A);
			if (_float3::Distance(point1, tileworldPos) >  3)
				continue;

			_float3 point2 = cell->Get_Point(CCell::POINT_B);
			_float3 point3 = cell->Get_Point(CCell::POINT_C);

			CCollider* col = tile->Get_ComCollider();
			if (col == nullptr)
				continue;
			if (col->ColliderCheck(point1, point2, point3))
			{
				cell->Set_TileType(CCell::CELLTYPE_STOP);
			}
		}
	}
	return S_OK;
}

HRESULT CDungeon_Manager::Set_Neigbor_Tile()
{
	// Ÿ���� �̿������ϱ�
	// 1. Ÿ�� �ε����� ã�� 

	// ��ü�� �����鼭 �ش� Ÿ���� �ε����� �ִ��� Ȯ��
	// �ִٸ� �ְ� ���ٸ� -1;

	int sizeXZ = mSizeX * mSizeZ;

	for (auto& obj: *mListVecTiles)
	{
		_int currentIndex = obj->Get_TileIndex();
		
		// �����¿� ���� Ÿ���� ������ -1�� �޾Ƶд�.
		// �ش� �ε����� ��ȿ���� �˻� // ���°��� -1�� �ְ� �������� �״�� �Ѵ�.
			// 0 ~ SizeX-1 // �Ʒ���
			// index%=SizeX == 0 // ����
			// index%=SizeX-1 == 0 // ������
			// (SizeX*(SizeZ-1)) ~((SizeX*SizeZ)-1) // ����
		
		// Ư�� ��ġ�� �ε������� ��ȣ�� Ÿ�� ���� ������ -1�� �ִ´�.

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

		// �ش� �ε����� ���� ����ó��

		// (index % SizeX) == 0 // ����
		if ((currentIndex % mSizeX) == 0)
		{
			if (iIndex[CGameObject_3D_Tile::NEIGHBOR_TILE_LEFT] > 0)
				iIndex[CGameObject_3D_Tile::NEIGHBOR_TILE_LEFT] = -1;
			else
				iIndex[CGameObject_3D_Tile::NEIGHBOR_TILE_LEFT] = -1;

		}


		// (SizeX*(SizeZ-1)) ~((SizeX*SizeZ)-1) // ����
		if ( ((mSizeX*(mSizeZ - 1)) <= currentIndex) && (currentIndex <= sizeXZ - 1))
		{

			if (iIndex[CGameObject_3D_Tile::NEIGHBOR_TILE_TOP] >= sizeXZ)
				iIndex[CGameObject_3D_Tile::NEIGHBOR_TILE_TOP] = -1;
			else
				iIndex[CGameObject_3D_Tile::NEIGHBOR_TILE_TOP] = -1;

		}
		// (index % (SizeX-1) == 0 // ������

		int h = currentIndex * mSizeX;
		if ((currentIndex!=0) && ((currentIndex % ((mSizeX * h)-1)) == 0) )
		{
			iIndex[CGameObject_3D_Tile::NEIGHBOR_TILE_RIGHT] = -1;

		}

		// 0 ~ SizeX-1 // �Ʒ���
		if (0 <= currentIndex && currentIndex <= mSizeX - 1)
		{
			if (iIndex[CGameObject_3D_Tile::NEIGHBOR_TILE_BOTTOM] < 0)
				iIndex[CGameObject_3D_Tile::NEIGHBOR_TILE_BOTTOM] = -1;
			else
				iIndex[CGameObject_3D_Tile::NEIGHBOR_TILE_BOTTOM] = -1;
		}

		// ���� ���������� ��ȿ�� �ε������� Ȯ��
		for (auto& searchobj : *mListVecTiles)
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




HRESULT CDungeon_Manager::Update_TileState(_int tileIndex)
{
	// Ÿ�� �ε����� ���ٸ� ��ü Ÿ�� ������Ʈ
	if (mListVecTiles == nullptr)
		return E_FAIL;

	bool isFind = false;
	if (tileIndex == -1)
	{
		isFind = true;
		for (auto& tile : *mListVecTiles)
		{
			// Ÿ�� ������ 
			tile->Update_NeighborTile();
			
		}
	}
	else
	{
		for (auto& tile : *mListVecTiles)
		{
			// Ÿ�� ������ 
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

CDungeon_Manager * CDungeon_Manager::Create()
{
	CDungeon_Manager* pInstance = NEW CDungeon_Manager();

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Created CDungeon_Manager");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDungeon_Manager::Free()
{
	
	Release_DaungonData();
}


