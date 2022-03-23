#pragma once

#include "GameObject.h"

BEGIN(Client)
class CGameObject_Default final:
	public CGameObject
{
protected:
	explicit CGameObject_Default(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject_Default(const CGameObject_Default& rhs);
	virtual ~CGameObject_Default() = default;


public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

public:
	static CGameObject_Default* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_Default* Clone(void* pArg);
	virtual void Free() override;
};

END