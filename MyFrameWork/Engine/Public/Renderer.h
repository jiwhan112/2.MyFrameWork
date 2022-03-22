#pragma once

#include "Component.h"

/* �ΰ��ӳ��� �׷������ϴ� ��ü���� ��Ƽ� ����(�׷����� �������.) �Ѵ�. */
/* ��ü���� �׸��� �Լ�(Render) �� ȣ�����ֳ�. */

BEGIN(Engine)

class ENGINE_DLL CRenderer final : public CComponent
{	
public:
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END };
public:
	explicit CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CRenderer() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	HRESULT Add_RenderGroup(RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);
	HRESULT Render();

private:
	list<class CGameObject*>				m_RenderObjects[RENDER_END];
	typedef	list <class CGameObject* >		RENDEROBJECTS;

private:
	HRESULT Render_Priority();
	HRESULT Render_NonAlpha();
	HRESULT Render_Alpha();
	HRESULT Render_UI();

public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free();
};

END