#pragma once

#include "GameObject_3D_Static.h"
#include "Model.h"


BEGIN(Client)

// 소켓 오브젝트
// 무조건 Dynamic 오브젝트에 종속적으로 생성
class CGameObject_3D_Socket  final
	:public CGameObject_3D_Static
{

public:
	typedef struct tagSocketDesc
	{
		CModel*			mTargetModel = nullptr;
		CTransform*		mTransform = nullptr;
		const char*		mSocketName = nullptr;
	}SOCKETDESC;

protected:
	explicit CGameObject_3D_Socket(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject_3D_Socket(const CGameObject_3D_Socket& rhs);
	virtual ~CGameObject_3D_Socket() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	virtual _int Tick(_double TimeDelta)override;

	virtual HRESULT Render() override;

	//virtual HRESULT CollisionFunc(_float3 PickPosition, _float dist) override;

public:
	HRESULT		Set_LoadSocketDESC(const char* MyFbxname, const SOCKETDESC& desc);
	const SOCKETDESC&	Get_SocketDesc() const { return mSocketDESC; }
	virtual HRESULT Set_Component()override;

protected: // 소켓의 정보 / 타겟의 뼈 행렬 정보 
	SOCKETDESC				mSocketDESC;
	CModel::BONEMATRIX_PTR	mBoneMatrixPtr;
	_float4x4				mSocketTransformMatrix;

	_float4x4				mMatSocket;

public:
	static CGameObject_3D_Socket* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_3D_Socket* Clone(void* pArg);
	virtual void Free() override;
};

END

