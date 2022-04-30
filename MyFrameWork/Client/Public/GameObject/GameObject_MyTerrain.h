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

	CVIBuffer_Terrain*		Get_TerrainBuffer() const { return mComVIBuffer; }

	const TERRAIN_DESC& Get_TerrainDESC() const { return mTerrainDESC; }

	HRESULT		Set_LoadModelDESC(const MODEL_STATIC_DESC& desc);


public:
	bool PickObject();

protected:
	virtual HRESULT Set_Component()override;

	virtual HRESULT Set_ConstantTable_Tex(); // 텍스처

protected:
	CVIBuffer_Terrain*		mComVIBuffer = nullptr;
	CTexture*				mComTexture = nullptr;

	TERRAIN_DESC			mTerrainDESC;


public:
	static CGameObject_MyTerrain* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_MyTerrain* Clone(void* pArg);
	virtual void Free() override;
};

END