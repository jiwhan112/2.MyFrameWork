#pragma once

#include "GameObject_Base.h"

BEGIN(Client)
// Ÿ�� ������Ʈ
class CGameObject_3D_Tiles  final:
	public CGameObject_Base
{
public:
	// ������Ʈ ���
	enum E_UPDATETYPE
	{
		// �⺻
		E_UPDATETYPE_NONE,

		// ���콺 ��ŷ�� 
		E_UPDATETYPE_PICK,
		E_UPDATETYPE_END
	};
	
	enum E_TILETYPE
	{
		TILETYPE_TOP,
		TILETYPE_ONEWALL,
		TILETYPE_TOWWALL,
		TILETYPE_END
	};


protected:
	explicit CGameObject_3D_Tiles(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject_3D_Tiles(const CGameObject_3D_Tiles& rhs);
	virtual ~CGameObject_3D_Tiles() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	
	virtual HRESULT Render();

	virtual HRESULT Set_Component() override;

public:
	void RenderOnOff(int index, bool b);



private:
	class CGameObject_3D_Static* mTileObject[TILETYPE_END] = {nullptr,};

public:
	static CGameObject_3D_Tiles* Create(ID3D11Device* d, ID3D11DeviceContext* cont);
	virtual CGameObject_3D_Tiles* Clone(void* pArg);
	virtual void Free() override;
};

END

