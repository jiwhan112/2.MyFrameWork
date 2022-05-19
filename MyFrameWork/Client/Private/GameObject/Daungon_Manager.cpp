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
	// �̿��� Ÿ�� �������� ���� Ÿ���� ���¸� �����Ѵ�.


	// Ÿ���� ����Ǹ� �����
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

	// �̿� Ÿ�� �ֱ�
	Set_Neigbor_Tile();

	// �ʱ� ����
	Update_TileState();
	return S_OK;
}

HRESULT CDaungon_Manager::Set_Neigbor_Tile()
{
	// Ÿ���� �̿������ϱ�
	// 1. Ÿ�� �ε����� ã�� 

	// ��ü�� �����鼭 �ش� Ÿ���� �ε����� �ִ��� Ȯ��
	// �ִٸ� �ְ� ���ٸ� -1;

	int sizeXZ = mSizeX * mSizeZ;

	for (auto& obj: *mVecTiles)
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
	// Ÿ�� �ε����� ���ٸ� ��ü Ÿ�� ������Ʈ
	if (mVecTiles == nullptr)
		return E_FAIL;

	bool isFind = false;
	if (tileIndex == -1)
	{
		isFind = true;
		for (auto& tile : *mVecTiles)
		{
			// Ÿ�� ������ 
			tile->Update_NeighborTile();
			
		}
	}
	else
	{
		for (auto& tile : *mVecTiles)
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


