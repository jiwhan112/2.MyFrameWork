#pragma once

#include "GameObject_Base.h"

BEGIN(Engine)
class CModel;
END

// 파티클용 3D 오브젝트
// 10개 이하로 사용

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
	virtual HRESULT Set_ConstantTable_Model(); // 모델 설정


protected: // 3D모델 Com / DESC 추가
	CModel*						mComModel = nullptr;
	CTexture_map*				mComTextureMap = nullptr;

	// 파티클 모델 / 텍스처 이름 / 파티클 속성
	string						mModelName;
	TEXTURE_NAMES_DESC			mTexStrDESC;
	PARTICLEDESC				mParticleDESC;


public:
	static CGameObject_3D_Particle* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_3D_Particle* Clone(void* pArg);
	virtual void Free() override;
};

END