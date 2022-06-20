#include "..\Public\GameInstance.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
	: m_pGraphic_Device(CGraphic_Device::GetInstance())
	, m_pInput_Device(CInput_Device::GetInstance())
	, m_pTimer_Manager(CTimer_Manager::GetInstance())
	, m_pComponent_Manager(CComponent_Manager::GetInstance())
	, m_pLevel_Manager(CLevel_Manager::GetInstance())
	, m_pObject_Manager(CObject_Manager::GetInstance())
	, m_pPipeLine(CPipeLine::GetInstance())
	, m_pLightMgr(CLightMgr::GetInstance())
	, m_pFileMgr(CFileInfo::GetInstance())
	, m_pPickMgr(CPicking::GetInstance())
	, m_pFrstumMgr(CFrustum::GetInstance())
	, m_pFontMgr(CFontMgr::GetInstance())
	, m_pEasingMgr(CEasingMgr::GetInstance())
	, m_pRenderTargetMgr(CRenderTargetMgr::GetInstance())
	, m_pSoundMgr(CSoundMgr::GetInstance())
	, m_pThreadMgr(CThreadMgr::GetInstance())
	
{
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pInput_Device);
	Safe_AddRef(m_pTimer_Manager);
	Safe_AddRef(m_pComponent_Manager);
	Safe_AddRef(m_pLevel_Manager);
	Safe_AddRef(m_pObject_Manager);
	Safe_AddRef(m_pPipeLine);
	Safe_AddRef(m_pLightMgr);
	Safe_AddRef(m_pFileMgr);
	Safe_AddRef(m_pPickMgr);
	Safe_AddRef(m_pFrstumMgr);
	Safe_AddRef(m_pFontMgr);
	Safe_AddRef(m_pEasingMgr);
	Safe_AddRef(m_pRenderTargetMgr);
	Safe_AddRef(m_pSoundMgr);
	Safe_AddRef(m_pThreadMgr);
	

	mIsRender_Collider = true;
	mIsRender_Collider_Navi = true;

}

HRESULT CGameInstance::Initialize_Engine(HINSTANCE hInstance, _uint iNumLevels, const CGraphic_Device::GRAPHICDESC & GraphicDesc, ID3D11Device ** ppDeviceOut, ID3D11DeviceContext ** ppDeviceContextOut)
{
	if (nullptr == m_pGraphic_Device ||
		nullptr == m_pObject_Manager || 
		nullptr == m_pFrstumMgr)
		return E_FAIL;

	FAILED_CHECK(m_pGraphic_Device->Ready_Graphic_Device(GraphicDesc.hWnd, GraphicDesc.eWinMode, GraphicDesc.iWinCX, GraphicDesc.iWinCY, ppDeviceOut, ppDeviceContextOut));
	FAILED_CHECK(m_pInput_Device->Ready_Input_Device(hInstance, GraphicDesc.hWnd, 0.3f));
	FAILED_CHECK(m_pObject_Manager->Reserve_Container(iNumLevels));
	FAILED_CHECK(m_pComponent_Manager->Reserve_Container(iNumLevels));
	FAILED_CHECK(m_pPickMgr->Initialize(*ppDeviceOut, *ppDeviceContextOut, GraphicDesc.hWnd));
	FAILED_CHECK(m_pFrstumMgr->Initialize());
	FAILED_CHECK(m_pSoundMgr->Initialize_FMOD());

//	FAILED_CHECK(m_pRenderTargetMgr->Initialize(*ppDeviceOut, *ppDeviceContextOut));
//	FAILED_CHECK(m_pLightMgr->Initialize(*ppDeviceOut, *ppDeviceContextOut));

	return S_OK;
}

_int CGameInstance::Tick_Engine(_double TimeDelta)
{
	if (nullptr == m_pLevel_Manager ||
		nullptr == m_pObject_Manager)
		return -1;

#ifdef _DEBUG
	Update_Debug();
#endif // _DEBUG

	// INPUT
	FAILED_CHECK(m_pInput_Device->SetUp_InputDeviceState(TimeDelta));

	// Pick
	FAILED_CHECK(m_pPickMgr->Transform_ToWorldSpace());

	// Tick
	FAILED_CHECK(m_pLevel_Manager->Tick(TimeDelta));
	FAILED_CHECK(m_pObject_Manager->Tick(TimeDelta));



	// PipeLine
	FAILED_CHECK(m_pPipeLine->Tick());
	// Frstum
	(m_pFrstumMgr->Tick());


	return 0;
}

_int CGameInstance::LateTick_Engine(_double TimeDelta)
{

	if (nullptr == m_pLevel_Manager ||
		nullptr == m_pObject_Manager)
		return -1;


	// LateTick
	FAILED_CHECK(m_pLevel_Manager->LateTick(TimeDelta));
	FAILED_CHECK(m_pObject_Manager->LateTick(TimeDelta));

	return 0;
}

HRESULT CGameInstance::Clear_LevelResource(_uint iLevelIndex)
{
	if (nullptr == m_pObject_Manager ||
		nullptr == m_pComponent_Manager)
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Clear_LevelObject(iLevelIndex)))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Clear_LevelObject(iLevelIndex)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGameInstance::Clear_BackBuffer_View(_float4 vClearColor)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pGraphic_Device->Clear_BackBuffer_View(vClearColor);
}

HRESULT CGameInstance::Clear_DepthStencil_View()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pGraphic_Device->Clear_DepthStencil_View();
}

HRESULT CGameInstance::Present()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pGraphic_Device->Present();
}

_byte CGameInstance::Get_DIKeyState(_ubyte eKeyID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIKeyState(eKeyID);
}

_long CGameInstance::Get_DIMouseMoveState(CInput_Device::MOUSEMOVESTATE eMouseMoveState)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMouseMoveState(eMouseMoveState);
}

_byte CGameInstance::Get_DIMouseButtonState(CInput_Device::MOUSEBUTTONSTATE eMouseButtonState)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMouseButtonState(eMouseButtonState);
}

_double CGameInstance::Get_TimeDelta(const _tchar * pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return 0.f;

	return m_pTimer_Manager->Get_TimeDelta(pTimerTag);
}

HRESULT CGameInstance::Add_Timer(const _tchar * pTimerTag)
{
	NULL_CHECK_HR(m_pTimer_Manager);

	return m_pTimer_Manager->Add_Timer(pTimerTag);
}

HRESULT CGameInstance::OpenLevel(_uint iLevelIndex, CLevel * pNextLevel)
{
	NULL_CHECK_HR(m_pLevel_Manager);
	return m_pLevel_Manager->OpenLevel(iLevelIndex, pNextLevel);
}

HRESULT CGameInstance::Render_Level()
{
	NULL_CHECK_HR(m_pLevel_Manager);
	return m_pLevel_Manager->Render();
}

_uint CGameInstance::Get_CurrentLevelIndex() const
{
	NULL_CHECK_HR(m_pLevel_Manager);
	return m_pLevel_Manager->Get_CurrentLevelIndex();
}

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const _tchar * pPrototypeTag, CComponent * pPrototype)
{
	NULL_CHECK_HR(m_pComponent_Manager);
	return m_pComponent_Manager->Add_Prototype(iLevelIndex, pPrototypeTag, pPrototype);
}

CComponent * CGameInstance::Clone_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, void * pArg)
{
	NULL_CHECK_RETURN(m_pComponent_Manager, nullptr);
	return m_pComponent_Manager->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
}

CComponent * CGameInstance::Get_Component(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pComponentTag, _uint iIndex)
{
	NULL_CHECK_RETURN(m_pObject_Manager, nullptr);
	return m_pObject_Manager->Get_Component(iLevelIndex, pLayerTag, pComponentTag, iIndex);
}

HRESULT CGameInstance::Add_Prototype(const _tchar * pPrototypeTag, CGameObject * pPrototype)
{
	NULL_CHECK_HR(m_pObject_Manager);
	return m_pObject_Manager->Add_Prototype(pPrototypeTag, pPrototype);
}

CGameObject* CGameInstance::Add_GameObject(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pPrototypeTag, void * pArg)
{
	if (m_pObject_Manager == nullptr)
		return nullptr;

	return m_pObject_Manager->Add_GameObject(iLevelIndex, pLayerTag, pPrototypeTag, pArg);
}

CGameObject * CGameInstance::Create_GameObject(const _tchar * pPrototypeTag, void * pArg)
{
	if (m_pObject_Manager == nullptr)
		return nullptr;

	return m_pObject_Manager->Create_GameObject(pPrototypeTag, pArg);
}

HRESULT CGameInstance::Push_Object(_uint levelindex, const _tchar * pLayerTag, CGameObject * obj)
{
	if (m_pObject_Manager == nullptr)
		return E_FAIL;

	return m_pObject_Manager->Push_GameObject(levelindex, pLayerTag, obj);
}

const list<CGameObject*>* CGameInstance::Get_GameObjectLayerList(_uint iLevelIndex, const _tchar * pLayerTag)
{
	if (m_pObject_Manager == nullptr)
		return nullptr;

	return m_pObject_Manager->Get_GameObjectLayerList(iLevelIndex, pLayerTag);
}

const map<wstring, class CLayer*>* CGameInstance::Get_All_GameObjectLayerMap(_uint iLevelIndex)
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	if (m_pObject_Manager == nullptr)
		return nullptr;
	return m_pObject_Manager->Get_All_GameObjectLayerMap(iLevelIndex);
}

HRESULT CGameInstance::SetTransform(CPipeLine::E_TRANSFORMSTATETYPE eStateType, _fmatrix TransformMatrix)
{
	NULL_CHECK_HR(m_pPipeLine);
	return m_pPipeLine->SetTransform(eStateType, TransformMatrix);
}


_float4x4 CGameInstance::GetTransformFloat4x4(CPipeLine::E_TRANSFORMSTATETYPE eStateType)
{
	NULL_CHECK_BREAK(m_pPipeLine);
	return m_pPipeLine->GetTransformFloat4x4(eStateType);
}

_float4x4 CGameInstance::GetTransformFloat4x4_TP(CPipeLine::E_TRANSFORMSTATETYPE eStateType)
{
	NULL_CHECK_BREAK(m_pPipeLine);
	return m_pPipeLine->GetTransformFloat4x4_TP(eStateType);
}

_float4 CGameInstance::GetCameraPosition_float() const
{
	NULL_CHECK_BREAK(m_pPipeLine);
	return m_pPipeLine->GetCameraPosition_float();
}

_vector CGameInstance::GetCameraPosition_vec() const
{
	NULL_CHECK_BREAK(m_pPipeLine);
	return m_pPipeLine->GetCameraPosition_vec();
}

const LIGHTDESC * CGameInstance::Get_LightDesc(_uint iIndex) const
{
	NULL_CHECK_BREAK(m_pLightMgr);

	return m_pLightMgr->Get_LightDesc(iIndex);
}

HRESULT CGameInstance::Add_Light(ID3D11Device * device, ID3D11DeviceContext * context, const LIGHTDESC & desc)
{
	NULL_CHECK_BREAK(m_pLightMgr);

	return m_pLightMgr->Add_Light(device, context, desc);
}

HRESULT CGameInstance::FolderFinder(const wstring& FileFolder)
{
	NULL_CHECK_BREAK(m_pFileMgr);

	return m_pFileMgr->FolderFinder(FileFolder);
}

void CGameInstance::SaveVectorToDat(const wstring&  savetxtName, wstring exename)
{
	NULL_CHECK_BREAK(m_pFileMgr);
	m_pFileMgr->SaveVectorToDat(savetxtName, exename);
}

list<MYFILEPATH*> CGameInstance::Load_ExtensionList(const wstring& txtfilepath, string exe, bool bFlag)
{
	NULL_CHECK_BREAK(m_pFileMgr);
	return m_pFileMgr->Load_ExtensionList(txtfilepath, exe, bFlag);
}

wstring CGameInstance::Get_PathData(wstring Fullpath)
{
	NULL_CHECK_BREAK(m_pFileMgr);
	return m_pFileMgr->Get_PathData(Fullpath);
}

HRESULT CGameInstance::Transform_ToLocalSpace(_fmatrix WorldMatrixinverse)
{
	NULL_CHECK_BREAK(m_pPickMgr);

	FAILED_CHECK(m_pPickMgr->Transform_ToLocalSpace(WorldMatrixinverse));
	return S_OK;
}

_bool CGameInstance::isPick(_float3 * pLocalPoint, _float3 * pOut)
{
	NULL_CHECK_BREAK(m_pPickMgr);

	return m_pPickMgr->isPick(pLocalPoint, pOut);
}

_float3 CGameInstance::Get_PickPos() const
{
	NULL_CHECK_BREAK(m_pPickMgr);

	return m_pPickMgr->Get_PickPos();
}
_bool CGameInstance::Get_isPick() const
{
	NULL_CHECK_BREAK(m_pPickMgr);

	return m_pPickMgr->Get_isPick();
}

_ray CGameInstance::Get_Ray_World() const
{
	NULL_CHECK_BREAK(m_pPickMgr);
	return m_pPickMgr->Get_Ray_World();
}

_float2 CGameInstance::Get_WindowPos() const
{
	NULL_CHECK_BREAK(m_pPickMgr);
	return m_pPickMgr->Get_WindowPos();
}



_bool CGameInstance::IsIn_WorldSpace(_fvector vPoint, _float fRange)
{
	NULL_CHECK_BREAK(m_pFrstumMgr);

	return m_pFrstumMgr->IsIn_WorldSpace(vPoint,fRange);
}

_bool CGameInstance::IsIn_LocalSpace(_fvector vPoint, _float fRange)
{
	NULL_CHECK_BREAK(m_pFrstumMgr);

	return m_pFrstumMgr->IsIn_LocalSpace(vPoint, fRange);
}

HRESULT CGameInstance::Add_Font(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar * pFontTag, const _tchar * pFontFilePath)
{
	NULL_CHECK_BREAK(m_pFontMgr);

	return m_pFontMgr->Add_Font(pDevice,pDeviceContext,pFontTag,pFontFilePath);
}

HRESULT CGameInstance::Render_Font(const _tchar * pFontTag, const _tchar * pText, _float2 vPosition, _fvector vColor)
{
	NULL_CHECK_BREAK(m_pFontMgr);

	return m_pFontMgr->Render_Font(pFontTag, pText, vPosition, vColor);
}

void CGameInstance::Update_Debug()
{
	if (m_pInput_Device->Get_DIKeyState(DIK_1)& DIS_Down)
	{
		mIsRender_Collider = !mIsRender_Collider;
	}
	if (m_pInput_Device->Get_DIKeyState(DIK_2)& DIS_Down)
	{
		mIsRender_Collider_Navi = !mIsRender_Collider_Navi;
	}
}

_float CGameInstance::Easing(EasingTypeID eEasingType, _float fStartPoint, _float fTargetPoint, _float fPassedTime, _float fTotalTime)
{
	NULL_CHECK_BREAK(m_pEasingMgr);
	return m_pEasingMgr->Easing(eEasingType, fStartPoint, fTotalTime, fPassedTime, fTotalTime);
}

_float3 CGameInstance::Easing3(EasingTypeID eEasingType, _float3 fStartPoint, _float3 fTargetPoint, _float fPassedTime, _float fTotalTime)
{
	NULL_CHECK_BREAK(m_pEasingMgr);
	return m_pEasingMgr->Easing3(eEasingType, fStartPoint, fTargetPoint, fPassedTime, fTotalTime);
}

ID3D11ShaderResourceView * CGameInstance::Get_RenderTargetSRV(const _tchar * pTargetTag)
{
	if (nullptr == m_pRenderTargetMgr)
		return nullptr;
	return m_pRenderTargetMgr->Get_SRV(pTargetTag);
}

_int CGameInstance::Channel_VolumeUp(CHANNELID eID, _float _vol)
{
	NULL_CHECK_MSG(m_pSoundMgr, L"Not Have m_pSoundMgr");
	return m_pSoundMgr->Channel_VolumeUp(eID, _vol);
}

_int CGameInstance::Channel_VolumeDown(CHANNELID eID, _float _vol)
{
	NULL_CHECK_MSG(m_pSoundMgr, L"Not Have m_pSoundMgr");

	return m_pSoundMgr->Channel_VolumeDown(eID, _vol);
}

_int CGameInstance::Channel_Pause(CHANNELID eID)
{
	NULL_CHECK_MSG(m_pSoundMgr, L"Not Have m_pSoundMgr");

	return m_pSoundMgr->Channel_Pause(eID);
}

HRESULT CGameInstance::PlaySound(TCHAR * pSoundKey, CHANNELID eID, _float fLouderMultiple)
{
	NULL_CHECK_MSG(m_pSoundMgr, L"Not Have m_pSoundMgr");

	return m_pSoundMgr->PlaySound(pSoundKey, eID, fLouderMultiple);
}

HRESULT CGameInstance::PlayBGM(TCHAR * pSoundKey, _float fLouderMultiple)
{
	NULL_CHECK_MSG(m_pSoundMgr, L"Not Have m_pSoundMgr");

	return m_pSoundMgr->PlayBGM(pSoundKey, fLouderMultiple);
}

void CGameInstance::Stop_ChannelSound(CHANNELID eID)
{
	NULL_CHECK_MSG(m_pSoundMgr, L"Not Have m_pSoundMgr");

	m_pSoundMgr->Stop_ChannelSound(eID);

	return;
}

void CGameInstance::Stop_AllChannel()
{
	NULL_CHECK_MSG(m_pSoundMgr, L"Not Have m_pSoundMgr");
	m_pSoundMgr->Stop_AllChannel();
	return;
}

_float CGameInstance::Get_Channel_Volume(CHANNELID eID)
{
	NULL_CHECK_MSG(m_pSoundMgr, L"Not Have m_pSoundMgr");
	return 	m_pSoundMgr->Get_Channel_Volume(eID);
}

_bool CGameInstance::Get_Channel_IsPaused(CHANNELID eID)
{
	NULL_CHECK_MSG(m_pSoundMgr, L"Not Have m_pSoundMgr");
	return 	m_pSoundMgr->Get_Channel_IsPaused(eID);
}

HRESULT CGameInstance::PlayThread(void * _ThreadFunc, void * _pArg)
{
	if (m_pThreadMgr == nullptr)
		return E_FAIL;

	return m_pThreadMgr->PlayThread(_ThreadFunc, _pArg);
}

void CGameInstance::Release_Engine()
{
	if (0 != CGameInstance::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CGameInstance");

	if (0 != CTimer_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CTimer_Manager");

	if (0 != CObject_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CObject_Manager");

	if (0 != CComponent_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CComponent_Manager");

	if (0 != CLevel_Manager::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CLevel_Manager");

	if (0 != CLightMgr::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CLightMgr");

	if (0 != CRenderTargetMgr::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CRenderTargetMgr");



	if (0 != CPipeLine::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CPipeLine");

	if (0 != CFileInfo::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CFileInfo");

	if (0 != CPicking::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CPicking");

	if (0 != CFrustum::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CFrustum");

	if (0 != CFontMgr::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CFontMgr");

	if (0 != CEasingMgr::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CEasingMgr");

	if (0 != CSoundMgr::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CSoundMgr");

	if (0 != CThreadMgr::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CThreadMgr");

	if (0 != CInput_Device::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CInput_Device");

	if (0 != CGraphic_Device::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CGraphic_Device");

}

void CGameInstance::Free()
{
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pLightMgr);
	Safe_Release(m_pFileMgr);
	Safe_Release(m_pPickMgr);
	Safe_Release(m_pFrstumMgr);
	Safe_Release(m_pFontMgr);
	Safe_Release(m_pEasingMgr);
	Safe_Release(m_pRenderTargetMgr);
	Safe_Release(m_pSoundMgr);
	Safe_Release(m_pThreadMgr);
	
	

}