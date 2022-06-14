#pragma once

#include "GameObject_Base.h"

BEGIN(Engine)
class CModel;
END

// ��ƼŬ�� 3D ������Ʈ
// 10�� ���Ϸ� ���

BEGIN(Client)
class CGameObject_3D_Particle:
	public CGameObject_Base
{

protected:
	explicit CGameObject_3D_Particle(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject_3D_Particle(const CGameObject_3D_Particle& rhs);
	virtual ~CGameObject_3D_Particle() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

public:
	CModel*		Get_ComModel() const { return mComModel; }

protected:
	virtual HRESULT Set_Component()override;
	virtual HRESULT Set_ConstantTable_Model(); // �� ����


protected: // 3D�� Com / DESC �߰�
	CModel*						mComModel = nullptr;
	CTexture_map*				mComTextureMap = nullptr;

	// ��ƼŬ �� / �ؽ�ó �̸� / ��ƼŬ �Ӽ�
	string						mModelName;
	TEXTURE_NAMES_DESC			mTexStrDESC;
	PARTICLEDESC				mParticleDESC;


public:
	static CGameObject_3D_Particle* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_3D_Particle* Clone(void* pArg);
	virtual void Free() override;
};

END