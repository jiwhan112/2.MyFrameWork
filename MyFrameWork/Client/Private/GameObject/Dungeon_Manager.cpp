#include "stdafx.h"
#include "GameObject/Dungeon_Manager.h"
#include "GameObject/Dungeon_Objects.h"

CDungeon_Manager::CDungeon_Manager()
{
	mDungeon_Objects = nullptr;
	mDevice = nullptr;
	mDeviceContext = nullptr;

}

HRESULT CDungeon_Manager::NativeConstruct(ID3D11Device* device,ID3D11DeviceContext* context)
{
	mDevice = device;
	mDeviceContext = context;

	Safe_AddRef(mDevice);
	Safe_AddRef(mDeviceContext);

	return S_OK;
}

_int CDungeon_Manager::Tick(_double TimeDelta)
{

	return UPDATENONE;
}

_int CDungeon_Manager::LateTick(_double TimeDelta)
{
	return UPDATENONE;
}

HRESULT CDungeon_Manager::NativeConstruct_Level(E_LEVEL level)
{
	if (mDungeon_Objects == nullptr)
	{
		mDungeon_Objects = CDungeon_Objects::Create(mDevice, mDeviceContext, level);
		NULL_CHECK_HR(mDungeon_Objects);
	}

	return S_OK;
}

void CDungeon_Manager::Release_DaungonData()
{
	Safe_Release(mDungeon_Objects);
}

CGameObject_3D_Tile * CDungeon_Manager::FInd_TIleForIndex(_int TileIndex) const
{
	NULL_CHECK_BREAK(mDungeon_Objects);
	return mDungeon_Objects->FInd_TIleForIndex(TileIndex);
}

HRESULT CDungeon_Manager::RemoveTile(CGameObject_3D_Tile * pTIle)
{
	NULL_CHECK_BREAK(mDungeon_Objects);
	return mDungeon_Objects->RemoveTile(pTIle);
}

HRESULT CDungeon_Manager::Setup_TileState(_int tileIndex)
{
	NULL_CHECK_BREAK(mDungeon_Objects);
	return mDungeon_Objects->Setup_TileState(tileIndex);
}

CDungeon_Manager * CDungeon_Manager::Create(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	CDungeon_Manager* pInstance = NEW CDungeon_Manager();

	if (FAILED(pInstance->NativeConstruct(device,deviceContext)))
	{
		MSGBOX("Failed to Created CDungeon_Manager");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CDungeon_Manager::Free()
{
	Release_DaungonData();
	Safe_Release(mDevice);
	Safe_Release(mDeviceContext);

}


