#pragma once

#include "Base.h"

BEGIN(Client)

// ���� ������Ʈ ���� ����
class CDungeon_Objects final :
	public CBase
{

protected:
	explicit CDungeon_Objects();
	virtual ~CDungeon_Objects() = default;

public:
	// �������� �ʱ�ȭ
	HRESULT Init_GameLevel(E_LEVEL level);
	HRESULT Release_Level();

private:
	// ����
	HRESULT Ready_Light();
	HRESULT Ready_Camera();
	HRESULT Ready_BackGround();
	HRESULT Ready_GameObjects();


private:
	// ���� ������Ʈ ����

	// ������


	// ������Ʈ

	E_LEVEL mCurrentLevel = LEVEL_END;

public:
	static CDungeon_Objects* Create( E_LEVEL level);
	virtual void Free() override;
};

END