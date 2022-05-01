#pragma once

#include "GameObject_Base.h"

BEGIN(Engine)
class CVIBuffer;
class CVIBuffer_Terrain;
END

BEGIN(Client)

// 지형 
// 
class CGameObject_MyTerrain final :
	public CGameObject_Base
{
public:
	enum E_TILE_MODE
	{
		TILEMODE_NONE,
		TILEMODE_X,
		TILEMODE_XX,
		TILEMODE_XXX,
		TILEMODE_END
	};

	typedef struct tag_MyTile
	{
		explicit tag_MyTile()
		{
			mTileIndex = -1;
			mTileMode = TILEMODE_END;
		}
		explicit tag_MyTile(int index, E_TILE_MODE e)
		{
			mTileIndex = index;
			mTileMode = e;
		}

		int mTileIndex;
		E_TILE_MODE mTileMode;

	}MYTILE;

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

	const TERRAIN_DESC& Get_TerrainDESC() const { return mTerrainDESC; }
	HRESULT		Set_LoadTerrainDESC(const TERRAIN_DESC& desc);
	HRESULT		Set_TerrainMode(E_TERRAINSIZE e);
	
	const _float3& Get_PickWorldPos()const
	{
		return mPickWorldPos;
	}

	bool Get_isPick() const { return misPick; };

protected:
	virtual HRESULT Set_Component()override;
	virtual HRESULT Set_ConstantTable_Tex(); // 텍스처

private:
	void Update_PickPos(_float3 pickPos);
	void Update_TileVec(int x,int z);

protected:
	CVIBuffer_Terrain*		mComVIBuffer = nullptr;
	CTexture*				mComTexture = nullptr;

	TERRAIN_DESC			mTerrainDESC;

	bool misPick = false;	
	_float3 mPickWorldPos = _float3(0,0,0);


	// 타일 벡터를 저장해서 여기서 길찾기 수행
	vector<MYTILE*>*			mVecTile = nullptr;


public:
	static CGameObject_MyTerrain* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_MyTerrain* Clone(void* pArg);
	virtual void Free() override;
};

END