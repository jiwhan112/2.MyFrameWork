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
	HRESULT Ready_Initialize();
	HRESULT Ready_Prototype_Components();
	HRESULT Ready_Prototype_GameObject();

	HRESULT Ready_Prototype_Components_Model();
	HRESULT Ready_Prototype_Components_AniModel();
	HRESULT Ready_Prototype_Components_Texture();
	HRESULT Ready_Prototype_Components_Shader();

	HRESULT Ready_Prototype_GameObject_Emptyobject();

private:
	CGameInstance*			m_pGameInstance = nullptr;
	CGameManager*			m_pGameManager = nullptr;
	CRenderer*				m_pRenderer = nullptr;
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;

public:
	static CMainApp* Create();
	virtual void Free() override;
};

END