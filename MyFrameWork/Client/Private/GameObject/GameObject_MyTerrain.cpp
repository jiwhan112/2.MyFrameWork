#include "stdafx.h"
#include "GameObject/GameObject_MyTerrain.h"

const wchar_t* STR_PATH_HEIGHTMAP_64 = L"../Bin/Resources/Sprite/HeightMap_64.bmp";
const wchar_t* STR_PATH_HEIGHTMAP_32 = L"../Bin/Resources/Sprite/HeightMap_32.bmp";
CGameObject_MyTerrain::CGameObject_MyTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_Base(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_TERRAIN;
	meMapType = CGameObject_MyTerrain::MAPTYPE_END;
}

CGameObject_MyTerrain::CGameObject_MyTerrain(const CGameObject_MyTerrain& rhs)
	: CGameObject_Base(rhs)
{
}

HRESULT CGameObject_MyTerrain::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());

	return S_OK;
}

HRESULT CGameObject_MyTerrain::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));

	return S_OK;
}

HRESULT CGameObject_MyTerrain::Init_SetupInit()
{
	// DESC정보 초기화
	if (meMapType == CGameObject_MyTerrain::MAPTYPE_DUNGEON)
	{
		mStrMapDatName = "DungeonBaseMap.terrdat";
		wstring wstr = CHelperClass::Convert_str2wstr(mStrMapDatName);
		TERRAIN_DESC* pTerrainDESC = GetSingle(CGameManager)->Get_ObjectIOManager()->Find_TerrainDesc(wstr);
	//	pTerrainDESC->mTileSize = 0;
		Set_LoadTerrainDESC(*pTerrainDESC);


	}
	else if (meMapType == CGameObject_MyTerrain::MAPTYPE_WORLD)
	{
		mStrMapDatName = "WorldBaseMap.terrdat";
	}

	// Terrain 생성
	Init_Map(TAGLAY(LAY_OBJECT));
	// LoadTex2Name
	LoadTextureMap();
	
	return S_OK;
}

_int CGameObject_MyTerrain::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));

	// 지형이라면 네비메시 타입으로 충돌체를 넣는다.

	// 네비게이셔 충돌 기능 -> 충돌 매니저로 옮기자
	GetSingle(CGameManager)->Add_ColliderObject(CColliderManager::COLLIDEROBJ_TERRAIN, this);


	// 네비 메시 충돌
	//if (true == mComNaviMesh->Pick(mComTransform->GetWorldFloat4x4().Invert(), &pick))
	//{
	//	Update_PickPos(pick);
	//}

	return UPDATENONE;
}

_int CGameObject_MyTerrain::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));
	_float3 pos =  GetSingle(CGameManager)->Get_PickPos();

	mComRenderer->Add_RenderGroup(CRenderer::RENDER_NONBLEND_FIRST, this);
	return UPDATENONE;
}

HRESULT CGameObject_MyTerrain::Render()
{
	if (__super::Render() == S_FALSE)
	{
		return S_FALSE;
	}

	FAILED_CHECK(Set_ConstantTable_World());
	FAILED_CHECK(Set_ConstantTable_Tex());
	FAILED_CHECK(Set_ConstantTable_Light());

	if (meMapType == CGameObject_MyTerrain::MAPTYPE_WORLD)
		mCurrentShaderPass = 1;
	else 
		mCurrentShaderPass = 0;

	mComVIBuffer->Render(mComShader, mCurrentShaderPass);

#ifdef _DEBUG
	mComNaviMesh->Render(mComTransform);
#endif // _DEBUG
	return S_OK;
}

HRESULT CGameObject_MyTerrain::Set_LoadTerrainDESC(const TERRAIN_DESC & desc)
{
	memcpy(&mTerrainDESC, &desc, sizeof(TERRAIN_DESC));
	return S_OK;
}

HRESULT CGameObject_MyTerrain::Set_TerrainMode(E_TERRAINSIZE e)
{
	if (mTerrainDESC.meTerrainSize != e)
		mTerrainDESC.meTerrainSize = e;

	// 해당 모델 컴포넌트로 변경
	if (mComVIBuffer != nullptr)
	{
		Safe_Release(mComVIBuffer);
		mComVIBuffer = nullptr;
	}

	FAILED_CHECK(__super::Release_Component(TEXT("Com_VIBuffer")));
	wstring datapath = STR_FILEPATH_RESOURCE_DAT_L;
	datapath += L"\\";

	switch (mTerrainDESC.meTerrainSize)
	{
	case TERRAINSIZE_16:
		mTerrainDESC.mTextureMultiSize = 16;
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_VIBUFFER_TERRAIN_16), TEXT("Com_VIBuffer"), (CComponent**)&mComVIBuffer));
		mComNaviMesh->Load_NaviMeshData(datapath + mComNaviMesh->NAVI_FILENAME16);
		break;

	case TERRAINSIZE_32:
		if (meMapType == CGameObject_MyTerrain::MAPTYPE_DUNGEON)
		{
			mTerrainDESC.mTextureMultiSize = 10;
			FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_VIBUFFER_TERRAIN_32), TEXT("Com_VIBuffer"), (CComponent**)&mComVIBuffer));
			mComNaviMesh->Load_NaviMeshData(datapath + mComNaviMesh->NAVI_FILENAME32);
		}
		else if (meMapType == CGameObject_MyTerrain::MAPTYPE_WORLD)
		{
			mTerrainDESC.mTextureMultiSize = 10;
			FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_VIBUFFER_TERRAIN_32_WORLD), TEXT("Com_VIBuffer"), (CComponent**)&mComVIBuffer));
			// 높이맵 적용된 네비메시 설정
			mComNaviMesh->Load_NaviMeshData(datapath + mComNaviMesh->NAVI_FILENAME32);
			Set_HeightNewMap();
		}
		break;

	case TERRAINSIZE_64:
		mTerrainDESC.mTextureMultiSize = 64;
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_VIBUFFER_TERRAIN_64), TEXT("Com_VIBuffer"), (CComponent**)&mComVIBuffer));
		mComNaviMesh->Load_NaviMeshData(datapath + mComNaviMesh->NAVI_FILENAME64);
		break;

	case TERRAINSIZE_128:
		mTerrainDESC.mTextureMultiSize = 128;
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_VIBUFFER_TERRAIN_128), TEXT("Com_VIBuffer"), (CComponent**)&mComVIBuffer));
		// Update_TileVec(129, 129);
		break;
	case TERRAINSIZE_END:
		break;
	default:
		break;
	}

	return S_OK;
}

int CGameObject_MyTerrain::Get_TileIndex(_float3 worldPos)
{
	
	return mComVIBuffer->Get_TileIndex(worldPos);
}

_float3 CGameObject_MyTerrain::Get_TileWorld(_uint index)
{
	return mComVIBuffer->Get_TileWorldPos(index);
}

_uint CGameObject_MyTerrain::GetMapSize()
{
	switch (mTerrainDESC.meTerrainSize)
	{
	case TERRAINSIZE_16:
		return 16;
	case TERRAINSIZE_32:
		return 32;
	case TERRAINSIZE_64:
		return 64;
	case TERRAINSIZE_128:
		return 128;
	case TERRAINSIZE_END:
		return 0;
	default:
		return 0;
	}
	
	return 0;
}

void CGameObject_MyTerrain::Set_ColorFiter(E_SOURCETYPE type, _color * color, _float val)
{
	switch (type)
	{
	case Client::CGameObject_MyTerrain::SOURCE_A:
		color->w = val;
		break;
	case Client::CGameObject_MyTerrain::SOURCE_R:
		color->x = val;

		break;
	case Client::CGameObject_MyTerrain::SOURCE_G:
		color->y = val;

		break;
	case Client::CGameObject_MyTerrain::SOURCE_B:
		color->z = val;

		break;
	case Client::CGameObject_MyTerrain::SOURCE_END:
		break;
	default:
		break;
	}
}

HRESULT CGameObject_MyTerrain::SaveCurrentFiterMap()
{
	// 필터 텍스처 저장
	if (mFiterTexture == nullptr)
		return E_FAIL;

	FAILED_CHECK(SaveWICTextureToFile(m_pDeviceContext, mFiterTexture, GUID_ContainerFormatPng, FILEPATH_FILTERTEXTURE, &GUID_WICPixelFormat32bppBGRA));
	return S_OK;
}

HRESULT CGameObject_MyTerrain::UpdateFiterTextue()
{
	// 필터 텍스처 생성및 셰이더로 넘김
	_uint size = GetMapSize();
	
	
	// 필터 텍스처 생성
	if (mFiterTexture == nullptr)
	{
		D3D11_TEXTURE2D_DESC		TextureDesc;
		ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

		TextureDesc.Width = size;
		TextureDesc.Height = size;
		TextureDesc.MipLevels = 1;
		TextureDesc.ArraySize = 1;
		TextureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		TextureDesc.SampleDesc.Count = 1;
		TextureDesc.SampleDesc.Quality = 0;
		TextureDesc.Usage = D3D11_USAGE_DYNAMIC;
		TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA			SubResourceData;
		ZeroMemory(&SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
		_ulong*		pPixel = new _ulong[size * size];
		ZeroMemory(pPixel, sizeof(_ulong) * size * size);

		for (_uint i = 0; i < size; ++i)
		{
			for (_uint j = 0; j < size; ++j)
			{
				_uint iIndex = i * size + j;
				pPixel[iIndex] = D3D11COLOR_ARGB(0, 0, 0, 0);
			}
		}
		SubResourceData.pSysMem = pPixel;
		SubResourceData.SysMemPitch = sizeof(_ulong) * size;

		FAILED_CHECK(m_pDevice->CreateTexture2D(&TextureDesc, &SubResourceData, &mFiterTexture));
		Safe_Delete_Array(pPixel);
	}
	

	// 변경 
	// Map unMap
	D3D11_MAPPED_SUBRESOURCE		SubResource;
	ZeroMemory(&SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	mPixels = new _ulong[size * size];
	ZeroMemory(mPixels, sizeof(_ulong) * size * size);

	m_pDeviceContext->Map(mFiterTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource);
	
	for (_uint i = 0; i < size; ++i)
	{
		for (_uint j = 0; j < size; ++j)
		{
			// 필터맵 초기화
			/*	int randA = CHelperClass::RandomInt(0, 255);
				int randR = CHelperClass::RandomInt(0, 255);
				int randG = CHelperClass::RandomInt(0, 255);
				int randB = CHelperClass::RandomInt(0, 255);*/

			// 초기화

			_uint iIndex = i * size + j;
			_color coloirValue = *(_color*)&mPixels[iIndex];
		//	coloirValue = D3D11COLOR_ARGB(0, 0, 0, 0);
			coloirValue = _color(0, 0, 0, 0);
			mPixels[iIndex] = *(_ulong*)&coloirValue;
		}
	}


	memcpy(SubResource.pData, mPixels, sizeof(_ulong) * size*size);
	m_pDeviceContext->Unmap(mFiterTexture, 0);

	// 셰이더 리소스뷰를 만들어서 사용할 수 있다.
	Safe_Release(mFiterSRV);
	if (FAILED(m_pDevice->CreateShaderResourceView(mFiterTexture, nullptr, &mFiterSRV)))
		return E_FAIL;

	return S_OK;

}

HRESULT CGameObject_MyTerrain::UpdateFiterTextue_TOOL(E_SOURCETYPE type, _float3 worldPos, _float Range, _uint value)
{
	if (mFiterTexture == nullptr)
		return E_FAIL;
	_uint size = GetMapSize();


	// 피킹 위치를 지점으로 해당 텍스처 블랜드
	D3D11_MAPPED_SUBRESOURCE		SubResource;
	ZeroMemory(&SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	m_pDeviceContext->Map(mFiterTexture,0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource);
	// distance로 판단해서 색을 넣어준다.
	_uint PickTileIndex = Get_TileIndex(worldPos);
	for (_uint i = 0; i < size; ++i)
	{
		for (_uint j = 0; j < size; ++j)
		{
			_uint iIndex = i * size + j;

			_color coloirValue = *(_color*)&mPixels[iIndex];
			if (iIndex == PickTileIndex)
				Set_ColorFiter(type, &coloirValue, value);

			//_float distance = _float3::Distance(CurrentTilePos, worldPos);

			//if (distance < 0.5f)
			//{
			//	Set_ColorFiter(type, &coloirValue, value);
			////	Set_ColorFiter(type, &coloirValue, value*((Range - distance) / Range));
			//}

			_ulong color = *(_ulong*)&coloirValue.RGBA();
			mPixels[iIndex] = color;
		}
	}

	memcpy(SubResource.pData, mPixels, sizeof(_ulong) * size*size);
	m_pDeviceContext->Unmap(mFiterTexture, 0);

	// 셰이더 리소스뷰를 만들어서 사용할 수 있다.
	Safe_Release(mFiterSRV);
	if (FAILED(m_pDevice->CreateShaderResourceView(mFiterTexture, nullptr, &mFiterSRV)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGameObject_MyTerrain::Set_Component()
{
	if (mComRenderer == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_RENDERER), TEXT("Com_Renderer"), (CComponent**)&mComRenderer));

	// 모델 타입에 따라 정적모델 동적모델 처리
	if (mComShader == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_TERRAIN), TEXT("Com_Shader"), (CComponent**)&mComShader));

	
	if (mComNaviMesh == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_NAVIMESH), TEXT("Com_Navimesh"), (CComponent**)&mComNaviMesh));

	if (mComVIBuffer == nullptr)
	{
		Set_TerrainMode(TERRAINSIZE_32);
	}
	if (mComDefaultTex == nullptr)
	{
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_MAP), TEXT("Com_Texture"), (CComponent**)&mComDefaultTex));
		
	}

	return S_OK;
}

HRESULT CGameObject_MyTerrain::Set_ConstantTable_Tex()
{
	// Diffuse
	FAILED_CHECK(mComDefaultTex->SetUp_OnShader(mComShader, STR_TEX_DIFFUSE));
	FAILED_CHECK(mComShader->Set_RawValue(STR_TEXTURESIZE, &mTerrainDESC.mTextureMultiSize, sizeof(_float)));

	// Source
	if (meMapType == CGameObject_MyTerrain::MAPTYPE_WORLD)
	{

		// 이미지에 따라 세팅
		FAILED_CHECK(mComFiterSourceTex[SOURCE_A]->SetUp_OnShader(mComShader, "g_SourDiffuseTexture1"));
		FAILED_CHECK(mComFiterSourceTex[SOURCE_R]->SetUp_OnShader(mComShader, "g_SourDiffuseTexture2"));
		FAILED_CHECK(mComFiterSourceTex[SOURCE_G]->SetUp_OnShader(mComShader, "g_SourDiffuseTexture3"));
		FAILED_CHECK(mComFiterSourceTex[SOURCE_B]->SetUp_OnShader(mComShader, "g_SourDiffuseTexture4"));

		// Fiter_XYZ Brush
		if (mFiterSRV)
		{
			FAILED_CHECK(mComShader->Set_Texture("g_FilterTexture", mFiterSRV));
		}
		else if (mComFiter_XYZW)
		{
			FAILED_CHECK(mComFiter_XYZW->SetUp_OnShader(mComShader, "g_FilterTexture"));
		}

		FAILED_CHECK(mComBrushTex->SetUp_OnShader(mComShader, "g_BrushTexture"));

		// Value
		_float3 worldPickPos = GetSingle(CGameManager)->Get_PickPos();
		FAILED_CHECK(mComShader->Set_RawValue("g_vBrushPos", &worldPickPos, sizeof(_float3)));
		FAILED_CHECK(mComShader->Set_RawValue("g_fRadius", &mRadius, sizeof(_float)));
	}
	return S_OK;
}

HRESULT CGameObject_MyTerrain::LoadTextureMap()
{
	// Texture 이름으로 로드
	if (meMapType == CGameObject_MyTerrain::MAPTYPE_DUNGEON)
	{
		strcpy_s(mTextureNameDesc.mTextureKey_Diffuse, "FLOOR1.dds");
	}

	else if (meMapType == CGameObject_MyTerrain::MAPTYPE_WORLD)
	{
		// 컴포넌트 복제
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_MAP), TEXT("Com_Fiter1"), (CComponent**)&mComFiterSourceTex[SOURCE_A]));
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_MAP), TEXT("Com_Fiter2"), (CComponent**)&mComFiterSourceTex[SOURCE_R]));
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_MAP), TEXT("Com_Fiter3"), (CComponent**)&mComFiterSourceTex[SOURCE_G]));
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_MAP), TEXT("Com_Fiter4"), (CComponent**)&mComFiterSourceTex[SOURCE_B]));

		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_MAP), TEXT("Com_XYZW"), (CComponent**)&mComFiter_XYZW));
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_MAP), TEXT("Com_Brush"), (CComponent**)&mComBrushTex));


		strcpy_s(mTextureNameDesc.mTextureKey_Diffuse, "GROUND1.dds");
		strcpy_s(mTextureNameDesc.mTextureKey_01, "GRASS.dds");
		strcpy_s(mTextureNameDesc.mTextureKey_02, "GROUNDTILE1.dds");
		strcpy_s(mTextureNameDesc.mTextureKey_03, "GROUNDTILE2.dds");
		strcpy_s(mTextureNameDesc.mTextureKey_04, "GROUNDTILE3.dds");

		// Fiter_XYZ Brush
		strcpy_s(mTextureNameDesc.mTextureKey_05, "MyFilter.png");
		strcpy_s(mTextureNameDesc.mTextureKey_06, "Brush.png");

		FAILED_CHECK(mComFiterSourceTex[SOURCE_A]->Set_TextureMap(mTextureNameDesc.mTextureKey_01));
		FAILED_CHECK(mComFiterSourceTex[SOURCE_R]->Set_TextureMap(mTextureNameDesc.mTextureKey_02));
		FAILED_CHECK(mComFiterSourceTex[SOURCE_G]->Set_TextureMap(mTextureNameDesc.mTextureKey_03));
		FAILED_CHECK(mComFiterSourceTex[SOURCE_B]->Set_TextureMap(mTextureNameDesc.mTextureKey_04));

		FAILED_CHECK(mComFiter_XYZW->Set_TextureMap(mTextureNameDesc.mTextureKey_01));
		FAILED_CHECK(mComBrushTex->Set_TextureMap(mTextureNameDesc.mTextureKey_06));
	}

	// LOAD TEX
	FAILED_CHECK(mComDefaultTex->Set_TextureMap(mTextureNameDesc.mTextureKey_Diffuse));
	return S_OK;
}

HRESULT CGameObject_MyTerrain::Set_HeightNewMap()
{
	_float3 WorldMapPos = _float3(0, 10, 0);

	Set_Position(WorldMapPos);
	// 버퍼 수정
	FAILED_CHECK(mComVIBuffer->Set_HeightMap(STR_PATH_HEIGHTMAP_32));
	FAILED_CHECK(mComNaviMesh->SetUp_CurrentPoint(mComVIBuffer));

	return S_OK;
}

_float CGameObject_MyTerrain::Get_HeightY(_float3 PositionXZ)
{
	_float newY = -1;
	if (mComVIBuffer == nullptr)
		return newY;

	newY =  mComVIBuffer->Get_HeightY(PositionXZ);
	
	newY += Get_WorldPostition().y;
	return newY;
}

HRESULT CGameObject_MyTerrain::Init_Map(const _tchar* layertag)
{
	
	// 맵생성
	Set_TerrainMode(mTerrainDESC.meTerrainSize);

	if (mTerrainDESC.mObjectSize <= 0)
		return S_FALSE;

	// 저장된 오브젝트 생성
	CGameObject_Creater* creater = GetSingle(CGameManager)->Get_CreaterManager();
	_uint idx = GetSingle(CGameInstance)->Get_CurrentLevelIndex();

	for (int i = 0; i < mTerrainDESC.mObjectSize; ++i)
	{
		string str = mTerrainDESC.mArrayModelObjects[i].mProtoName;
		wstring wstr = CHelperClass::Convert_str2wstr(str);
		CGameObject_Base* cloneObject = creater->Create_ObjectClone_Prefab(idx, wstr, layertag);
		if (cloneObject == nullptr)
			return E_FAIL;
		_float4x4 worlmat = mTerrainDESC.mArrayModelObjects[i].mWorldMat;
		cloneObject->Get_ComTransform()->Set_WorldMat(worlmat);
	}

	return S_OK;
}

HRESULT CGameObject_MyTerrain::SaveDESC_Objects(const list<_uint>& uintList, const list<MODEL_WORLD_DESC>& worldObjList)
{
	//Safe_Delete_Array(mTerrainDESC.mArrayModelObjects);
	//Safe_Delete_Array(mTerrainDESC.mArrayIndes);
	mTerrainDESC.mTileSize = 0;
	mTerrainDESC.mObjectSize = 0;

	// 정보 업데이트 및 저장
	int tilesize = uintList.size();
	int objectsize = worldObjList.size();

	if (tilesize > 0)
	{
		_uint* NoTileArray = NEW _uint[tilesize];

		_uint count = 0;
		for (auto& index : uintList)
		{
			NoTileArray[count]= index;
			count++;
		}

		mTerrainDESC.mTileSize = tilesize;
		mTerrainDESC.mArrayIndes = NoTileArray;
	}

	if (objectsize > 0)
	{
		mTerrainDESC.mObjectSize = objectsize;
		MODEL_WORLD_DESC* ObjectsArray = NEW MODEL_WORLD_DESC[objectsize];

		_uint count = 0;
		for (auto& staticobj : worldObjList)
		{
			strcpy_s(ObjectsArray[count].mProtoName, staticobj.mProtoName);
			ObjectsArray[count].mWorldMat = staticobj.mWorldMat;
			count++;
		}

		mTerrainDESC.mObjectSize = objectsize;
		mTerrainDESC.mArrayModelObjects = ObjectsArray;
	}

	return S_OK;
}

CGameObject_MyTerrain * CGameObject_MyTerrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_MyTerrain*	pInstance = NEW CGameObject_MyTerrain(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_MyTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject_MyTerrain* CGameObject_MyTerrain::Clone(void* pArg)
{
	CGameObject_MyTerrain*	pInstance = NEW CGameObject_MyTerrain(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_MyTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_MyTerrain::Free()
{
	__super::Free();
	Safe_Release(mComVIBuffer);
	Safe_Release(mComNaviMesh);
	
	Safe_Release(mFiterTexture);
	Safe_Release(mFiterSRV);
	Safe_Delete_Array(mPixels);

	

	// TEX
	Safe_Release(mComDefaultTex);
	Safe_Release(mComFiterSourceTex[SOURCE_A]);
	Safe_Release(mComFiterSourceTex[SOURCE_R]);
	Safe_Release(mComFiterSourceTex[SOURCE_G]);
	Safe_Release(mComFiterSourceTex[SOURCE_B]);

	Safe_Release(mComFiter_XYZW);
	Safe_Release(mComBrushTex);
	
}