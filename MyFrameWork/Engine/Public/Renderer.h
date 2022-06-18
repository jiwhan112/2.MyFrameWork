#pragma once

#include "Component.h"

/* 인게임내에 그려져야하는 객체들을 모아서 보관(그려지는 순서대로.) 한다. */
/* 객체들의 그리는 함수(Render) 를 호출해주낟. */

BEGIN(Engine)

class ENGINE_DLL CRenderer final : public CComponent
{
public:
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_NONBLEND_FIRST,RENDER_NONBLEND_SECOND,RENDER_NONLIGHT, RENDER_BLEND, RENDER_UI, RENDER_END };

private:
	explicit CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CRenderer() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	HRESULT Add_RenderGroup(RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);
	HRESULT Render();

	// 랜더 타겟과 조명 
private:
	class CRenderTargetMgr*					mRenderTargetManager	= nullptr;
	class CLightMgr*						mLightManager			= nullptr;

private:
	class CVIBuffer_Rect*					mComVIRECT	= nullptr;
	class CShader*							mComShader		= nullptr;

	_float4x4								mWorldMat;
	_float4x4								mViewdMat;
	_float4x4								mProjMat;

private:
	list<class CGameObject*>				mRenderObjects[RENDER_END];
	typedef	list <class CGameObject* >		RENDEROBJECTS;

private:
	HRESULT Render_Priority();
	HRESULT Render_NonAlpha();
	HRESULT Render_Lights();
	HRESULT Render_Blend();
	HRESULT Render_NoLights();
	HRESULT Render_Alpha();
	HRESULT Render_UI();
	
private:
	HRESULT RenderTargetSetting();


public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free();
};

END