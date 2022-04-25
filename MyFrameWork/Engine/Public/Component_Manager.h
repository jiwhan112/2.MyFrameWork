#pragma once

// #Components ������Ʈ ����
#include "Renderer.h"
#include "Transform.h"
#include "Texture.h"
#include "Texture_map.h"
#include "Shader.h"
#include "Model.h"
#include "Collider.h"

#include "VIBuffer_Rect.h"
#include "VIBuffer_Cube.h"
#include "VIBuffer_Terrain.h"

/* ������Ʈ �������� ��Ƴ��´�. */

BEGIN(Engine)

class CComponent_Manager final : public CBase
{
	DECLARE_SINGLETON(CComponent_Manager)
public:
	CComponent_Manager();
	virtual ~CComponent_Manager() = default;

public:
	HRESULT Reserve_Container(_uint iNumLevels);
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, class CComponent* pPrototype);
	CComponent* Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg);
	HRESULT Clear_LevelObject(_uint iLevelIndex);


private:
	map<const _tchar*, class CComponent*>*				mMapProtos = nullptr;
	typedef map<const _tchar*, class CComponent*>		PROTOTYPES;

private:
	_uint			m_iNumLevels;

private:
	class CComponent* Find_Component(_uint iLevelIndex, const _tchar* pPrototypeTag);

public:
	virtual void Free() override;
};

END