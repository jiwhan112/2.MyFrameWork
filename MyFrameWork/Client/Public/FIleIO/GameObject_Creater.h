#pragma once
#include "Base.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

// ������Ʈ ������

// �ܺ� �����͸� �޾ƿͼ� ������Ÿ���� �����Ѵ�.

// ������Ʈ�� ������Ʈ�� �ٸ��� ������ �׳� ���⼭ �����ص���.
class CGameObject_Creater
	:public CBase
{

private:
	CGameObject_Creater() = default;
	~CGameObject_Creater() = default;




private:

	// �� �����ͺ��� �ε�

	// ������ ������Ʈ �̸����� ����Ʈ�� �����ص���
	list<string> mList_GameObject_ProtoNames;


};
END