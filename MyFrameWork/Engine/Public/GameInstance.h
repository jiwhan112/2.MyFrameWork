#pragma once

#include "Graphic_Device.h"
#include "Input_Device.h"
#include "Timer_Manager.h"
#include "Component_Manager.h"
#include "Level_Manager.h"
#include "Object_Manager.h"

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	HRESULT Initialize_Engine(HINSTANCE hInstance, _uint iNumLevels, const CGraphic_Device::GRAPHICDESC& GraphicDesc, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppDeviceContextOut);
	_int Tick_Engine(_double TimeDelta);
	HRESULT Clear_LevelResource(_uint iLevelIndex);

public: /* For.Graphic_Device */
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT Clear_DepthStencil_View();
	HRESULT Present();

public: /* For.Input_Device */
	_byte Get_DIKeyState(_ubyte eKeyID);
	_long Get_DIMouseMoveState(CInput_Device::MOUSEMOVESTATE eMouseMoveState);
	_byte Get_DIMouseButtonState(CInput_Device::MOUSEBUTTONSTATE eMouseButtonState);

public: /* For.Timer_Manager */
	_double	Get_TimeDelta(const _tchar* pTimerTag);
	HRESULT Add_Timer(const _tchar* pTimerTag);

public: /* For.Level_Manager */
	HRESULT OpenLevel(_uint iLevelIndex, class CLevel* pNextLevel);
	HRESULT Render_Level();

public: /* For.Component_Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, class CComponent* pPrototype);
	CComponent* Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg = nullptr);

public: /* For.Object_Manager */
	class CComponent* Get_Component(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex = 0);
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pPrototypeTag, void* pArg = nullptr);

private:
	CGraphic_Device*			m_pGraphic_Device = nullptr;
	CTimer_Manager*				m_pTimer_Manager = nullptr;
	CLevel_Manager*				m_pLevel_Manager = nullptr;
	CObject_Manager*			m_pObject_Manager = nullptr;
	CComponent_Manager*			m_pComponent_Manager = nullptr;
	CInput_Device*				m_pInput_Device = nullptr;

public:
	static void Release_Engine();
	virtual void Free() override;
};

END