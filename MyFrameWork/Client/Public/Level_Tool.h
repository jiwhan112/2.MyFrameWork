#pragma once

#include "Level.h"

BEGIN(Client)
class CLevel_Tool final :
	public CLevel
{
private:
	explicit CLevel_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CLevel_Tool() = default;

public:
	virtual HRESULT NativeConstruct();
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

public:
	HRESULT Ready_Layer_Camera(const _tchar * pLayerTag);
	HRESULT	Ready_Layer_BackGround(const _tchar * pLayerTag);
	HRESULT ReadyTools();

private:
	class CImgui_MyDemo* mDemo;
	class CImgui_UI* mUI;

public:
	static CLevel_Tool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
};

END