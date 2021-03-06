#pragma once

#include "GameObject/GameObject_MyTerrain.h"

BEGIN(Client)

class CAstar final :
	public CBase
{
private:
	explicit CAstar();
	explicit CAstar(const CAstar& rhs);
	virtual ~CAstar() = default;

public:
	HRESULT NativeConstruct_Prototype();

//	const list<CCell*>&		GetBestList() { return mListBest; }

public:
	void		Start_AStar(const _float3& vStart, const _float3& vGoal);

private:
	bool		MakeRoute(int iStartIndex, int iGoalIndex);
	void		MakeBestList(int iGoalIndex);

	//int		GetTileIndex(const D3DXVECTOR3& vPos);
	//bool		Picking(const D3DXVECTOR3& vPos, int iIndex);
	//bool		CheckClose(int iIndex);
	//bool		CheckOpen(int iIndex);

private:
	list<_uint>										mListOpen;
	list<_uint>										mListClose;

//	list<CCell*>									mListBest;

	int												miStartIndex;

public:
	static CAstar* Create();
	virtual void Free() override;
};
END
