#pragma once

#include "GameObject_3D_Dynamic.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

// 마인 AI 설정
class CGameObject_Orc final :
	public CGameObject_3D_Dynamic
{
public:

protected:
	explicit CGameObject_Orc(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject_Orc(const CGameObject_Orc& rhs);
	virtual ~CGameObject_Orc() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	//virtual _int Tick(_double TimeDelta);
	//virtual _int LateTick(_double TimeDelta);
	//virtual HRESULT Render();

	virtual HRESULT Init_Unit();
	virtual HRESULT Init_AI();
	HRESULT Init_AI_Default();

//	virtual HRESULT AttackFunc() override;



	// AI
public:
	HRESULT Select_WorldAttack(CGameObject_3D_Dynamic* target);

protected:

public:
	static CGameObject_Orc* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_Orc* Clone(void* pArg);
	virtual void Free() override;
};

END