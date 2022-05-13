#pragma once

#include "GameObject_MyTerrain.h"

BEGIN(Engine)
class CVIBuffer;
class CVIBuffer_Terrain;
class CNavigation;
END

BEGIN(Client)

// 던전 지형
class CGameObject_MyTerrain_Daungon final :
	public CGameObject_MyTerrain
{

protected:
	explicit CGameObject_MyTerrain_Daungon(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject_MyTerrain_Daungon(const CGameObject_MyTerrain_Daungon& rhs);
	virtual ~CGameObject_MyTerrain_Daungon() = default;

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

	const _float3& Get_PickWorldPos()const
	{
		return mPickWorldPos;
	}

	bool		Get_isPick() const { return misPick; };
	int			Get_TileIndex(_float3 worldPos);
	_float3		Get_TileWorld(_uint index);
	vector<MYTILE*>* Get_VecTile() { return mVecTile; }

public: // DESC
	HRESULT Init_Map(const _tchar* layertag); // 맵 데이터로 맵에 따른 객체생성
	HRESULT SaveDESC_Objects(const list<MODEL_WORLD_DESC>& worldObjList); // 맵 데이터로 맵에 따른 객체생성



protected:
	virtual HRESULT Set_Component()override;
	virtual HRESULT Set_ConstantTable_Tex(); // 텍스처

protected:
	// 상하좌우 검사 해서 큐브 타일 배치
	HRESULT Update_TerrainTile();



private:
	void Update_PickPos(_float3 pickPos);
	void Update_TileVec(int x, int z);

protected:
	CVIBuffer_Terrain*		mComVIBuffer = nullptr;
	CTexture*				mComTexture = nullptr;
	CNavigation*			mComNaviMesh = nullptr;

	TERRAIN_DESC			mTerrainDESC;

	bool misPick = false;
	_float3 mPickWorldPos = _float3(0, 0, 0);

	// 타일 벡터를 저장해서 여기서 길찾기 수행
	vector<MYTILE*>*			mVecTile = nullptr;

public:
	static CGameObject_MyTerrain_Daungon* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_MyTerrain_Daungon* Clone(void* pArg);
	virtual void Free() override;
};

END