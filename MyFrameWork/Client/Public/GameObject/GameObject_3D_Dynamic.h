#pragma once

#include "GameObject_Base.h"
#include "Animatior.h"

BEGIN(Engine)
class CModel;
class CCell;
END

// �ִϸ��̼� 3D ������Ʈ ���
BEGIN(Client)

class CGameObject_3D_Dynamic  :
	public CGameObject_Base
{
public:
	enum E_UNITTYPE
	{
		UNIT_PLAYER,
		UNIT_ENEMY,
		UNIT_BOSS,
		UNIT_END,
	};

	enum E_MAPTYPE
	{
		MAPTYPE_DUNGEON,
		MAPTYPE_WORLD,
		MAPTYPE_END,
	};

	enum E_TICK_TYPE
	{
		TICK_TYPE_NONE,
		TICK_TYPE_DUNGION_PICK, // ��ü�� Ŭ���ϸ� Pick���� ��ȯ
		TICK_TYPE_WORLD_PICK, 
		TICK_TYPE_END,
	};

protected:
	explicit CGameObject_3D_Dynamic(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject_3D_Dynamic(const CGameObject_3D_Dynamic& rhs);
	virtual ~CGameObject_3D_Dynamic() = default;

public: // Base
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();
	
	virtual HRESULT Init_Unit();
	virtual HRESULT Init_AI();
	virtual HRESULT Init_Create();


public: // GetSet
	HRESULT						Set_MapSetting(E_MAPTYPE type);

	CModel*						Get_ComModel() const { return mComModel; }
	CBehaviorTree*				Get_ComBehavior() const { return mComBehavior; }
	const list<CCollider*>*		Get_ComListCollider() const { return mComListCollider; }

	CGameObject_MyTerrain*		Get_Terrain(E_MAPTYPE type) const { return mTerrain_Maps[type]; }
	CNavigation*				Get_ComNaviMesh(E_MAPTYPE type) const { return mComNavi[type]; }

	const MODEL_DYNAMIC_DESC&	Get_ModelDESC() const { return mModelDesc; }

	HRESULT		Set_LoadModelDynamicDESC(const MODEL_DYNAMIC_DESC& desc);

	HRESULT		Add_ColliderDesc(COLLIDER_DESC desc);
	HRESULT		Add_ColliderDesc(COLLIDER_DESC* desc,int size);

public: // Collision
	virtual HRESULT CollisionFunc(_float3 PickPosition, _float dist, _uint ColliderIndex);

public: // Move AI
	HRESULT FindPathForCurrentNavi(_float3 GoalPosition);
	_bool	Get_PathList_Frontpop(_float3* NextPosition);
	// �� �� �ִ� ��ġ �̱�
	_bool	FindPathRandAblePostition(_int fRange, _float3* GoalPos);

	_float3 Get_GoalPostiton()const;

	void	Set_IsTerrainHeight(_bool b) { mIsTerrainHeight = b; }
	_bool	Get_IsTerrainHeight() const { return mIsTerrainHeight; }
	_float3	Get_TerrainHeightPostition() ;

public: // Animation
// �ִϸ��̼� �̸����� ����
	HRESULT Set_AniEnum(CAnimatior::E_COMMON_ANINAME name);
	HRESULT Set_AniIndex(_uint AniIndex);
	HRESULT Set_AniString(string str);


protected:
	// AI ���� ����
	virtual HRESULT Set_Component() override;
	HRESULT Update_Collider();
	HRESULT Set_Terrain_HeightY(class CGameObject_MyTerrain* terrain);

	//Test
	//_float2 WorldToView(_float3 pos);

protected: // 3D�� Com / DESC �߰�
	CModel*						mComModel = nullptr;
	// AI
	CBehaviorTree*				mComBehavior = nullptr;

	// �ݶ��̴� ����Ʈ �߰� / �ݶ��̴��� �ʿ� ���� �ʰ� ���⼭�� �����Ѵ�.
	// ���� �޸��� ����
	list<CCollider*>*			mComListCollider = nullptr;


	MODEL_DYNAMIC_DESC			mModelDesc;
	list<COLLIDER_DESC>			mListColliderDesc;
	

	E_UNITTYPE					meUnitType = UNIT_END;

	// MOVE
	list<CCell*>				mCurrentPathList;
	_float3						mCurrentPosition;

	// ����
	E_MAPTYPE					meCurrentMap = MAPTYPE_END;
	
	// ���� 

	// ���۵� 
	CGameObject_MyTerrain*		mTerrain_Maps[MAPTYPE_END] = {nullptr,nullptr };
	CNavigation*				mComNavi[MAPTYPE_END] = { nullptr, nullptr };

	CGameObject_MyTerrain*		mCurrentMap = nullptr;
	CNavigation*				mCurrentNavi = nullptr;

	_bool						mIsTerrainHeight = false;

	// TICKTYPE
	E_TICK_TYPE					meTickType = TICK_TYPE_END;

	const _float mMouseOffset = 4;

public:
	static CGameObject_3D_Dynamic* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_3D_Dynamic* Clone(void* pArg);
	virtual void Free() override;
};

END