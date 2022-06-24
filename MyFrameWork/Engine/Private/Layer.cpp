#include "..\Public\Layer.h"
#include "GameObject.h"

CLayer::CLayer()
{
}

CComponent * CLayer::Get_Component(const _tchar * pComponentTag, _uint iIndex)
{
	if (iIndex >= m_Objects.size())
		return nullptr;

	auto	iter = m_Objects.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->Get_Component(pComponentTag);
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	m_Objects.push_back(pGameObject);

	return S_OK;
}

_int CLayer::Tick(_double TimeDelta)
{
	for (auto& pGameObject : m_Objects)
	{
		if (pGameObject->Get_IsLife() == false)
			continue;

		if (pGameObject->Tick(TimeDelta) < 0)
			return -1;
	}

	return 0;
}

_int CLayer::LateTick(_double TimeDelta)
{
	for (auto& pGameObject : m_Objects)
	{
		if (pGameObject->Get_IsLife() == false)
			continue;

		if (pGameObject->LateTick(TimeDelta) < 0)
			return -1;
	}

	// 죽은 오브젝트 실제 삭제
	Release_DeadObejct();

	return 0;
}

CLayer * CLayer::Create()
{
	CLayer* pLayer = NEW CLayer();

	return pLayer;
}

void CLayer::Free()
{
	for (auto& pGameObject : m_Objects)
		Safe_Release(pGameObject);

	m_Objects.clear();
}

HRESULT CLayer::Release_DeadObejct()
{
	// 죽은 오브젝트 리스트 삭제
	OBJECTS::iterator iter_begin = m_Objects.begin();
	OBJECTS::iterator iter_end = m_Objects.end();

	for (OBJECTS::iterator iter = iter_begin; iter != m_Objects.end();)
	{
		if ((*iter)->Get_IsLife() == false)
		{
			Safe_Release(*iter);
			iter = m_Objects.erase(iter);
		}
		else
			iter++;
	}

	return S_OK;
}