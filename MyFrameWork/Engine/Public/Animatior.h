#pragma once

#include "Base.h"

BEGIN(Engine)

// �ִϸ��̼� ���¸� �����ϴ� ������Ʈ
// �ʿ� ����� �ִϸ��̼� ��� �����ϰ� ������..

class ENGINE_DLL CAnimatior final 
	: public CBase
{
private:
	CAnimatior();
	virtual ~CAnimatior() = default;

public: // Get Set



public: // ������ �ִϸ��̼��� �ִ´�.
	HRESULT NativeConstruct();


	

public:
	static CAnimatior* Create();
	virtual void Free() override;
};

END