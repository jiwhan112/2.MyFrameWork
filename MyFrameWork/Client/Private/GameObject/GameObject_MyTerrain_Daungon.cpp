#include "stdafx.h"
#include "GameObject/GameObject_MyTerrain_Daungon.h"

CGameObject_MyTerrain_Daungon::CGameObject_MyTerrain_Daungon(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_MyTerrain(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_TERRAIN;
}

CGameObject_MyTerrain_Daungon::CGameObject_MyTerrain_Daungon(const CGameObject_MyTerrain_Daungon& rhs)
	: CGameObject_MyTerrain(rhs)
{
	//	Safe_AddRef(mComVIBuffer);
	//	Safe_AddRef(mComTexture);
	//	Safe_AddRef(mComNaviMesh);
	//
	//	// DESC깊은 복사
	//	if (mTerrainDESC.mObjectSize != 0)
	//	{
	//		mTerrainDESC.mModelObjects = NEW MODEL_WORLD_DESC[mTerrainDESC.mObjectSize];
	//		for (int i = 0; i < mTerrainDESC.mObjectSize; ++i)
	//		{
	//			memcpy(&mTerrainDESC.mModelObjects[i], &(rhs.mTerrainDESC.mModelObjects[i]), sizeof(MODEL_WORLD_DESC));
	//		}
	//	}
	//
	//	mVecTile = nullptr;
}


HRESULT CGameObject_MyTerrain_Daungon::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());

	return S_OK;
}

HRESULT CGameObject_MyTerrain_Daungon::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));
	FAILED_CHECK(Set_Component());
	misPick = false;

	Init_Map(TAGLAY(LAY_OBJECT));


	return S_OK;
}

_int CGameObject_MyTerrain_Daungon::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));

	_float3 pick;

	// 네비 메시 충돌
	if (true == mComNaviMesh->Pick(mComTransform->GetWorldFloat4x4().Invert(), &pick))
	{
		Update_PickPos(pick);
	}

	
	return UPDATENONE;
}

_int CGameObject_MyTerrain_Daungon::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));

	mComRenderer->Add_RenderGroup(CRenderer::RENDER_NONBLEND_FIRST, this);
	return UPDATENONE;
}

HRESULT CGameObject_MyTerrain_Daungon::Render()
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

HRESULT CGameObject_MyTerrain_Daungon::Set_LoadTerrainDESC(const TERRAIN_DESC & desc)
{
	memcpy(&mTerrainDESC, &desc, sizeof(TERRAIN_DESC));

	return S_OK;
}

HRESULT CGameObject_MyTerrain_Daungon::Set_TerrainMode(E_TERRAINSIZE e)
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
		Update_TileVec(17, 17);
		
		mComNaviMesh->Load_NaviMeshData(datapath + mComNaviMesh->NAVI_FILENAME16);
		
		
		break;
	case TERRAINSIZE_32:
		mTerrainDESC.mTextureMultiSize = 32;
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_VIBUFFER_TERRAIN_32), TEXT("Com_VIBuffer"), (CComponent**)&mComVIBuffer));
		Update_TileVec(33, 33);
		mComNaviMesh->Load_NaviMeshData(datapath + mComNaviMesh->NAVI_FILENAME32);
		break;

	case TERRAINSIZE_64:
		mTerrainDESC.mTextureMultiSize = 64;
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_VIBUFFER_TERRAIN_64), TEXT("Com_VIBuffer"), (CComponent**)&mComVIBuffer));
		Update_TileVec(65, 65);
		mComNaviMesh->Load_NaviMeshData(datapath + mComNaviMesh->NAVI_FILENAME64);
		break;
	case TERRAINSIZE_128:
		mTerrainDESC.mTextureMultiSize = 128;
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_VIBUFFER_TERRAIN_128), TEXT("Com_VIBuffer"), (CComponent**)&mComVIBuffer));
		Update_TileVec(129, 129);

		break;
	case TERRAINSIZE_END:
		break;
	default:
		break;
	}

	return S_OK;
}

int CGameObject_MyTerrain_Daungon::Get_TileIndex(_float3 worldPos)
{
	return mComVIBuffer->Get_TileIndex(worldPos);
}

_float3 CGameObject_MyTerrain_Daungon::Get_TileWorld(_uint index)
{
	return mComVIBuffer->Get_TileWorldPos(index);
}

HRESULT CGameObject_MyTerrain_Daungon::Set_Component()
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
		Set_TerrainMode(TERRAINSIZE_16);
	}

	return S_OK;
}

HRESULT CGameObject_MyTerrain_Daungon::Set_ConstantTable_Tex()
{
	FAILED_CHECK(mComTexture->SetUp_OnShader(mComShader, STR_TEX_DIFFUSE, 0));
	FAILED_CHECK(mComShader->Set_RawValue(STR_TEXTURESIZE, &mTerrainDESC.mTextureMultiSize, sizeof(_uint)));
	return S_OK;
}

HRESULT CGameObject_MyTerrain_Daungon::Init_Map(const _tchar* layertag)
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
		string str = mTerrainDESC.mModelObjects[i].mProtoName;
		wstring wstr = CHelperClass::Convert_str2wstr(str);
		CGameObject_Base* cloneObject = creater->Create_ObjectClone_Prefab(idx, wstr, layertag);
		if (cloneObject == nullptr)
			return E_FAIL;
		_float4x4 worlmat = mTerrainDESC.mModelObjects[i].mWorldMat;
		cloneObject->Get_TransformCom()->Set_WorldMat(worlmat);
	}
	return S_OK;
}

HRESULT CGameObject_MyTerrain_Daungon::SaveDESC_Objects(const list<MODEL_WORLD_DESC>& worldObjList)
{
	// worldMat
	int size =  worldObjList.size();
	if (size <= 0)
		return E_FAIL;

	if (mTerrainDESC.mObjectSize != 0)
	{
		Safe_Delete_Array(mTerrainDESC.mModelObjects); 
		mTerrainDESC.mObjectSize = 0;
	}
	
	mTerrainDESC.mObjectSize = size;
	MODEL_WORLD_DESC* NewObjects = NEW MODEL_WORLD_DESC[size];
	int count = 0;
	for (auto& staticobj: worldObjList)
	{
		strcpy_s(NewObjects[count].mProtoName, staticobj.mProtoName);
		NewObjects[count].mWorldMat = staticobj.mWorldMat;
		count++;
	}
	mTerrainDESC.mModelObjects = NewObjects;

	return S_OK;
}


void CGameObject_MyTerrain_Daungon::Update_PickPos(_float3 pickPos)
{
	misPick = true;
	// 피킹시 게임 매니저에 전달??
	_uint iIndex = mComVIBuffer->Get_TileIndex(pickPos);
	mPickWorldPos = mComVIBuffer->Get_TileWorldPos(iIndex);
}

void CGameObject_MyTerrain_Daungon::Update_TileVec(int xx, int zz)
{
	if (mVecTile == nullptr)
		mVecTile = NEW vector<MYTILE *>;
	else
	{
		for (auto tile : *mVecTile)
		{
			Safe_Delete(tile);
		}
		mVecTile->clear();
	}

	mVecTile->reserve(xx*zz);

	for (int z = 0; z < zz; ++z)
	{
		for (int x = 0; x < xx; ++x)
		{
			int iIndex = z * xx + x;
			MYTILE* newtile = NEW MYTILE(iIndex, TILEMODE_NONE);
			mVecTile->push_back(newtile);
		}
	}
}

CGameObject_MyTerrain_Daungon * CGameObject_MyTerrain_Daungon::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CGameObject_MyTerrain_Daungon*	pInstance = NEW CGameObject_MyTerrain_Daungon(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Creating CGameObject_MyTerrain_Daungon");
		Safe_Release(pInstance);
	}

	return pInstance;
}
CGameObject_MyTerrain_Daungon* CGameObject_MyTerrain_Daungon::Clone(void* pArg)
{
	CGameObject_MyTerrain_Daungon*	pInstance = NEW CGameObject_MyTerrain_Daungon(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Creating CGameObject_MyTerrain_Daungon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameObject_MyTerrain_Daungon::Free()
{
	__super::Free();
	Safe_Release(mComVIBuffer);
	Safe_Release(mComTexture);
	Safe_Release(mComNaviMesh);

	if (mVecTile == nullptr)
		return;

	for (auto tile : *mVecTile)
	{
		Safe_Delete(tile);
	}
	mVecTile->clear();
	Safe_Delete(mVecTile);

	
	if (mTerrainDESC.mModelObjects != nullptr)
	{
		Safe_Delete_Array(mTerrainDESC.mModelObjects);
	}
	
}

HRESULT CGameObject_MyTerrain_Daungon::Update_TerrainTile()
{
	_uint levelIndex = GetSingle(CGameInstance)->Get_CurrentLevelIndex();

	auto baselist = GetSingle(CGameManager)->Get_LevelObjectList(TAGLAY(LAY_CUBETILE));

	return S_OK;
}
