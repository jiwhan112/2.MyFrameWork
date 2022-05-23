#include "stdafx.h"
#include "GameObject/GameObject_MyTerrain.h"

CGameObject_MyTerrain::CGameObject_MyTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_Base(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_TERRAIN;
}

CGameObject_MyTerrain::CGameObject_MyTerrain(const CGameObject_MyTerrain& rhs)
	: CGameObject_Base(rhs)
	, mTerrainDESC(rhs.mTerrainDESC)
{

	// Desc 내부의 동적오브젝트는 Load 함수호출로 생성;
}

HRESULT CGameObject_MyTerrain::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());

	return S_OK;
}

HRESULT CGameObject_MyTerrain::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));
	misPick = false;
	Init_Map(TAGLAY(LAY_OBJECT));


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

	mComTexture->SetUp_OnShader(mComShader, STR_TEX_DIFFUSE, 0);
	mComVIBuffer->Render(mComShader, 0);

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
		mTerrainDESC.mTextureMultiSize = 32;
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_VIBUFFER_TERRAIN_32), TEXT("Com_VIBuffer"), (CComponent**)&mComVIBuffer));
		mComNaviMesh->Load_NaviMeshData(datapath + mComNaviMesh->NAVI_FILENAME32);
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

HRESULT CGameObject_MyTerrain::Set_Component()
{
	if (mComRenderer == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_RENDERER), TEXT("Com_Renderer"), (CComponent**)&mComRenderer));

	// 모델 타입에 따라 정적모델 동적모델 처리
	if (mComShader == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_VTXNORTEX), TEXT("Com_Shader"), (CComponent**)&mComShader));

	if (mComTexture == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_DEFAULT_FLOOR), TEXT("Com_Texture"), (CComponent**)&mComTexture));

	if (mComNaviMesh == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_NAVIMESH), TEXT("Com_Navimesh"), (CComponent**)&mComNaviMesh));

	if (mComVIBuffer == nullptr)
	{
		Set_TerrainMode(TERRAINSIZE_32);
	}

	return S_OK;
}

HRESULT CGameObject_MyTerrain::Set_ConstantTable_Tex()
{
	FAILED_CHECK(mComTexture->SetUp_OnShader(mComShader, STR_TEX_DIFFUSE, 0));
	FAILED_CHECK(mComShader->Set_RawValue(STR_TEXTURESIZE, &mTerrainDESC.mTextureMultiSize, sizeof(_uint)));
	return S_OK;
}

HRESULT CGameObject_MyTerrain::Init_Map(const _tchar* layertag)
{

	if (mTerrainDESC.mObjectSize <= 0)
		return S_FALSE;
	Set_TerrainMode(mTerrainDESC.meTerrainSize);



	// static2 형태의 객체만 생성.
	// 정보만 DESC로 읽고 해당 레이어에 생성한다.
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
	mTerrainDESC.mNoTileSize = 0;
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

		mTerrainDESC.mNoTileSize = tilesize;
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


void CGameObject_MyTerrain::Update_PickPos(_float3 pickPos)
{
	misPick = true;
	// 피킹시 게임 매니저에 전달??
	_uint iIndex = mComVIBuffer->Get_TileIndex(pickPos);
	mPickWorldPos = mComVIBuffer->Get_TileWorldPos(iIndex);
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
	Safe_Release(mComTexture);
	Safe_Release(mComNaviMesh);
	
}