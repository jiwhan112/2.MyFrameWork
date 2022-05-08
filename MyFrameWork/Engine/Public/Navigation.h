#pragma once

#include "Component.h"

BEGIN(Engine)

// �׺���̼� ������Ʈ
class ENGINE_DLL CNavigation final : public CComponent
{
public:
	enum E_NAVI_OBJTYPE
	{
		NAVI_OBJTYPE_NONE,
		NAVI_OBJTYPE_PLAYER,
		NAVI_OBJTYPE_STATIC,
		NAVI_OBJTYPE_DYNAMIC,
		NAVI_OBJTYPE_END,
 };

private:
	CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CNavigation(const CNavigation& rhs);
	virtual ~CNavigation() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pNaviDataFilePath);
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	void Set_NaviObjType(E_NAVI_OBJTYPE t) { meNaviType = t; }
	bool Pick(const _float4x4& WorldMatrixInverse, _float3 * pOut);
	HRESULT SetUp_NewNaviMesh(list<_float3*>& vpointlist);
	HRESULT SetUp_AutoMesh(class CVIBuffer_Terrain* terrain);

	// ������ ����
	_bool Move_OnNavigation(_fvector vPos);

public:
	HRESULT Save_NaviMeshData(wstring wpath);
	HRESULT Load_NaviMeshData(wstring wpath);


#ifdef _DEBUG
public:
	HRESULT Render(class CTransform* pTransform);
#endif // _DEBUG

private: // �� ����
	vector<class CCell*>			mVecCells;
	typedef vector<class CCell*>	CELLS;

#ifdef _DEBUG
private:
	// �⺻ ���̴� ����
	BasicEffect*									mBaseEffect = nullptr;
	// ���̴��� �ʿ��� ���̾ƿ�
	ID3D11InputLayout*								mInputLayout = nullptr;

	// �⺻���� <��������> ����
	PrimitiveBatch<DirectX::VertexPositionColor>*	mBatch = nullptr;
#endif // _DEBUG

private:
	// ���� ������Ʈ ��ü�� �� �ε���
	_uint				mCurrentIndex = 0;
	E_NAVI_OBJTYPE		meNaviType = NAVI_OBJTYPE_NONE;
private:
	// ����Ʈ ����Ʈ�� �ʱ�ȭ
	HRESULT ReadyNaviMeshForListData(list<_float3*>& vpointlist);
	HRESULT SetUp_Neighbor();

	void ReadyDefault();
public:
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pNaviDataFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END