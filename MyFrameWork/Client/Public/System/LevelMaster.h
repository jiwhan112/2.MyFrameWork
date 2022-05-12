#pragma once

#include "Base.h"

BEGIN(Client)
// ���� ���� ������
class CLevelMaster final :
	public CBase
{
private:
	explicit CLevelMaster();
	explicit CLevelMaster(const CLevelMaster& rhs);
	virtual ~CLevelMaster() = default;



public:
	static CLevelMaster* Create();
	virtual void Free() override;
};
END
