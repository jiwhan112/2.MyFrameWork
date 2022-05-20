#pragma once

#include "GameObject_3D_Dynamic.h"

BEGIN(Engine)
class CModel;
END

// �ִϸ��̼� 3D ������Ʈ ���
BEGIN(Client)

class CGameObject_Mine final :
	public CGameObject_3D_Dynamic
{
public:
	// ���� AI ����

	//enum E_BASEAI
	//{
	//	BASEAI_IDLE,
	//	BASEAI_MOVE,
	//	BASEAI_END,
	//};

protected:
	explicit CGameObject_Mine(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject_Mine(const CGameObject_Mine& rhs);
	virtual ~CGameObject_Mine() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	// virtual _int Tick(_double TimeDelta);
	// virtual _int LateTick(_double TimeDelta);
	// virtual HRESULT Render();

	// �� ���� 



protected:

public:
	static CGameObject_Mine* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_Mine* Clone(void* pArg);
	virtual void Free() override;
};

END