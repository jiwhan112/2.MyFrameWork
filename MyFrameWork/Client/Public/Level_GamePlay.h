#pragma once

#include "Level.h"

BEGIN(Client)
class CLevel_GamePlay final:
	public CLevel
{
private:
	explicit CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CLevel_GamePlay() = default;

public:
	virtual HRESULT NativeConstruct();
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();


public:
	static CLevel_GamePlay* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
};

END