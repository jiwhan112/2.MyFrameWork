#pragma once

#include "Component.h"

/* �ΰ��ӳ��� �׷������ϴ� ��ü���� ��Ƽ� ����(�׷����� �������.) �Ѵ�. */
/* ��ü���� �׸��� �Լ�(Render) �� ȣ�����ֳ�. */

BEGIN(Engine)

class ENGINE_DLL CRenderer final : public CComponent
{
public:
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_NONBLEND_FIRST,RENDER_NONBLEND_SECOND, RENDER_BLEND, RENDER_UI, RENDER_END };
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
	list<class CGameObject*>				mRenderObjects[RENDER_END];
	typedef	list <class CGameObject* >		RENDEROBJECTS;

private:
	HRESULT Render_Priority();
	HRESULT Render_NonAlpha_First();
	HRESULT Render_NonAlpha_Second();
	HRESULT Render_Alpha();
	HRESULT Render_UI();

public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free();
};

END