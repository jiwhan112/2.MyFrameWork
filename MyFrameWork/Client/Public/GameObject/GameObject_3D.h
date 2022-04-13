#pragma once

#include "GameObject_Base.h"

BEGIN(Client)

// 3D 오브젝트용 부모클래스
class CGameObject_3D final:
	public CGameObject_Base
{
protected:
	explicit CGameObject_3D(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject_3D(const CGameObject_3D& rhs);
	virtual ~CGameObject_3D() = default;


public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

public:

protected:
	virtual HRESULT Set_Component()override;

private:
	HRESULT Set_ConstantTable_World();
	HRESULT Set_ConstantTable_Tex(_uint texid=0);


public:
	static CGameObject_3D* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_3D* Clone(void* pArg);
	virtual void Free() override;
};

END