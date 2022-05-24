#pragma once

#include "GameObject_Base.h"

BEGIN(Engine)
class CVIBuffer;
class CVIBuffer_Terrain;
class CNavigation;
END

BEGIN(Client)

// 지형
//
class CGameObject_MyTerrain  :
	public CGameObject_Base
{
public:


protected:
	explicit CGameObject_MyTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject_MyTerrain(const CGameObject_MyTerrain& rhs);
	virtual ~CGameObject_MyTerrain() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

public:
	CVIBuffer_Terrain*		Get_TerrainBuffer() const { return mComVIBuffer; }
	CNavigation*			Get_ComNavimesh() const { return mComNaviMesh; }

	const TERRAIN_DESC& Get_TerrainDESC() const { return mTerrainDESC; }
	HRESULT		Set_LoadTerrainDESC(const TERRAIN_DESC& desc);
	HRESULT		Set_TerrainMode(E_TERRAINSIZE e);


	int			Get_TileIndex(_float3 worldPos);
	_float3		Get_TileWorld(_uint index);
	_uint		GetMapSize();

public: // DESC
	HRESULT Init_Map(const _tchar* layertag); // 맵 데이터로 맵에 따른 객체생성
	HRESULT SaveDESC_Objects(const list<_uint>& uintList, const list<MODEL_WORLD_DESC>& worldObjList); // 맵 데이터로 맵에 따른 객체생성

public:
	HRESULT CreateFiterTexture();
	HRESULT Set_HeightNewMap();

protected:
	virtual HRESULT Set_Component()override;
	virtual HRESULT Set_ConstantTable_Tex(); // 텍스처

protected:
	CVIBuffer_Terrain*		mComVIBuffer = nullptr;
	CTexture*				mComTexture = nullptr;
	CNavigation*			mComNaviMesh = nullptr;

	TERRAIN_DESC			mTerrainDESC;

	// Texture Test
	
	// 필터링용 소스 텍스처 4개
	CTexture*				mFiter1 = nullptr;
	CTexture*				mFiter2 = nullptr;
	CTexture*				mFiter3 = nullptr;
	CTexture*				mFiter4 = nullptr;
//	CTexture*				mHeight = nullptr;

	// 소스텍스처를 XYZW로 섞어주는 텍스처
	CTexture*				mFiter_XYZW = nullptr;
	CTexture*				mBrush = nullptr;

	// 필터 텍스처 이름
	TEXTURE_NAMES_DESC		mTextureFiterDESC;

public:
	_float					mRadius = 3;
//	_float3					mWorldPickPos;

public:
	static CGameObject_MyTerrain* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_MyTerrain* Clone(void* pArg);
	virtual void Free() override;
};

END