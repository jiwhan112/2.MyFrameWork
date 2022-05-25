#pragma once

#include "GameObject_Base.h"

BEGIN(Engine)
class CVIBuffer;
class CVIBuffer_Terrain;
class CNavigation;
END

BEGIN(Client)

// ����
class CGameObject_MyTerrain  :
	public CGameObject_Base
{
public:
	enum E_SOURCETYPE
	{
		SOURCE_A,
		SOURCE_R,
		SOURCE_G,
		SOURCE_B,
		SOURCE_END,
	};
	
	// ����� / ������ ������
	enum E_MAPTYPE
	{
		MAPTYPE_DUNGEON,
		MAPTYPE_WORLD,
		MAPTYPE_END,
	};

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


	void	Set_ColorFiter(E_SOURCETYPE type, _color* color, _float val);


public: // DESC
	HRESULT Init_Map(const _tchar* layertag); // �� �����ͷ� �ʿ� ���� ��ü����
	HRESULT SaveDESC_Objects(const list<_uint>& uintList, const list<MODEL_WORLD_DESC>& worldObjList); // �� �����ͷ� �ʿ� ���� ��ü����

public:
	HRESULT SaveCurrentFiterMap(); // �ؽ�ó ����
	HRESULT UpdateFiterTextue(); // �ؽ�ó ���� / �� ����
	HRESULT UpdateFiterTextue_TOOL(E_SOURCETYPE type, _float3 worldPos,_float Range,_float AddValue); 
	HRESULT Set_HeightNewMap();

protected:
	virtual HRESULT Set_Component()override;
	virtual HRESULT Set_ConstantTable_Tex(); // �ؽ�ó
	HRESULT LoadTextureMap(); // �ؽ�ó �θ���
	

protected:
	CVIBuffer_Terrain*		mComVIBuffer = nullptr;
	CNavigation*			mComNaviMesh = nullptr;

	// Textures
	CTexture_map*			mComDefaultTex = nullptr;;
	CTexture_map*			mComFiterSourceTex[SOURCE_END] = { nullptr ,nullptr ,nullptr ,nullptr };
										
	CTexture_map*			mComFiter_XYZW = nullptr;

	ID3D11Texture2D*			mFiterTexture = nullptr;
	ID3D11ShaderResourceView*	mFiterSRV = nullptr;; // ���̴��� �ѱ� ���ҽ��� 

	CTexture_map*				mComBrushTex = nullptr;

	TERRAIN_DESC			mTerrainDESC;
	TEXTURE_NAMES_DESC		mTextureNameDesc;

public:
	_float					mRadius = 3;
//	_float3					mWorldPickPos;

public:
	static CGameObject_MyTerrain* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_MyTerrain* Clone(void* pArg);
	virtual void Free() override;
};


// 
END