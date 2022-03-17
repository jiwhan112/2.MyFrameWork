#pragma once
#include "Base.h"

BEGIN(Engine)
// 매니저 클래스 디폴트 코드
class CMgrDefault :public CBase
{

	DECLARE_SINGLETON(CEasingMgr);

private:
	explicit CEasingMgr();
	virtual ~CEasingMgr() = default;

public:
	virtual void Free() override;

};

END

