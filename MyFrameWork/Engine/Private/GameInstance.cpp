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
}

HRESULT CGameInstance::Initialize_Engine(HINSTANCE hInstance, _uint iNumLevels, const CGraphic_Device::GRAPHICDESC & GraphicDesc, ID3D11Device ** ppDeviceOut, ID3D11DeviceContext ** ppDeviceContextOut)
{
	if (nullptr == m_pGraphic_Device ||
		nullptr == m_pObject_Manager)
		return E_FAIL;

	FAILED_CHECK(m_pGraphic_Device->Ready_Graphic_Device(GraphicDesc.hWnd, GraphicDesc.eWinMode, GraphicDesc.iWinCX, GraphicDesc.iWinCY, ppDeviceOut, ppDeviceContextOut));
	FAILED_CHECK(m_pInput_Device->Ready_Input_Device(hInstance, GraphicDesc.hWnd,0.3f));
	FAILED_CHECK(m_pObject_Manager->Reserve_Container(iNumLevels));
	FAILED_CHECK(m_pComponent_Manager->Reserve_Container(iNumLevels));

	return S_OK;
}

_int CGameInstance::Tick_Engine(_double TimeDelta)
{
	if (nullptr == m_pLevel_Manager ||
		nullptr == m_pObject_Manager)
		return -1;

	// INPUT
	FAILED_CHECK(m_pInput_Device->SetUp_InputDeviceState(TimeDelta));

	// Tick
	FAILED_CHECK(m_pLevel_Manager->Tick(TimeDelta));
	FAILED_CHECK(m_pObject_Manager->Tick(TimeDelta));

	// PipeLine
	FAILED_CHECK(m_pPipeLine->Tick());

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

	return m_pObject_Manager->Push_GameObject(levelindex,pLayerTag,obj);
}

HRESULT CGameInstance::SetTransform(CPipeLine::E_TRANSFORMSTATETYPE eStateType, _fmatrix TransformMatrix)
{
	NULL_CHECK_HR(m_pPipeLine);
	return m_pPipeLine->SetTransform(eStateType, TransformMatrix);
}

_matrix CGameInstance::GetTransformMatrix(CPipeLine::E_TRANSFORMSTATETYPE eStateType)
{
	NULL_CHECK_BREAK(m_pPipeLine);
	return m_pPipeLine->GetTransformMatrix(eStateType);
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

void CGameInstance::SaveVectorToDat(const wstring&  savetxtName)
{
	NULL_CHECK_BREAK(m_pFileMgr);
	m_pFileMgr->SaveVectorToDat(savetxtName);
}

list<MYFILEPATH*> CGameInstance::Load_ExtensionList(const wstring& txtfilepath, string exe)
{
	NULL_CHECK_BREAK(m_pFileMgr);
	return m_pFileMgr->Load_ExtensionList(txtfilepath,exe);
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

	if (0 != CPipeLine::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CPipeLine");

	if (0 != CFileInfo::GetInstance()->DestroyInstance())
		MSGBOX("Failed to Delete CFileInfo");

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
}