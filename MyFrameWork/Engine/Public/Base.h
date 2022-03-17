#pragma once

#include "Engine_Defines.h"


// ��� ��ü���� �θ��̸� ���۷��� ī��Ʈ ����
BEGIN(Engine)

class ENGINE_DLL CBase abstract
{
protected:
	CBase();
	virtual ~CBase() = default;
public:
	unsigned long AddRef(); /* RefCnt�� ������Ų��. */
	unsigned long Release(); /* RefCnt�� ���ҽ�Ų��. or  �����Ѵ�. */

private:
	unsigned long			m_dwRefCnt = 0;	

public:
	virtual void Free() = 0;
};

END