#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
class CRenderer;
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

public:
	HRESULT Open_Level(E_LEVEL eLevelIndex);

private:
	CGameInstance*			m_pGameInstance = nullptr;
	CRenderer*				m_pRenderer = nullptr;
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;

#ifdef _DEBUG
private:
	_tchar				m_szFPS[MAX_PATH] = TEXT("");
	_ulong				m_dwNumRender = 0;
	_double				m_dTimerAcc = 0.0f;
#endif // _DEBUG
	HRESULT Ready_Initialize();
	HRESULT Ready_Prototype_Components();
	HRESULT Ready_Prototype_GameObject();


	HRESULT Ready_Prototype_Components_Model();
	HRESULT Ready_Prototype_Components_Texture();
	HRESULT Ready_Prototype_Components_Shader();

	HRESULT Ready_Prototype_GameObject_Emptyobject();


public:
	static CMainApp* Create();
	virtual void Free() override;
};

END