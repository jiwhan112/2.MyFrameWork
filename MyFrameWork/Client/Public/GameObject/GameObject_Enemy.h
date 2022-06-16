#pragma once

#include "GameObject_3D_Dynamic.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

// 마인 AI 설정
class CGameObject_Enemy final :
	public CGameObject_3D_Dynamic
{
public:

	enum E_ENEMYTYPE
	{
		ENEMY_WARRIOR,
		//ENEMY_BB,
		//ENEMY_CC,
		ENEMY_END,
	};
	
	enum E_ENEMY_MOVETARGET
	{
		ENEMY_MOVETARGET_WORLD1,
		ENEMY_MOVETARGET_WORLD2,
		ENEMY_MOVETARGET_WORLD3,
		ENEMY_MOVETARGET_DUNGEON,
		ENEMY_MOVETARGET_END,
	};


protected:
	explicit CGameObject_Enemy(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject_Enemy(const CGameObject_Enemy& rhs);
	virtual ~CGameObject_Enemy() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	//virtual _int Tick(_double TimeDelta);
	//virtual _int LateTick(_double TimeDelta);
	//virtual HRESULT Render();

	virtual HRESULT Tick_Dungeon(_double TimeDelta);
	virtual HRESULT LateTick_Dungeon(_double TimeDelta);

	virtual HRESULT Tick_World(_double TimeDelta);
	virtual HRESULT LateTick_World(_double TimeDelta);


	virtual HRESULT Init_Unit();
	virtual HRESULT Init_AI()override;
	virtual HRESULT Init_AI_Enemy();



	// 월드에서 경로 바꾸기
	HRESULT Set_MoveCount();
	// 던전으로 이동 명령
	HRESULT Set_GoDungeon();



	// CollisionFunc
	virtual HRESULT CollisionFunc(_float3 PickPosition, _float dist, _uint ColliderIndex);

	// 공격 함수
	// virtual HRESULT AttackFunc() override;


	// 모델 구현 
	virtual HRESULT Select_WorldAttack(CGameObject_3D_Dynamic* target)override;


protected:
	_double				mWorldIdleTimer = 0;
	_bool				mIsCreateOrder = false;
	_double				mWorldMoveTimeMax = 5;

	E_ENEMYTYPE			meEnemyType = ENEMY_WARRIOR;

	_float3				mMoveTarget[E_ENEMY_MOVETARGET::ENEMY_MOVETARGET_END];
	_uint				mWorldMoveIndex = 0;


	const _float3 mWorld_EnemyDungeonPos = _float3(52.f, 8.72f, 11.f);
	const _float3 mWorldTargetPos1 = _float3(12, 8.72f, 20);
	const _float3 mWorldTargetPos2 = _float3(25, 8.72f, 15);
	const _float3 mWorldTargetPos3 = _float3(50, 8.72f, 12);

public:
	static CGameObject_Enemy* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_Enemy* Clone(void* pArg);
	virtual void Free() override;
};

END