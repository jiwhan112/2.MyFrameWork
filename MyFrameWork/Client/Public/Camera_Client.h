#pragma once

#include "Camera.h"

BEGIN(Client)
class CCamera_Client final:
	public CCamera
{
protected:
	explicit CCamera_Client(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CCamera_Client(const CCamera_Client& rhs);
	virtual ~CCamera_Client() = default;


public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

public:
	static CCamera_Client* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CCamera_Client* Clone(void* pArg);
	virtual void Free() override;
};

END