#pragma once

#include "Level.h"

BEGIN(Client)
class CLevel_MyGamePlay final :
	public CLevel
{
private:
	explicit CLevel_MyGamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CLevel_MyGamePlay() = default;

public:
	virtual HRESULT NativeConstruct();
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_IMGUI();

	HRESULT Ready_Light();
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Mouse(const _tchar* pLayerTag);
	
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Level_BaseGame();
	HRESULT Ready_Layer_Effect(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI(const _tchar* pLayerTag);
	HRESULT Ready_Prototype_GameObject();

public:
	static CLevel_MyGamePlay* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
};

END