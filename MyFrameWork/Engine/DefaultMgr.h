#pragma once

#include "Base.h"

/* 객체들의 원형을 생성해서 모아놓는다. */
/* 원형을 찾아서 복제한후, 모아서 관리한다. */

BEGIN(Engine)

class CDefaultMgr final : public CBase
{
	DECLARE_SINGLETON(CDefaultMgr)
public:
	CDefaultMgr();
	virtual ~CDefaultMgr() = default;

public:
	_int Tick(_double TimeDelta);
	_int LateTick(_double TimeDelta);

public:
	virtual void Free() override;
};

END