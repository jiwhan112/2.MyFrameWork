#pragma once

#include "GameObject_3D_Dynamic.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

// 마인 AI 설정
class CGameObject_Goblin final :
	public CGameObject_3D_Dynamic
{
public:

protected:
	explicit CGameObject_Goblin(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject_Goblin(const CGameObject_Goblin& rhs);
	virtual ~CGameObject_Goblin() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	//virtual _int Tick(_double TimeDelta);
	//virtual _int LateTick(_double TimeDelta);
	//virtual HRESULT Render();

	virtual HRESULT Init_Unit();
	virtual HRESULT Init_AI();

	// 모델 구현 



protected:

public:
	static CGameObject_Goblin* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_Goblin* Clone(void* pArg);
	virtual void Free() override;
};

END