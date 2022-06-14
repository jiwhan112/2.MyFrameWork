#pragma once

#include "Component.h"
#include "Cell.h"

BEGIN(Engine)

// 네비게이션 컴포넌트
class ENGINE_DLL CNavigation final : public CComponent
{
public:
	const wstring NAVI_FILENAME16 = L"navi16.dat";
	const wstring NAVI_FILENAME32 = L"navi32.dat";
	const wstring NAVI_FILENAME64 = L"navi64.dat";

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

	_bool  Get_PickPosForIndex(_float3 PickPos,_uint* outIndex);

	void Pick_ChangeCellOption(const _float4x4& WorldMatrixInverse, CCell::E_CELLTYPE type);
	
	_uint	Get_CurrentCellIndex()const { return mCurrentIndex; }
	CCell*	Get_CurrentCell() const { return Get_TileForIndex(mCurrentIndex); }

	const vector<CCell*>* Get_CellVector() const
	{
		return &mVecCells;
	}
	
	HRESULT SetUp_NewNaviMesh(list<_float3*>& vpointlist);
	HRESULT SetUp_AutoMesh(class CVIBuffer_Terrain* terrain);

	// 움직임 연산
	_bool Move_OnNavigation(_fvector vPos);
	_float3 Move_OnNavigation_Able(_float3 vPosition);


public:
	HRESULT Save_NaviMeshData(wstring wpath);
	HRESULT Load_NaviMeshData(wstring wpath);
	HRESULT Remove_NaviMeshData();
	HRESULT SetUp_CurrentPoint(class CVIBuffer_Terrain* terrain);// 높이맵에 따른 포인트 재정의


	const list<CCell*>& AstartPathFind(_uint StartTileIndex, _uint GoalTileIndex);
	

#ifdef _DEBUG
public:
	HRESULT Render(class CTransform* pTransform);
private:
	// 기본 셰이더 제공
	BasicEffect*									mBaseEffect = nullptr;
	// 셰이더에 필요한 레이아웃
	ID3D11InputLayout*								mInputLayout = nullptr;

	// 기본적인 <정점버퍼> 제공
	PrimitiveBatch<DirectX::VertexPositionColor>*	mBatch = nullptr;
#endif // _DEBUG

private: // 길찾기
	//bool MakeRoute(_uint StartTileIndex, _uint GoalTileIndex,
	//	CCell* oriStartCell, CCell* oriGoalCell);
	bool MakeRoute_INDEX(_uint StartTileIndex, _uint GoalTileIndex,
		CCell* oriStartCell);
	//bool MakeRoute_LINE(_uint StartTileIndex, _uint GoalTileIndex,
	//	CCell* oriStartCell, CCell* oriGoalCell);

	CCell*	Get_TileForIndex(_uint index) const;
	bool	Check_Close(_uint index);
	bool	Check_Open(_uint index);


private:
	// 네비 메시 제작하기 위함 
	HRESULT ReadyNaviMeshForListData(list<_float3*>& vpointlist);
	HRESULT SetUp_Neighbor();

private: // 셀 저장
	vector<CCell*>			mVecCells;
	typedef vector<CCell*>	CELLS;

private: // 네비메시 길찾기
	list<_uint>			mListClose;
	list<_uint>			mListOpen;
	list<CCell*>		mListPathList;

private:
	// 현재 컴포넌트 객체의 셀 인덱스
	_uint				mCurrentIndex = 0;
	E_NAVI_OBJTYPE		meNaviType = NAVI_OBJTYPE_NONE;



//	void ReadyDefault();
public:
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pNaviDataFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END