#pragma once
#include "Base.h"

BEGIN(Engine)
// �Ŵ��� Ŭ���� ����Ʈ �ڵ�
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

