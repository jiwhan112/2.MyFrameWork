#pragma once

#include "Base.h"

/* ��ü���� ������ �����ؼ� ��Ƴ��´�. */
/* ������ ã�Ƽ� ��������, ��Ƽ� �����Ѵ�. */

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