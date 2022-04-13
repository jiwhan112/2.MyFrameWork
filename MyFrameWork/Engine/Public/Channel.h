#pragma once

#include "Component.h"

BEGIN(Engine)
class CHierarchyNode;

// Ư�� �ִϸ��̼��� ���� Ű������ ������ �����Ѵ�.
class CChannel final : public CBase
{
public:
	CChannel();
	virtual ~CChannel() = default;

public:
	HRESULT NativeConstruct(const char* pName, CHierarchyNode* pNode);

public:
	const	vector<KEYFRAME*>* Get_VecKeyFrame() const { return &mVectorKeyFrame; }
	_uint	Get_CurrentKeyFrame()  const { return mCurrentKeyFrame; }
	void	Set_CurrentKeyFrame(_uint value)
	{
		mCurrentKeyFrame = value;
	}

	void	Set_TransformationMat(_fmatrix transform);

public: // ���� �������̽�
	
	void Reserve(_uint iNumKeyFrames)
	{
		mVectorKeyFrame.reserve(iNumKeyFrames);
		mNumKeyFrames = iNumKeyFrames;
	}
	void Add_KeyFrame(KEYFRAME* pKeyFrame)
	{
		mVectorKeyFrame.push_back(pKeyFrame);
	}

private:
	char						mSzName[MAX_PATH] = "";
	_uint						mCurrentKeyFrame = 0;
	_uint						mNumKeyFrames = 0;
	vector<KEYFRAME*>			mVectorKeyFrame;
	typedef vector<KEYFRAME*>	KEYRANES;

	CHierarchyNode*				mHierNode = nullptr;

public:
	static CChannel* Create(const char* pName, class CHierarchyNode* pNode);
	virtual void Free() override;
};

END