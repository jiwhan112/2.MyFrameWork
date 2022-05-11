#pragma once

#include "GameObject_2D.h"

BEGIN(Client)

// ���콺 ������Ʈ ó��
// �� ���ӿ����� ���콺 == �÷��̾��̴�.
class CGameObject_Mouse final :
	public CGameObject_2D
{
protected:
	explicit CGameObject_Mouse(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject_Mouse(const CGameObject_Mouse& rhs);
	virtual ~CGameObject_Mouse() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	virtual _int Tick(_double TimeDelta) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;



#ifdef _DEBUG
private:
	BasicEffect*									mBaseEffect = nullptr;
	ID3D11InputLayout*								mInputLayout = nullptr;
	PrimitiveBatch<DirectX::VertexPositionColor>*	mBatch = nullptr;
#endif // _DEBUG



protected: // DESC
	_float2			mMousePos;
	_viewport		mViewPort;

	bool			mIsRender = false;
	_ray			mRay;

public:
	static CGameObject_Mouse* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_Mouse* Clone(void* pArg);
	virtual void Free() override;
};

END