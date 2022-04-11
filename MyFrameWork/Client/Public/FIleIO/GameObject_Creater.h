#pragma once
#include "Base.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

// 오브젝트 생성기

// 외부 데이터를 받아와서 프로토타입을 생성한다.

// 컴포넌트나 오브젝트외 다른것 생성도 그냥 여기서 정의해두자.
class CGameObject_Creater
	:public CBase
{

private:
	CGameObject_Creater() = default;
	~CGameObject_Creater() = default;




private:

	// 각 데이터별로 로드

	// 생성된 오브젝트 이름들을 리스트에 저장해두자
	list<string> mList_GameObject_ProtoNames;


};
END