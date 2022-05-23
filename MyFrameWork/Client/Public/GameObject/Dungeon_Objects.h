#pragma once

#include "Base.h"

BEGIN(Client)

// 게임 오브젝트 생성 저장
class CDungeon_Objects final :
	public CBase
{

protected:
	explicit CDungeon_Objects();
	virtual ~CDungeon_Objects() = default;

public:
	// 레벨마다 초기화
	HRESULT Init_GameLevel(E_LEVEL level);
	HRESULT Release_Level();

private:
	// 세팅
	HRESULT Ready_Light();
	HRESULT Ready_Camera();
	HRESULT Ready_BackGround();
	HRESULT Ready_GameObjects();


private:
	// 던전 오브젝트 정의

	// 라이팅


	// 오브젝트

	E_LEVEL mCurrentLevel = LEVEL_END;

public:
	static CDungeon_Objects* Create( E_LEVEL level);
	virtual void Free() override;
};

END