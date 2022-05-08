#pragma once

#include "Component.h"

BEGIN(Engine)

// 네비게이션 컴포넌트
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

	// 움직임 연산
	_bool Move_OnNavigation(_fvector vPos);

public:
	HRESULT Save_NaviMeshData(wstring wpath);
	HRESULT Load_NaviMeshData(wstring wpath);


#ifdef _DEBUG
public:
	HRESULT Render(class CTransform* pTransform);
#endif // _DEBUG

private: // 셀 저장
	vector<class CCell*>			mVecCells;
	typedef vector<class CCell*>	CELLS;

#ifdef _DEBUG
private:
	// 기본 셰이더 제공
	BasicEffect*									mBaseEffect = nullptr;
	// 셰이더에 필요한 레이아웃
	ID3D11InputLayout*								mInputLayout = nullptr;

	// 기본적인 <정점버퍼> 제공
	PrimitiveBatch<DirectX::VertexPositionColor>*	mBatch = nullptr;
#endif // _DEBUG

private:
	// 현재 컴포넌트 객체의 셀 인덱스
	_uint				mCurrentIndex = 0;
	E_NAVI_OBJTYPE		meNaviType = NAVI_OBJTYPE_NONE;
private:
	// 포인트 리스트로 초기화
	HRESULT ReadyNaviMeshForListData(list<_float3*>& vpointlist);
	HRESULT SetUp_Neighbor();

	void ReadyDefault();
public:
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pNaviDataFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END