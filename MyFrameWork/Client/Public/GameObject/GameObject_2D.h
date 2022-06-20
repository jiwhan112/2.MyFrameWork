#pragma once

#include "GameObject_Base.h"

BEGIN(Client)

// 모든 UI 처리
class CGameObject_2D :
	public CGameObject_Base
{
public:
	enum E_UI_TYPE
	{
		UITYPE_LOGO,	// 화면
		UITYPE_IMAGE,	// 단순 이미지
		UITYPE_BUTTON1, // 클릭이벤트
		UITYPE_BUTTON2_UNIT, // 조건 클릭이벤트2
		UITYPE_TEXT,	// 텍스트
		UITYPE_END,
	};

protected:
	explicit CGameObject_2D(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject_2D(const CGameObject_2D& rhs);
	virtual ~CGameObject_2D() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

	virtual HRESULT CollisionFunc(CGameObject_Base* object);


public:
	CTexture_map* Get_TextureMap() const { return mComTexture; }
	CVIBuffer* Get_VIBuffer() const { return mComVIBuffer; }

	void Set_LoadUIDesc(const UI_DESC& desc)
	{
		memcpy(&mUiDesc, &desc, sizeof(UI_DESC));
	}
	const UI_DESC& Get_UIDesc() const { return mUiDesc; }

	void Set_LoadTexDesc(const TEXTURE_NAMES_DESC& desc);
	const TEXTURE_NAMES_DESC& Get_TextureDesc() const { return mTexStrDESC; }

	// 오름차순으로 정렬 큰값을 나중에 그린다.
	virtual const _int& Get_Depth() override { return mUiDesc.mDepth; }

	HRESULT Setup_UIType(E_UI_TYPE e);
	HRESULT Get_UIType()const { return meUIType; }

	HRESULT Setup_UIPosition(_float fX, _float fY, _float fSizeX, _float fSizeY, _float depth);
	HRESULT Setup_UIPosition(UI_DESC desc);
	HRESULT Setup_UIPosition();
	void Set_LoadTexDiffuse(const char* str);
	void Set_LoadTexButton(const char* str1, const char* str2);
	void Set_LoadTexButton2(const char* str1, const char* str2, const char* str3);



protected:
	virtual HRESULT Set_Component()override;

	// 셰이더 넘기기
	virtual HRESULT Set_ConstantTable_UI(); // UI Cam
	virtual HRESULT Set_ConstantTable_Tex(); // 텍스처 설정

	void Set_ViewportSize(); // 화면 사이즈 초기화

	void LateTick_Button1();
	void LateTick_Button2_Unit();

protected: // UI에서 Com / DESC 추가

	CVIBuffer*		mComVIBuffer = nullptr;
	CTexture_map*	mComTexture = nullptr;

	// UI 위치와 텍스처 정보
	UI_DESC					mUiDesc;
	TEXTURE_NAMES_DESC		mTexStrDESC;

	E_UI_TYPE				meUIType = UITYPE_LOGO;

private: // setupMat
	_float4x4 mWorldmat, mViewmat, mProjmat;

	_bool mIsOverrMouse = false;			// 마우스 충돌
	_bool mIsActive = false;				// 조건 


public:

	static CGameObject_2D* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_2D* Clone(void* pArg);
	virtual void Free() override;
};

END