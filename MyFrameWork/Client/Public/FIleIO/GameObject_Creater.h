#pragma once
#include "Base.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

// 깡통오브젝트를 외부파일정보에 맞게 생성한다.
// 부모 자식관계가 있다면 해당 형태로 만들어서 넘긴다.

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

	// 각 데이터별로 로드

	// 생성된 오브젝트 이름들을 리스트에 저장해두자
	// 이 이름들이 프리펩의 이름들
	list<string> mList_GameObject_ProtoNames;

	// 생성될 

};
END