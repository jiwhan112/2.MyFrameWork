#pragma once

#include "Camera.h"

BEGIN(Client)
class CCamera_Game final :
	public CCamera
{
protected:
	explicit CCamera_Game(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CCamera_Game(const CCamera_Game& rhs);
	virtual ~CCamera_Game() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

public:
	static CCamera_Game* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CCamera_Game* Clone(void* pArg);
	virtual void Free() override;
};

END