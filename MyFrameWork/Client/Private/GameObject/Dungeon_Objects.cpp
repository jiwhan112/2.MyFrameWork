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
//#ifdef _DEBUG
		FAILED_CHECK(Ready_IMGUI());
//#endif

		FAILED_CHECK(Ready_Light());
		FAILED_CHECK(Ready_Camera());
		FAILED_CHECK(Ready_BackGround());
		FAILED_CHECK(Ready_GameObjects());
		FAILED_CHECK(Ready_StaticObject());
		

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
	Safe_Release(mWorldMap);

	if (mListVecTiles != nullptr)
	{
		for (auto& tile: *mListVecTiles)
		{
			Safe_Release(tile);
		}
		mListVecTiles->clear();
		Safe_Delete(mListVecTiles);
	}

	for (auto& obj : mStatic_InterActive_Objects)
	{
		Safe_Release(obj);
	}

	if (mListUnitDanumgeonMap)
	{
		mListUnitDanumgeonMap->clear();
		Safe_Delete(mListUnitDanumgeonMap);
	}
	if (mListUnitWorldMap)
	{
		mListUnitWorldMap->clear();
		Safe_Delete(mListUnitWorldMap);
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

	//LightDesc.eLightType = LIGHTDESC::TYPE_DIRECTIONAL;
	//LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	////LightDesc.vDiffuse = _float4(0.5f, 0.5f, 0.5f, 1.f);
	//LightDesc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 1.f);
	//LightDesc.vSpecular = _float4(0.3f, 0.3f, 0.3f, 1.f);

	//LightDesc.vDirection = _float4(1, -1.f, 1, 0.f);

	LightDesc.eLightType = LIGHTDESC::TYPE_DIRECTIONAL;
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.3f, 0.3f, 0.3f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);

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

	// ������ �� ����
	mDungeonMap = (CGameObject_MyTerrain*)pCreateManager->CreateEmptyObject(GAMEOBJECT_MYTERRAIN);
	NULL_CHECK_HR(mDungeonMap);
	Safe_AddRef(mDungeonMap);
	mDungeonMap->Set_MapType(CGameObject_MyTerrain::MAPTYPE_DUNGEON);
	mDungeonMap->Init_SetupInit();
	FAILED_CHECK(GetSingle(CGameInstance)->Push_Object(mCurrentLevel, TAGLAY(LAY_TERRAIN_DUNGEON), mDungeonMap));
	// �ʿ� Ÿ�� �߰�
	this->Setup_Terrain_Daungeon();

	// ����� ����
	mWorldMap = (CGameObject_MyTerrain*)pCreateManager->CreateEmptyObject(GAMEOBJECT_MYTERRAIN);
	NULL_CHECK_HR(mWorldMap);
	Safe_AddRef(mWorldMap);
	mWorldMap->Set_MapType(CGameObject_MyTerrain::MAPTYPE_WORLD);
	mWorldMap->Init_SetupInit();
	FAILED_CHECK(GetSingle(CGameInstance)->Push_Object(mCurrentLevel, TAGLAY(LAY_TERRAIN_WORLD), mWorldMap));
	this->Setup_Terrain_World();


	return S_OK;
}

HRESULT CDungeon_Objects::Ready_StaticObject()
{

	ZeroMemory(mStatic_InterActive_Objects, sizeof(CGameObject_3D_Static*)*STATICOBJECT_END);

	// �𵨸����� ��ü ��������
	mStatic_InterActive_Objects[STATICOBJECT_WORLDDOOR] = Get_ModelName("Entrance_Outside.fbx");
	mStatic_InterActive_Objects[STATICOBJECT_DUNGEONDOOR] = Get_ModelName("Entrance_Outside.fbx", mStatic_InterActive_Objects[STATICOBJECT_DUNGEONDOOR]);
	mStatic_InterActive_Objects[STATICOBJECT_HEART] = Get_ModelName("ThroneRoom_DungeonHeart.fbx");


	COLLIDER_DESC desc;
	desc.meColliderType = CCollider::COL_SPHERE;
	desc.mSize = _float3::One * 8;

	mStatic_InterActive_Objects[STATICOBJECT_DUNGEONDOOR]->Set_LoadColliderDESC(desc);
	mStatic_InterActive_Objects[STATICOBJECT_WORLDDOOR]->Set_LoadColliderDESC(desc);

	COLLIDER_DESC heartColDesc;
	heartColDesc.meColliderType = CCollider::COL_AABB;
	heartColDesc.mSize = _float3(1, 5.f, 1);

	mStatic_InterActive_Objects[STATICOBJECT_HEART]->Set_LoadColliderDESC(heartColDesc);

	for (auto& obj : mStatic_InterActive_Objects)
	{
		Safe_AddRef(obj);
	}

	return S_OK;
}

HRESULT CDungeon_Objects::Setup_Terrain_Daungeon(CGameObject_MyTerrain* map)
{
	if (mDungeonMap == nullptr)
	{
		mDungeonMap = map;
		Safe_AddRef(mDungeonMap);
	}

	if (mDungeonMap == nullptr)
		return E_FAIL;

	// Ÿ�� ����
	FAILED_CHECK(Create_Tiles(mCurrentLevel));
	// Ÿ�� �̿� ����
	FAILED_CHECK(Setup_Neigbor_Tile());
	// �׺�޽ÿ� Ÿ�� �浹
	FAILED_CHECK(Setup_Collision_Navi2Tile());
	// Ÿ�� �޽� ����
	FAILED_CHECK(Setup_TileState());
	return S_OK;
}

HRESULT CDungeon_Objects::Setup_Terrain_World(CGameObject_MyTerrain* map)
{
	if (mWorldMap == nullptr)
	{
		mWorldMap = map;
		Safe_AddRef(mWorldMap);
	}

	if (mWorldMap == nullptr)
		return E_FAIL;

	// �ݶ��̴��� �׺�޽� üũ 
	FAILED_CHECK(Setup_Collision_Navi2Object());
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

CGameObject_3D_Tile * CDungeon_Objects::Find_TileForIndex(_int TileIndex) const
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

	mListVecTiles->remove(pTIle);
	Safe_Release(pTIle);
	return S_OK;
}

HRESULT CDungeon_Objects::Create_Unit(E_TAYGAMEOBJECT id, _float3 PositionXZ)
{
	CGameObject_Creater* pCreateManager = GetSingle(CGameManager)->Get_CreaterManager();
	CGameObject_3D_Dynamic* mCreateUnit = nullptr;

	mCreateUnit = (CGameObject_3D_Dynamic*)pCreateManager->CreateEmptyObject(id);
	NULL_CHECK_HR(mCreateUnit);
	FAILED_CHECK(GetSingle(CGameInstance)->Push_Object(mCurrentLevel, TAGLAY(LAY_OBJECT_UNIT), mCreateUnit));
	return S_OK;
}

list<CGameObject_Base*> CDungeon_Objects::Get_ListObjecID(E_OBJECT_TYPE id)
{
	// ������Ʈ ID�� ����Ʈ
	list<CGameObject_Base*> ListObjectID;

	_uint idx = GetSingle(CGameManager)->Get_CurrentLevel();

	for (int i = 0; i < LAY_END; ++i)
	{
		if((E_TAYLAY)i == LAY_CAMERA)
			continue;

		const list<CGameObject*>* GameObjectList = GetSingle(CGameInstance)->Get_GameObjectLayerList(idx, TAGLAY((E_TAYLAY)i));
		if (GameObjectList == nullptr)
			continue;

		for (auto& obj: *GameObjectList)
		{
			CGameObject_Base* base = (CGameObject_Base*)obj;
			if (base == nullptr)
				continue;
			if (base->Get_ObjectTypeID() == id)
				ListObjectID.push_front(base);
		}
	}
	return ListObjectID;
}

CGameObject_3D_Static * CDungeon_Objects::Get_ModelName(string name, CGameObject_3D_Static* samename)
{
	auto staticlist = Get_ListObjecID(OBJECT_TYPE_3D_STATIC);

	bool match = false;
	for (auto& obj : staticlist)
	{
		match = (((CGameObject_3D_Static*)obj)->Get_ModelDESC().mModelName == name);

		if (match)
		{
			if (samename == obj)
				continue;
			else
				return (CGameObject_3D_Static*)obj;
		}
			
	}
	return nullptr;
}

list<CGameObject_3D_Dynamic*>* CDungeon_Objects::Get_UnitList_Dungeon()
{
	const list<CGameObject*>* listUnit = GetSingle(CGameManager)->Get_LevelObject_List(TAGLAY(LAY_OBJECT_UNIT));
	if (listUnit == nullptr)
		return nullptr;
	if (mListUnitDanumgeonMap == nullptr)
		mListUnitDanumgeonMap = NEW list<CGameObject_3D_Dynamic *>();

	mListUnitDanumgeonMap->clear();

	for (auto& obj: *listUnit)
	{
		auto oobj = static_cast<CGameObject_3D_Dynamic*>(obj);
		if (oobj->Get_CurrentMap() == CGameObject_3D_Dynamic::MAPTYPE_DUNGEON)
		{
			mListUnitDanumgeonMap->push_front(oobj);
		}
	}

	return mListUnitDanumgeonMap;
}

list<CGameObject_3D_Dynamic*>* CDungeon_Objects::Get_UnitList_World()
{
	const list<CGameObject*>* listUnit = GetSingle(CGameManager)->Get_LevelObject_List(TAGLAY(LAY_OBJECT_UNIT));
	if (listUnit == nullptr)
		return nullptr;
	if (mListUnitDanumgeonMap == nullptr)
		mListUnitDanumgeonMap = NEW list<CGameObject_3D_Dynamic *>();

	mListUnitDanumgeonMap->clear();

	for (auto& obj : *listUnit)
	{
		auto oobj = static_cast<CGameObject_3D_Dynamic*>(obj);
		if (oobj->Get_CurrentMap() == CGameObject_3D_Dynamic::MAPTYPE_WORLD)
		{
			mListUnitDanumgeonMap->push_front(oobj);
		}
	}

	return mListUnitDanumgeonMap;
}

HRESULT CDungeon_Objects::Create_Tiles(E_LEVEL level)
{
	// Ÿ�� ���� 
	_uint* TileXZ = mDungeonMap->Get_TerrainBuffer()->Get_XZ();
	mSizeX = TileXZ[0];
	mSizeZ = TileXZ[1];

	const TERRAIN_DESC* desc = &mDungeonMap->Get_TerrainDESC();

	if (mSizeX <= 0)
		return E_FAIL;

	if (mListVecTiles == nullptr)
		mListVecTiles = NEW list<CGameObject_3D_Tile*>;

	CGameObject_Creater* Create_Manager = GetSingle(CGameManager)->Get_CreaterManager();
	_uint levelindex = GetSingle(CGameInstance)->Get_CurrentLevelIndex();

	// ����� Ÿ�ϸ� ���� �����Ѵ�.

	for (_int z = 0; z < mSizeZ - 1; ++z)
	{
		for (_int x = 0; x < mSizeX - 1; ++x)
		{
			bool ismatch = false;
			_int iIndex = mDungeonMap->Get_TerrainBuffer()->Get_TilIndex(x, z);

			if (desc->mTileSize != 0)
			{
				for (int i = 0; i < desc->mTileSize; ++i)
				{
					if (desc->mArrayIndes[i] == iIndex)
					{
						ismatch = true;
						break;;
					}
				}
			}
			else
				ismatch = true;

			if(!ismatch)
				continue;

			CGameObject_3D_Tile* tileObj = (CGameObject_3D_Tile*)Create_Manager->CreateEmptyObject(GAMEOBJECT_3D_TILE);
			_float3 CreatePosition = mDungeonMap->Get_TerrainBuffer()->Get_TileWorldPos(x, z);

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

HRESULT CDungeon_Objects::Setup_Collision_Navi2Object()
{
	// ������Ʈ�� �׺�޽��� �浹 üũ
	CNavigation* navi = mWorldMap->Get_ComNavimesh();
	auto VecCell = navi->Get_CellVector();

	auto Objects = GetSingle(CGameManager)->Get_LevelObject_List(TAGLAY(LAY_OBJECT_W));
	if (Objects == nullptr)
		return S_FALSE;


	for (auto& obj : *Objects)
	{
		// #TODO: ������ Ÿ�� �̸� ����صα�..
		CCollider* objCollider = ((CGameObject_3D_Static*)obj)->Get_ComCollider();
		objCollider->Update_Transform(((CGameObject_3D_Static*)obj)->Get_ComTransform()->GetWorldFloat4x4());

		_float3 CellOffset = mWorldMap->Get_WorldPostition();
		for (auto& cell : *VecCell)
		{
			if(cell->Get_CellType() == CCell::CELLTYPE_STOP)
				continue;

			_float3 point1 = cell->Get_Point(CCell::POINT_A);
			_float3 point2 = cell->Get_Point(CCell::POINT_B);
			_float3 point3 = cell->Get_Point(CCell::POINT_C);

			
			point1 += CellOffset;
			point2 += CellOffset;
			point3 += CellOffset;

			if (objCollider->ColliderCheck(point1, point2, point3))
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
	CGameObject_3D_Tile* tile = Find_TileForIndex(index);
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
		for (auto& searchobj : *mListVecTiles)
		{
			// �밢�� ã��
			searchobj->Update_Tile_CrossCheck();
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
