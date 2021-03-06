#pragma once

#include "GameObject_Base.h"
#include "GameObject/GameObject_Environment.h"

BEGIN(Engine)
class CVIBuffer;
class CVIBuffer_Terrain;
class CNavigation;

END

BEGIN(Client)

// 지형
class CGameObject_MyTerrain  :
	public CGameObject_Base
{
public:
	enum E_SOURCETYPE
	{
		SOURCE_NONE,
		SOURCE_A,
		SOURCE_R,
		SOURCE_G,
		SOURCE_B,
		SOURCE_END,
	};
	
	// 월드맵 / 던전맵 나누기
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
	HRESULT	Init_SetupInit(bool istool = false); // 정보가 설정되고 초기화하는 함수

	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

public:
	CVIBuffer_Terrain*		Get_TerrainBuffer() const { return mComVIBuffer; }
	CNavigation*			Get_ComNavimesh() const { return mComNaviMesh; }

	const TERRAIN_DESC& Get_TerrainDESC() const { return mTerrainDESC; }
	HRESULT		Set_LoadTerrainDESC(const TERRAIN_DESC& desc);
//	HRESULT		Set_TerrainMode(E_TERRAINSIZE e);
	HRESULT		Set_TerrainMode(E_MAPTYPE type);

	int			Get_TileIndex(_float3 worldPos);
	_float3		Get_TileWorld(_uint index);
	_uint		Get_MapSize();


	void		Set_ColorFiter(E_SOURCETYPE type, _color* color, _float val);
	void		Set_MapType(E_MAPTYPE type) { meMapType = type; }
	E_MAPTYPE	Get_MapType()const { return meMapType; }


public: // DESC
	HRESULT Init_Map(); // 맵 데이터로 맵에 따른 객체생성
	HRESULT Get_MODEL_WORLD_DESC_List(list<MODEL_WORLD_DESC>* SaverList = nullptr);
	HRESULT SaveDESC_Objects(const list<_uint>& uintList, const list<MODEL_WORLD_DESC>& worldObjList); // 맵 데이터로 맵에 따른 객체생성

public:
	HRESULT SaveCurrentFiterMap(); // 텍스처 저장
	HRESULT UpdateFiterTextue(); // 텍스처 생성 / 값 변경
	HRESULT UpdateFiterTextue_TOOL(E_SOURCETYPE type, _float3 worldPos,_float Range, _float value);
	HRESULT Set_HeightNewMap();
	_float Get_HeightY(_float3 PositionXZ);

	void Add_MapObject(CGameObject_Base* obj)
	{
		if (mListBackObjects != nullptr)
			mListBackObjects->push_front(obj);
	}
	const list<CGameObject_Base*>* Get_MapObjects()
	{
		return mListBackObjects;
	}
protected:
	virtual HRESULT Set_Component()override;
	virtual HRESULT Set_ConstantTable_Tex(); // 텍스처
	HRESULT LoadTextureMap(); // 텍스처 부르기
	
	// 환경 오브젝트 만들기
	CGameObject_Environment::E_ENVIORMENT_TYPE Get_Str2EnviorType(string str);


protected:
	CVIBuffer_Terrain*			mComVIBuffer = nullptr;
	CNavigation*				mComNaviMesh = nullptr;

	// Textures
	CTexture_map*				mComDefaultTex = nullptr;;
	CTexture_map*				mComFiterSourceTex[SOURCE_END] = { nullptr ,nullptr ,nullptr ,nullptr };
										
	CTexture_map*				mComFiter_XYZW = nullptr;

	ID3D11Texture2D*			mFiterTexture = nullptr;
	ID3D11ShaderResourceView*	mFiterSRV = nullptr;; // 셰이더에 넘길 리소스뷰
	_ulong*						mPixels = nullptr;

	CTexture_map*				mComBrushTex = nullptr;

	TERRAIN_DESC				mTerrainDESC;
	TEXTURE_NAMES_DESC			mTextureNameDesc;
	E_MAPTYPE					meMapType = MAPTYPE_END;
	string						mStrMapDatName = "";

	// 생성된 배경 오브젝트 저장
	list<CGameObject_Base*>*		mListBackObjects = nullptr;

public:
	_float					mRadius = 0.5f;
//	_float3					mWorldPickPos;

public:
	static CGameObject_MyTerrain* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_MyTerrain* Clone(void* pArg);
	virtual void Free() override;
};

// 
END