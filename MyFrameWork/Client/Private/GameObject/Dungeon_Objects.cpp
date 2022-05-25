#include "stdafx.h"
#include "GameObject/Dungeon_Objects.h"
#include "GameObject/Client_Object.h"
#include "Tool/Imgui_InGame.h"

CDungeon_Objects::CDungeon_Objects()
{
}

HRESULT CDungeon_Objects::NativeConstruct_Prototype(ID3D11Device* device, ID3D11DeviceContext* context,E_LEVEL level)
{
	mDevice = device;
	mDeviceContext = context;
	Safe_AddRef(mDevice);
	Safe_AddRef(mDeviceContext);

	mCurrentLevel = level;

	
	if (mCurrentLevel == LEVEL_MYGAMEPLAY)
	{
#ifdef _DEBUG
		FAILED_CHECK(Ready_IMGUI());
#endif

		FAILED_CHECK(Ready_Light());
		FAILED_CHECK(Ready_Camera());
		FAILED_CHECK(Ready_BackGround());
		FAILED_CHECK(Ready_GameObjects());


		//if (Option == CDungeon_Objects::LEVEL_GAMEOPTION_GAME_DUNGEON)
		//{
		//	// ���� �׽�Ʈ
		//	Init_GameScene_Dungeon();
		//}
		//else if (Option == CDungeon_Objects::LEVEL_GAMEOPTION_GAME_WORLD)
		//{
		//	// ����� �׽�Ʈ
		//	Init_GameScene_World();
		//}
		//else if (Option == CDungeon_Objects::LEVEL_GAMEOPTION_GAME)
		//{
		//	// ����
		//}

		

		return S_OK;
	}

	else if (mCurrentLevel == LEVEL_TOOL)
	{
		//Init_ToolScene_Tool();
		return S_OK;
	}

	return E_FAIL;
}

HRESULT CDungeon_Objects::Release_Objects()
{
	Safe_Release(mDevice);
	Safe_Release(mDeviceContext);

	mDevice = nullptr;
	mDeviceContext = nullptr;

	Safe_Release(mDungeonMap);
	Safe_Release(mTestUnit);

	if (mListVecTiles != nullptr)
	{
		for (auto& tile: *mListVecTiles)
		{
			Safe_Release(tile);
		}
		mListVecTiles->clear();
		Safe_Delete(mListVecTiles);
	}

	if (mListRemoveTile != nullptr)
	{
		mListRemoveTile->clear();
		Safe_Delete(mListRemoveTile);
	}

	return S_OK;
}

//HRESULT CDungeon_Objects::Init_GameScene_Dungeon()
//{
//	FAILED_CHECK(Ready_Light());
//	FAILED_CHECK(Ready_Camera());
//	FAILED_CHECK(Ready_BackGround());
//	FAILED_CHECK(Ready_GameObjects());
//
//	return S_OK;
//}
//
//HRESULT CDungeon_Objects::Init_GameScene_World()
//{
//
//	return S_OK;
//}
//
//HRESULT CDungeon_Objects::Init_ToolScene_Tool()
//{
//	return S_OK;
//}

HRESULT CDungeon_Objects::Ready_IMGUI()
{
	// IMGUI ����
	GetSingle(CGameManager)->Get_ImGuiManager()->Add_IMGUI(CImgui_InGame::Create(mDevice, mDeviceContext));
	//	GetSingle(CGameManager)->Get_ImGuiManager()->Add_IMGUI(CImgui_MyDemo::Create(m_pDevice, m_pDeviceContext));
	return S_OK;
}

HRESULT CDungeon_Objects::Ready_Light()
{
	// ����Ʈ ����
	// �� ����
	CGameInstance*	pGameInstance = GetSingle(CGameInstance);

	LIGHTDESC		LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eLightType = LIGHTDESC::TYPE_DIRECTIONAL;
	//	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vDiffuse = _float4(0.5f, 0.5f, 0.5f, 1.f);
	LightDesc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 1.f);
	LightDesc.vSpecular = _float4(0.3f, 0.3f, 0.3f, 1.f);

	LightDesc.vDirection = _float4(1, -1.f, 1, 0.f);

	if (FAILED(pGameInstance->Add_Light(mDevice, mDeviceContext, LightDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDungeon_Objects::Ready_Camera()
{
	// ī�޶� ����
	// ���� ī�޶� ������� ����
	CCamera::CAMERADESC		CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CameraDesc));

	CameraDesc.vEye = _float3(-5, 10.f, -5.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);

	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fAspect = _float(g_iWinCX) / g_iWinCY;
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 300.f;

	CameraDesc.TransformDesc.SpeedPersec = 10.f;
	CameraDesc.TransformDesc.RotPersec = XMConvertToRadians(90.0f);

	CCamera_Client* client = (CCamera_Client*)(GetSingle(CGameInstance)->Add_GameObject(mCurrentLevel, TAGLAY(LAY_CAMERA), TAGOBJ(GAMEOBJECT_CAMERA), &CameraDesc));
	client->Set_CameraMode(CCamera_Client::CAMERA_MODE_GAME_D);

	return S_OK;
}

HRESULT CDungeon_Objects::Ready_BackGround()
{
	// �÷��̾� == ���콺
	NULL_CHECK_HR(GetSingle(CGameInstance)->Add_GameObject(mCurrentLevel, TAGLAY(LAY_MOUSE), TAGOBJ(GAMEOBJECT_MOUSE)));

	// ���
//	NULL_CHECK_HR(GetSingle(CGameInstance)->Add_GameObject(mCurrentLevel, TAGLAY(LAY_UI), TAGOBJ(GAMEOBJECT_2D)));
//	NULL_CHECK_HR(GetSingle(CGameInstance)->Add_GameObject(mCurrentLevel, TAGLAY(LAY_NONE), TAGOBJ(GAMEOBJECT_3D_DYNAMIC)));

	// ��ī�� �ڽ�
	NULL_CHECK_HR(GetSingle(CGameInstance)->Add_GameObject(mCurrentLevel, TAGLAY(LAY_BACKGROUND), TAGOBJ(GAMEOBJECT_SKY)));


	return S_OK;
}

HRESULT CDungeon_Objects::Ready_GameObjects()
{
	// ���� ������Ʈ
	CGameObject_Creater* pCreateManager = GetSingle(CGameManager)->Get_CreaterManager();

	// �� Ÿ�� �ʱ�ȭ

	// �� ����
	mDungeonMap = (CGameObject_MyTerrain*)pCreateManager->CreateEmptyObject(GAMEOBJECT_MYTERRAIN);
	NULL_CHECK_HR(mDungeonMap);
	Safe_AddRef(mDungeonMap);

	// �ʿ� Ÿ�� �߰�
	FAILED_CHECK(GetSingle(CGameInstance)->Push_Object(mCurrentLevel, TAGLAY(LAY_TERRAIN), mDungeonMap));
	this->Setup_Terrain();

	// ���� ����
	mTestUnit = (CGameObject_3D_Dynamic*)pCreateManager->CreateEmptyObject(GAMEOBJECT_3D_DYNAMIC);
	NULL_CHECK_HR(mTestUnit);
	Safe_AddRef(mTestUnit);

	FAILED_CHECK(GetSingle(CGameInstance)->Push_Object(mCurrentLevel, TAGLAY(LAY_OBJECT), mTestUnit));

	return S_OK;
}

HRESULT CDungeon_Objects::Setup_Terrain_Tool(CGameObject_MyTerrain* map)
{
	if (map == nullptr)
		return E_FAIL;

	mDungeonMap = map;
	Safe_AddRef(mDungeonMap);

	// Ÿ�� ����
	FAILED_CHECK(Create_Tiles(mCurrentLevel));
	// Ÿ�� �̿� ����
	FAILED_CHECK(Setup_Neigbor_Tile());
	// Ÿ�� �޽� ����
	FAILED_CHECK(Setup_TileState());
	// �׺�޽ÿ� Ÿ�� �浹
	FAILED_CHECK(Setup_Collision_Navi2Tile());

	return S_OK;
}

HRESULT CDungeon_Objects::Setup_DungeonTerrain_Data(TERRAIN_DESC * data)
{
	mDesc_DungeonMap = data;
	return S_OK;
}

HRESULT CDungeon_Objects::Setup_DungeonWorld_Data(TERRAIN_DESC * data)
{
	mDesc_WorldMap = data;
	return S_OK;
}

HRESULT CDungeon_Objects::ResetTile_Tool(TERRAIN_DESC * data)
{
	if (mDungeonMap == nullptr)
		return E_FAIL;

	if (mListVecTiles != nullptr)
	{
		for (auto& tile : *mListVecTiles)
		{
			tile->Set_Dead();
			Safe_Release(tile);
		}
		mListVecTiles->clear();
		Safe_Delete(mListVecTiles);
	}

	if (mListRemoveTile != nullptr)
	{
		mListRemoveTile->clear();
		Safe_Delete(mListRemoveTile);
	}

	mDesc_DungeonMap = data;

	// Ÿ�� ����
	FAILED_CHECK(Create_Tiles(mCurrentLevel));
	// Ÿ�� �̿� ����
	FAILED_CHECK(Setup_Neigbor_Tile());
	// Ÿ�� �޽� ����
	FAILED_CHECK(Setup_TileState());
	// �׺�޽ÿ� Ÿ�� �浹
	FAILED_CHECK(Setup_Collision_Navi2Tile());
	
	// Static ������Ʈ ����
	_uint objSize = data->mObjectSize;
	for (int i =0; i< objSize ; ++i)
	{
		string str = data->mArrayModelObjects[i].mProtoName;
		wstring wstr = CHelperClass::Convert_str2wstr(str);
		_float4x4 worldmat = data->mArrayModelObjects[i].mWorldMat;

		CGameObject_Base* obj  =  GetSingle(CGameManager)->Get_CreaterManager()->Create_ObjectClone_Prefab(mCurrentLevel,wstr,
			TAGLAY(LAY_BACKGROUND));
		if (obj == nullptr)
			return E_FAIL;
		obj->Set_Transform(worldmat);
	}

	return S_OK;
}

HRESULT CDungeon_Objects::Setup_Terrain()
{
	if (mDungeonMap == nullptr)
		return E_FAIL;

	// Ÿ�� ����
	FAILED_CHECK(Create_Tiles(mCurrentLevel));
	// Ÿ�� �̿� ����
	FAILED_CHECK(Setup_Neigbor_Tile());
	// Ÿ�� �޽� ����
	FAILED_CHECK(Setup_TileState());
	// �׺�޽ÿ� Ÿ�� �浹
	FAILED_CHECK(Setup_Collision_Navi2Tile());

	return S_OK;
}

CGameObject_3D_Tile * CDungeon_Objects::FInd_TIleForIndex(_int TileIndex) const
{
	for (auto& obj : *mListVecTiles)
	{
		if (obj->Get_TileIndex() == TileIndex)
		{
			return obj;
		}
	}
	return nullptr;
}

HRESULT CDungeon_Objects::RemoveTile(CGameObject_3D_Tile * pTIle)
{
	if (mListVecTiles == nullptr)
		return E_FAIL;
	if (pTIle == nullptr)
		return E_FAIL;

	if (mCurrentLevel == LEVEL_TOOL)
	{
		if (mListRemoveTile == nullptr)
			mListRemoveTile = NEW list<_uint>();
		mListRemoveTile->push_front(pTIle->Get_TileIndex());
	}

	mListVecTiles->remove(pTIle);
	Safe_Release(pTIle);
	return S_OK;
}

HRESULT CDungeon_Objects::Create_Tiles(E_LEVEL level)
{
	// Ÿ�� ���� 
	_uint* TileXZ = mDungeonMap->Get_TerrainBuffer()->Get_XZ();
	mSizeX = TileXZ[0];
	mSizeZ = TileXZ[1];


	if (mSizeX <= 0)
		return E_FAIL;

	if (mListVecTiles == nullptr)
		mListVecTiles = NEW list<CGameObject_3D_Tile*>;

	CGameObject_Creater* Create_Manager = GetSingle(CGameManager)->Get_CreaterManager();
	_uint levelindex = GetSingle(CGameInstance)->Get_CurrentLevelIndex();
	for (_int z = 0; z < mSizeZ - 1; ++z)
	{
		for (_int x = 0; x < mSizeX - 1; ++x)
		{
			bool ismatch = false;
			_int iIndex = mDungeonMap->Get_TerrainBuffer()->Get_TilIndex(x, z);

			if (mDesc_DungeonMap != nullptr)
			{
				for (int i = 0; i < mDesc_DungeonMap->mNoTileSize; ++i)
				{
					if (mDesc_DungeonMap->mArrayIndes[i] == iIndex)
					{
						ismatch = true;
						break;;
					}
				}
			}	
			if(ismatch)
				continue;;

			CGameObject_3D_Tile* tileObj = (CGameObject_3D_Tile*)Create_Manager->CreateEmptyObject(GAMEOBJECT_3D_TILE);
			_float3 CreatePosition = mDungeonMap->Get_TerrainBuffer()->Get_TileWorldPos(x, z);

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

	return S_OK;
}

HRESULT CDungeon_Objects::Setup_Collision_Navi2Tile()
{
	if (mListVecTiles == nullptr)
		return E_FAIL;


	CNavigation* navi = mDungeonMap->Get_ComNavimesh();
	auto VecCell = navi->Get_CellVector();

	for (auto& tile : *mListVecTiles)
	{
		_float3 tileworldPos = tile->Get_WorldPostition();

		for (auto& cell : *VecCell)
		{
			_float3 point1 = cell->Get_Point(CCell::POINT_A);
			if (_float3::Distance(point1, tileworldPos) > 3)
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

HRESULT CDungeon_Objects::Update_Collision_Navi2Tile(_uint index)
{
	if (mListVecTiles == nullptr)
		return E_FAIL;

	// �׺� �޽ÿ� �浹ü �浹..
	CGameObject_3D_Tile* tile = FInd_TIleForIndex(index);
	int maxcount = 0;
	if (tile == nullptr)
		return E_FAIL;


	_float3 tilePos = tile->Get_ComTransform()->GetState(CTransform::STATE_POSITION);
	CNavigation* navi = mDungeonMap->Get_ComNavimesh();
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
			maxcount++;
			if (maxcount >= 2)
				return S_OK;
		}
	}

	return S_OK;
}

HRESULT CDungeon_Objects::Setup_Neigbor_Tile()
{
	// Ÿ���� �̿������ϱ�
		// 1. Ÿ�� �ε����� ã�� 

		// ��ü�� �����鼭 �ش� Ÿ���� �ε����� �ִ��� Ȯ��
		// �ִٸ� �ְ� ���ٸ� -1;

	int sizeXZ = mSizeX * mSizeZ;

	for (auto& obj : *mListVecTiles)
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
		if (((mSizeX*(mSizeZ - 1)) <= currentIndex) && (currentIndex <= sizeXZ - 1))
		{

			if (iIndex[CGameObject_3D_Tile::NEIGHBOR_TILE_TOP] >= sizeXZ)
				iIndex[CGameObject_3D_Tile::NEIGHBOR_TILE_TOP] = -1;
			else
				iIndex[CGameObject_3D_Tile::NEIGHBOR_TILE_TOP] = -1;

		}
		// (index % (SizeX-1) == 0 // ������

		int h = currentIndex * mSizeX;
		if ((currentIndex != 0) && ((currentIndex % ((mSizeX * h) - 1)) == 0))
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
			for (int i = 0; i < CGameObject_3D_Tile::NEIGHBOR_TILE_END; ++i)
			{
				if (iIndex[i] < 0 || iIndex[i] >= sizeXZ)
				{
					obj->Set_TileIndex((CGameObject_3D_Tile::E_NEIGHBOR_TILE)i, -1);
				}

				if (iIndex[i] == SearchIndex)
				{
					obj->Set_TileIndex((CGameObject_3D_Tile::E_NEIGHBOR_TILE)i, SearchIndex);
				}
			}
		}
	}

	return S_OK;
}

HRESULT CDungeon_Objects::Setup_TileState(_int tileIndex)
{
	if (mListVecTiles == nullptr)
		return E_FAIL;

	if (tileIndex == -1)
	{
		// ��� Ÿ�� ������Ʈ
		for (auto& tile : *mListVecTiles)
		{
			// Ÿ�� ������ 
			tile->Update_NeighborTile();
		}
		return S_OK;
	}

	else
	{
		for (auto& tile : *mListVecTiles)
		{
			// �ش� Ÿ�ϸ� ������Ʈ 
			if (tileIndex == tile->Get_TileIndex())
			{
				tile->Update_NeighborTile();
				Update_Collision_Navi2Tile(tileIndex);
				return S_OK;
			}
		}
	}
	return E_FAIL;
}

CDungeon_Objects * CDungeon_Objects::Create(ID3D11Device* device, ID3D11DeviceContext* context, E_LEVEL level )
{
	CDungeon_Objects*	pInstance = NEW CDungeon_Objects();

	if (FAILED(pInstance->NativeConstruct_Prototype(device, context, level)))
	{
		MSGBOX("Failed to Creating CRenderer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDungeon_Objects::Free()
{
	Release_Objects();

	
}
