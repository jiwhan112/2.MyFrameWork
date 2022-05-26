#include "stdafx.h"
#include "Level/Level_MyGamePlay.h"
#include "Tool/Imgui_MyDemo.h"
#include "Tool/Imgui_InGame.h"
#include "Camera_Client.h"

#include "GameObject/Client_Object.h"

CLevel_MyGamePlay::CLevel_MyGamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
{
	mLevelIndex = LEVEL_MYGAMEPLAY;
}

HRESULT CLevel_MyGamePlay::NativeConstruct()
{
	FAILED_CHECK(__super::NativeConstruct());
	// 파일 원령객체 초기화
	CGameObject_Creater* Create_Manager = GetSingle(CGameManager)->Get_CreaterManager();
	Create_Manager->LoaderDatFile_For_PrototypeObject();

	FAILED_CHECK(GetSingle(CGameManager)->Get_DaungonManager()->NativeConstruct_Level((E_LEVEL)mLevelIndex));
	return S_OK;
}

_int CLevel_MyGamePlay::Tick(_double TimeDelta)
{
	FAILED_UPDATE(__super::Tick(TimeDelta));

	return UPDATENONE;
}

_int CLevel_MyGamePlay::LateTick(_double TimeDelta)
{
	FAILED_UPDATE(__super::LateTick(TimeDelta));
	return UPDATENONE;
}

HRESULT CLevel_MyGamePlay::Render()
{
	FAILED_CHECK(__super::Render());
	return S_OK;
}


CLevel_MyGamePlay * CLevel_MyGamePlay::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CLevel_MyGamePlay*	pInstance = NEW CLevel_MyGamePlay(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Creating CLevel_MyGamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_MyGamePlay::Free()
{
	__super::Free();
}