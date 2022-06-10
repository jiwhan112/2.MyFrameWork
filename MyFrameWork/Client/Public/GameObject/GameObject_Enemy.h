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


	// 모델 구현 
	E_ENEMYTYPE			meEnemyType = ENEMY_WARRIOR;
protected:

public:
	static CGameObject_Enemy* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_Enemy* Clone(void* pArg);
	virtual void Free() override;
};

END