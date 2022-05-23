#include "stdafx.h"
#include "GameObject/Dungeon_Objects.h"

CDungeon_Objects::CDungeon_Objects()
{
}

HRESULT CDungeon_Objects::Init_GameLevel(E_LEVEL level)
{
	if (level == LEVEL_MYGAMEPLAY)
	{
		mCurrentLevel = level;
		Ready_Light();

		// 주면 지형 생성
		// FAILED_CHECK(Ready_Light());
		// FAILED_CHECK(Ready_Camera());
		// FAILED_CHECK(Ready_Layer_Mouse());
		// FAILED_CHECK(Ready_Layer_BackGround());
		// FAILED_CHECK(Ready_Level_BaseGame());

		


		return S_OK;
	}

	return E_FAIL;
}

HRESULT CDungeon_Objects::Release_Level()
{
	return S_OK;
}

HRESULT CDungeon_Objects::Ready_Light()
{
	// 라이트 세팅

	return S_OK;
}

HRESULT CDungeon_Objects::Ready_Camera()
{
	// 카메라 세팅

	return S_OK;
}

HRESULT CDungeon_Objects::Ready_BackGround()
{
	return S_OK;
}

HRESULT CDungeon_Objects::Ready_GameObjects()
{
	return S_OK;
}

CDungeon_Objects * CDungeon_Objects::Create(E_LEVEL level)
{
	CDungeon_Objects* pInstance = NEW CDungeon_Objects();


	if (FAILED(pInstance->Init_GameLevel(level)))
	{
		MSGBOX("Failed to Created CDungeon_Objects");
		Safe_Release(pInstance);
	}
	return pInstance;

}

void CDungeon_Objects::Free()
{
}
