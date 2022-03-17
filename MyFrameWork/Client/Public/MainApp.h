#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;
public:
	HRESULT NativeConstruct();
	_int Tick(_double TimeDelta);
	HRESULT Render();

private:
	CGameInstance*			m_pGameInstance = nullptr;

	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;

public:
	static CMainApp* Create();
	virtual void Free() override;
};

END