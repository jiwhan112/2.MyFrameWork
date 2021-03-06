#pragma once

#include "Base.h"

/* 객체들의 원형을 생성해서 모아놓는다. */
/* 원형을 찾아서 복제한후, 모아서 관리한다. */

BEGIN(Engine)

class CObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CObject_Manager)
public:
	CObject_Manager();
	virtual ~CObject_Manager() = default;
public:
	class CComponent* Get_Component(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex);
	const list<class CGameObject*>* Get_GameObjectLayerList(_uint iLevelIndex, const _tchar* pLayerTag);
	const map<wstring, class CLayer*>* Get_All_GameObjectLayerMap(_uint iLevelIndex);

public:
	HRESULT			Reserve_Container(_uint iNumLevels);
	HRESULT			Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype); /* 원형객체를 추가한다. */
	CGameObject*	Add_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pPrototypeTag, void* pArg); /* 사본(게임내에 출현해야할 객체) 객체를 추가한다. */
	// 이미 만들어진 오브젝트 추가
	HRESULT			Push_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, CGameObject* cloneObj);

	// 원형오브젝트를 클론해서 넘겨준다.
	CGameObject*	Create_GameObject(const _tchar* pPrototypeTag, void* pArg);
	HRESULT			Clear_LevelObject(_uint iLevelIndex);
public:
	_int Tick(_double TimeDelta);
	_int LateTick(_double TimeDelta);
private:
	map<wstring, class CGameObject*>			m_Prototypes;
	typedef map<wstring, class CGameObject*>	PROTOTYPES;
private:
	map<wstring, class CLayer*>*			m_pLayers = nullptr;
	typedef map<wstring, class CLayer*>	LAYERS;

	_uint			m_iNumLevels = 0;

private:
	class CGameObject* Find_Prototype(const _tchar* pPrototypeTag);
	class CLayer* Find_Layer(_uint iLevelIndex, const _tchar* pLayerTag);

public:
	virtual void Free() override;
};

END