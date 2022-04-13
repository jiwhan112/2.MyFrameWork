#pragma once
#include "Base.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

// ���������Ʈ�� �ܺ����������� �°� �����Ѵ�.
// �θ� �ڽİ��谡 �ִٸ� �ش� ���·� ���� �ѱ��.

class CGameObject_Creater
	:public CBase
{

private:
	CGameObject_Creater() = default;
	~CGameObject_Creater() = default;

public:
	HRESULT NativeConstruct();

	HRESULT Creater2D(const char* datapath);

private:

	// �� �����ͺ��� �ε�

	// ������ ������Ʈ �̸����� ����Ʈ�� �����ص���
	// �� �̸����� �������� �̸���
	list<string> mList_GameObject_ProtoNames;

	// ������ 

};
END