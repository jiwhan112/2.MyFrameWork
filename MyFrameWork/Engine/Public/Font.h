#pragma once

#include "Base.h"

BEGIN(Engine)

class CFont final : public CBase
{

private:
	CFont(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CFont() = default;

public:
	HRESULT NativeConstruct(const _tchar* pFontFilePath);
	HRESULT Render(const _tchar* pText, _float2 vPosition, _fvector vColor);

private:
	ID3D11Device*							mDevice = nullptr;
	ID3D11DeviceContext*					mDeviceContext = nullptr;

private:
	// �⺻ ���̴� ����
	BasicEffect*							mBaseEffect = nullptr;
	// ���̴��� �ʿ��� ���̾ƿ�
	ID3D11InputLayout*						mInputLayout = nullptr;

private:
	SpriteBatch*							mBatch = nullptr;
	SpriteFont*								mFont = nullptr;

public:
	// ��� ��ġ�� ��Ʈ ��ü ����
	static CFont* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pFontFilePath);
	virtual void Free() override;
};

END