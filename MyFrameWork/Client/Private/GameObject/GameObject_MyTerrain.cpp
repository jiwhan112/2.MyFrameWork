#include "stdafx.h"
#include "GameObject/GameObject_MyTerrain.h"

const wchar_t* STR_PATH_HEIGHTMAP_64 = L"../Bin/Resources/Sprite/HeightMap_64.bmp";
CGameObject_MyTerrain::CGameObject_MyTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CGameObject_Base(pDevice, pDeviceContext)
{
	mObjectTypeid = (int)E_OBJECT_TYPE::OBJECT_TYPE_TERRAIN;
}

CGameObject_MyTerrain::CGameObject_MyTerrain(const CGameObject_MyTerrain& rhs)
	: CGameObject_Base(rhs)
	, mTerrainDESC(rhs.mTerrainDESC)
{

	// Desc ������ ����������Ʈ�� Load �Լ�ȣ��� ����;
}

HRESULT CGameObject_MyTerrain::NativeConstruct_Prototype()
{
	FAILED_CHECK(__super::NativeConstruct_Prototype());

	return S_OK;
}

HRESULT CGameObject_MyTerrain::NativeConstruct(void* pArg)
{
	FAILED_CHECK(__super::NativeConstruct(pArg));
	Init_Map(TAGLAY(LAY_OBJECT));

	return S_OK;
}

_int CGameObject_MyTerrain::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));

	// �����̶�� �׺�޽� Ÿ������ �浹ü�� �ִ´�.

	// �׺���̼� �浹 ��� -> �浹 �Ŵ����� �ű���
	GetSingle(CGameManager)->Add_ColliderObject(CColliderManager::COLLIDEROBJ_TERRAIN, this);


	// �׺� �޽� �浹
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

	mCurrentShaderPass = 1;

	mComTexture->SetUp_OnShader(mComShader, STR_TEX_DIFFUSE, 0);
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

	// �ش� �� ������Ʈ�� ����
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

HRESULT CGameObject_MyTerrain::CreateFiterTexture()
{
	_uint size = 0;
	switch (mTerrainDESC.meTerrainSize)
	{
	case TERRAINSIZE_16:
		size = 17;
		break;
	case TERRAINSIZE_32:
		size = 33;
		break;
	case TERRAINSIZE_64:
		size = 65;
		break;
	case TERRAINSIZE_128:
		size = 129;
		break;
	default:
		return E_FAIL;
	}

	// ���� �ؽ�ó �����
	// ���� ������ ��� �����.
	ID3D11Texture2D*			pTexture;

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

	int half = size *0.5f;
	for (_uint i = 0; i < size; ++i)
	{
		for (_uint j = 0; j < size; ++j)
		{
			_uint iIndex = i * size + j;
			
			// ����� ���� �ؽ�ó ��
			if (j < half)
				pPixel[iIndex] = D3D11COLOR_ARGB(255, 255, 255, 255);
			else
				pPixel[iIndex] = D3D11COLOR_ARGB(255, 0, 0, 0);
		}
	}


	SubResourceData.pSysMem = pPixel;
	SubResourceData.SysMemPitch = sizeof(_ulong) * size;

	// �ؽ�ó�� ����� ShaderResourceView�� �����.

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, &SubResourceData, &pTexture)))
		return E_FAIL;

	// ���̴� ���ҽ��並 ���� ����� �� �ִ�.
	/*ID3D11ShaderResourceView* SRV = nullptr;
	if (FAILED(m_pDevice->CreateShaderResourceView(pTexture, nullptr, &SRV)))
		return E_FAIL;
	if (SRV)
		Safe_Release(SRV);*/

	if (FAILED(SaveWICTextureToFile(m_pDeviceContext, pTexture, GUID_ContainerFormatPng, FILEPATH_FILTERTEXTURE, &GUID_WICPixelFormat32bppBGRA)))
		return E_FAIL;

	Safe_Release(pTexture);

	Safe_Delete_Array(pPixel);
	return S_OK;
}

HRESULT CGameObject_MyTerrain::Set_Component()
{
	if (mComRenderer == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_RENDERER), TEXT("Com_Renderer"), (CComponent**)&mComRenderer));

	// �� Ÿ�Կ� ���� ������ ������ ó��
	if (mComShader == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_SHADER_TERRAIN), TEXT("Com_Shader"), (CComponent**)&mComShader));

	if (mComTexture == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_DEFAULT_FLOOR), TEXT("Com_Texture"), (CComponent**)&mComTexture));

	if (mComNaviMesh == nullptr)
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_NAVIMESH), TEXT("Com_Navimesh"), (CComponent**)&mComNaviMesh));

	if (mComVIBuffer == nullptr)
	{
		Set_TerrainMode(TERRAINSIZE_32);
	}

	if (mFiter1 == nullptr)
	{
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_FITER1), TEXT("Com_Fiter1"), (CComponent**)&mFiter1));
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_FITER2), TEXT("Com_Fiter2"), (CComponent**)&mFiter2));
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_FITER3), TEXT("Com_Fiter3"), (CComponent**)&mFiter3));
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_FITER4), TEXT("Com_Fiter4"), (CComponent**)&mFiter4));
		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_FITER_XYZW), TEXT("Com_XYZW"), (CComponent**)&mFiter_XYZW));

		FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TAGCOM(COMPONENT_TEXTURE_BRUSH), TEXT("Com_Brush"), (CComponent**)&mBrush));
		
	}

	return S_OK;
}

HRESULT CGameObject_MyTerrain::Set_ConstantTable_Tex()
{
	// Fiter12 / Brush / Range
	FAILED_CHECK(mComShader->Set_RawValue(STR_TEXTURESIZE, &mTerrainDESC.mTextureMultiSize, sizeof(_float)));

	FAILED_CHECK(mComTexture->SetUp_OnShader(mComShader, "g_SourDiffuseTexture", 0));
	FAILED_CHECK(mFiter1->SetUp_OnShader(mComShader, "g_DestDiffuseTexture", 0));

	FAILED_CHECK(mBrush->SetUp_OnShader(mComShader, "g_BrushTexture", 0));
	FAILED_CHECK(mFiter_XYZW->SetUp_OnShader(mComShader, "g_FilterTexture", 0));


	_float3 worldPickPos = GetSingle(CGameManager)->Get_PickPos();
	FAILED_CHECK(mComShader->Set_RawValue("g_vBrushPos", &worldPickPos, sizeof(_float3)));
	FAILED_CHECK(mComShader->Set_RawValue("g_fRadius", &mRadius, sizeof(_float)));

	return S_OK;
}

HRESULT CGameObject_MyTerrain::Set_HeightNewMap()
{
	// ���� ����

	// �ؽ�ó ���� ����� �ޱ�

	// ���� ����
	mComVIBuffer->Set_HeightMap(STR_PATH_HEIGHTMAP_64);


	return S_OK;
}

HRESULT CGameObject_MyTerrain::Init_Map(const _tchar* layertag)
{
	if (mTerrainDESC.mObjectSize <= 0)
		return S_FALSE;
	Set_TerrainMode(mTerrainDESC.meTerrainSize);



	// static2 ������ ��ü�� ����.
	// ������ DESC�� �а� �ش� ���̾ �����Ѵ�.
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

	// ���� ������Ʈ �� ����
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
	
	Safe_Release(mFiter1);
	Safe_Release(mFiter2);
	Safe_Release(mFiter3);
	Safe_Release(mFiter4);
	Safe_Release(mFiter_XYZW);
	Safe_Release(mBrush);
	
}