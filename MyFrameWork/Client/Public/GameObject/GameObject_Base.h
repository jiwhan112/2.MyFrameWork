#pragma once

#include "GameObject.h"

// 2D에 해당하는 UI / 파티클을 만들기 위한 오브젝트

BEGIN(Engine)
class CShader;
class CRenderer;
class CVIBuffer;
//class CTexture;
class CTexture_map;
END

BEGIN(Client)

// 클라이언트에서 사용하는 기본 게임 오브젝트
// 컴포넌트 반환 / 기본 컴포넌트 세팅 / 기본 기능 정의
class CGameObject_Base abstract:
	public CGameObject
{
protected:
	explicit CGameObject_Base(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject_Base(const CGameObject_Base& rhs);
	virtual ~CGameObject_Base() = default;


public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();




public: // 컴포넌트 반환
	CShader*		Get_ComShader() { return mComShader; }
	CRenderer*		Get_ComRenderer() { return mComRenderer; }
	CVIBuffer*		Get_ComVIBuffer() { return mComVIBuffer; }
	CTexture_map*	Get_ComTexture() { return mComTexture; }


public:

	void Set_LoadTexDesc(const TEXTUREDESC& desc);

	CTexture_map* Get_TextureMap() const
	{
		return mComTexture;
	}

	const TEXTUREDESC& Get_TextureDesc() const { return mTexStrDESC; }

protected:
	// 월드와 텍스처 빛 정보 셰이더 기본 세팅
	virtual HRESULT Set_ConstantTable_World();
	virtual HRESULT Set_ConstantTable_Tex();
	virtual HRESULT Set_ConstantTable_Light(_uint lightid = 0);

	bool PickObject_3D();

protected:
	_uint			mCurrentShaderPass = 0;

	// 기본 컴포넌트
	CShader*		mComShader		= nullptr;
	CRenderer*		mComRenderer	= nullptr;
	CVIBuffer*		mComVIBuffer	= nullptr;
	CTexture_map*	mComTexture		= nullptr;

protected: // DESC
	TEXTUREDESC		mTexStrDESC;



public:
	virtual void Free() override;
};

END