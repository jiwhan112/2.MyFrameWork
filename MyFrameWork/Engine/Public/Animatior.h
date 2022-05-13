#pragma once

#include "Base.h"

BEGIN(Engine)

// 애니메이션 상태를 관리하는 컴포넌트
// 맵에 저장된 애니메이션 대로 수행하게 만들자..

class ENGINE_DLL CAnimatior final 
	: public CBase
{
private:
	CAnimatior();
	virtual ~CAnimatior() = default;

public: // Get Set



public: // 관리할 애니메이션을 넣는다.
	HRESULT NativeConstruct();


	

public:
	static CAnimatior* Create();
	virtual void Free() override;
};

END