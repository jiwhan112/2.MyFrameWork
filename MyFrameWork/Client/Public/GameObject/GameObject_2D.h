#pragma once

#include "GameObject_Base.h"

BEGIN(Client)

// ��� UI ó��
class CGameObject_2D :
	public CGameObject_Base
{
public:
	enum E_UI_TYPE
	{
		UITYPE_LOGO,	// ȭ��
		UITYPE_IMAGE,	// �ܼ� �̹���
		UITYPE_BUTTON1, // Ŭ���̺�Ʈ
		UITYPE_BUTTON2_UNIT, // ���� Ŭ���̺�Ʈ2
		UITYPE_TEXT,	// �ؽ�Ʈ
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

	// ������������ ���� ū���� ���߿� �׸���.
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

	// ���̴� �ѱ��
	virtual HRESULT Set_ConstantTable_UI(); // UI Cam
	virtual HRESULT Set_ConstantTable_Tex(); // �ؽ�ó ����

	void Set_ViewportSize(); // ȭ�� ������ �ʱ�ȭ

	void LateTick_Button1();
	void LateTick_Button2_Unit();

protected: // UI���� Com / DESC �߰�

	CVIBuffer*		mComVIBuffer = nullptr;
	CTexture_map*	mComTexture = nullptr;

	// UI ��ġ�� �ؽ�ó ����
	UI_DESC					mUiDesc;
	TEXTURE_NAMES_DESC		mTexStrDESC;

	E_UI_TYPE				meUIType = UITYPE_LOGO;

private: // setupMat
	_float4x4 mWorldmat, mViewmat, mProjmat;

	_bool mIsOverrMouse = false;			// ���콺 �浹
	_bool mIsActive = false;				// ���� 


public:

	static CGameObject_2D* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_2D* Clone(void* pArg);
	virtual void Free() override;
};

END