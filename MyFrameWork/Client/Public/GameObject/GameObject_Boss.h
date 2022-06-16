#pragma once

#include "GameObject_3D_Dynamic.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

// ���� AI ����
class CGameObject_BOSS final :
	public CGameObject_3D_Dynamic
{
public:

protected:
	explicit CGameObject_BOSS(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject_BOSS(const CGameObject_BOSS& rhs);
	virtual ~CGameObject_BOSS() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	virtual HRESULT Tick_World(_double TimeDelta);
	virtual HRESULT LateTick_World(_double TimeDelta);


	virtual HRESULT Init_Unit();
	virtual HRESULT Init_AI()override;
	virtual HRESULT Init_AI_Boss();


//	void Select_Idle();
	void Select_Move();
	void Select_Warrior(CAnimatior::E_COMMON_ANINAME type = CAnimatior::E_COMMON_ANINAME::E_COMMON_ANINAME_SKILL,int index=-1); // Heal ���
	void Select_Warrior_();

	void Select_Pattern1();
	void Select_Pattern2();
	void Select_Pattern3();

	void Select_Pattern();


protected:
	_float3 mStartPostition = _float3();

public:
	static CGameObject_BOSS* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_BOSS* Clone(void* pArg);
	virtual void Free() override;
};

END