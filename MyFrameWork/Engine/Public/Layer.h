#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CLayer final : public CBase
{
public:
	CLayer();
	virtual ~CLayer() = default;
public:
	class CComponent* Get_Component(const _tchar* pComponentTag, _uint iIndex);

public:
	HRESULT Add_GameObject(class CGameObject* pGameObject);
	_int Tick(_double TimeDelta);
	_int LateTick(_double TimeDelta);

private:
	list<class CGameObject*>			m_Objects;
	typedef list<class CGameObject*>	OBJECTS;
public:
	static CLayer* Create();
	virtual void Free();
};

END