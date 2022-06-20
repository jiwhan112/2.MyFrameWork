#pragma once

#include "Level.h"

BEGIN(Client)
class CLevel_Logo final :
	public CLevel
{
private:
	explicit CLevel_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CLevel_Logo() = default;

public:
	virtual HRESULT NativeConstruct();
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Prototype_GameObject();
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI_LOGO(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI_GAMEPLAY(const _tchar* pLayerTag);

public:
	static CLevel_Logo* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
};

END