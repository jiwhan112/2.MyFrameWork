#pragma once

#include "Graphic_Device.h"
#include "Input_Device.h"
#include "Timer_Manager.h"
#include "Component_Manager.h"
#include "Level_Manager.h"
#include "Object_Manager.h"
#include "PipeLine.h"
#include "LightMgr.h"
#include "FileInfo.h"
#include "Picking.h"
#include "Frstum.h"
#include "FontMgr.h"
#include "EasingMgr.h"
#include "RenderTargetMgr.h"
#include "SoundMgr.h"
#include "ThreadMgr.h"


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
	_int LateTick_Engine(_double TimeDelta);

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
	_uint	Get_CurrentLevelIndex() const;

public: /* For.Component_Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, class CComponent* pPrototype);
	CComponent* Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg = nullptr);

public: /* For.Object_Manager */
	class CComponent* Get_Component(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex = 0);
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	CGameObject* Add_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pPrototypeTag, void* pArg = nullptr);
	CGameObject* Create_GameObject(const _tchar* pPrototypeTag, void* pArg = nullptr);
	HRESULT		 Push_Object(_uint levelindex, const _tchar* pLayerTag, CGameObject* obj);
	const list<CGameObject*>* Get_GameObjectLayerList(_uint iLevelIndex, const _tchar* pLayerTag);
	const map<wstring, class CLayer*>* Get_All_GameObjectLayerMap(_uint iLevelIndex);

public: /* For. PipeLine Single*/
	HRESULT SetTransform(CPipeLine::E_TRANSFORMSTATETYPE eStateType, _fmatrix TransformMatrix);
	_float4x4 GetTransformFloat4x4(CPipeLine::E_TRANSFORMSTATETYPE eStateType);
	_float4x4 GetTransformFloat4x4_TP(CPipeLine::E_TRANSFORMSTATETYPE eStateType);
	_float4 GetCameraPosition_float() const;
	_vector GetCameraPosition_vec() const;

public: // For. Light
	const LIGHTDESC* Get_LightDesc(_uint iIndex) const;
	HRESULT Add_Light(ID3D11Device* device, ID3D11DeviceContext* context, const LIGHTDESC& desc);

public: // For. File
	HRESULT FolderFinder(const wstring& FileFolder);
	void SaveVectorToDat(const wstring& savetxtName, wstring exename = L"");
	list<MYFILEPATH*> Load_ExtensionList(const wstring& txtfilepath, string exe,bool bFlag);
	wstring Get_PathData(wstring Fullpath);

public: // For. Picking
	HRESULT Transform_ToLocalSpace(_fmatrix WorldMatrixinverse);
	_bool isPick(_float3 * pLocalPoint, _float3 * pOut);
	_float3 Get_PickPos() const;
	_bool	 Get_isPick() const;
	_ray	 Get_Ray_World() const;
	_float2		Get_WindowPos()const;

public: // For. Frestum
	_bool IsIn_WorldSpace(_fvector vPoint, _float fRange = 0);
	_bool IsIn_LocalSpace(_fvector vPoint, _float fRange = 0);

public: // For, Font
	HRESULT Add_Font(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pFontTag, const _tchar* pFontFilePath);
	HRESULT Render_Font(const _tchar* pFontTag, const _tchar* pText, _float2 vPosition, _fvector vColor);

public: // FOR.EASING
	_float	Easing(EasingTypeID eEasingType, _float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime);
	_float3	Easing3(EasingTypeID eEasingType, _float3 fStartPoint, _float3 fTargetPoint, _float fPassedTime, _float fTotalTime);

public: // For. RenderTarget
	// ·£´õ Å¸°Ù 
	ID3D11ShaderResourceView* Get_RenderTargetSRV(const _tchar* pTargetTag);

	//SoundMgr
public:
	_int  Channel_VolumeUp(CHANNELID eID, _float _vol);
	_int  Channel_VolumeDown(CHANNELID eID, _float _vol);
	_int  Channel_Pause(CHANNELID eID);

	HRESULT PlaySound(TCHAR* pSoundKey, CHANNELID eID, _float fLouderMultiple = 1.f);
	HRESULT PlayBGM(TCHAR* pSoundKey, _float fLouderMultiple = 1.f);
	void Stop_ChannelSound(CHANNELID eID);
	void Stop_AllChannel();

	_float  Get_Channel_Volume(CHANNELID eID);
	_bool  Get_Channel_IsPaused(CHANNELID eID);

public: /*For TreadMgr*/
	HRESULT		PlayThread(void* _ThreadFunc, void* _pArg);

public: // FOR.DEBUG
	bool Get_IsColliderRender() const { return mIsRender_Collider; }
	bool Get_IsColliderRender_Navi() const { return mIsRender_Collider_Navi; }

private: // FOR.DEBUG
	void	Update_Debug();

	bool	mIsRender_Collider = true;
	bool	mIsRender_Collider_Navi = true;


private:
	CGraphic_Device*			m_pGraphic_Device = nullptr;
	CTimer_Manager*				m_pTimer_Manager = nullptr;
	CLevel_Manager*				m_pLevel_Manager = nullptr;
	CObject_Manager*			m_pObject_Manager = nullptr;
	CComponent_Manager*			m_pComponent_Manager = nullptr;
	CInput_Device*				m_pInput_Device = nullptr;
	CPipeLine*					m_pPipeLine = nullptr;
	CLightMgr*					m_pLightMgr = nullptr;
	CFileInfo*					m_pFileMgr = nullptr;
	CPicking*					m_pPickMgr = nullptr;
	CFrustum*					m_pFrstumMgr = nullptr;
	CFontMgr*					m_pFontMgr = nullptr;
	CEasingMgr*					m_pEasingMgr = nullptr;
	CRenderTargetMgr*			m_pRenderTargetMgr = nullptr;
	CSoundMgr*					m_pSoundMgr = nullptr;
	CThreadMgr*					m_pThreadMgr = nullptr;

public:
	static void Release_Engine();
	virtual void Free() override;
};

END