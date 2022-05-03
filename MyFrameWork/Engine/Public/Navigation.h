#pragma once

#include "Component.h"

BEGIN(Engine)

// �׺���̼� ������Ʈ
class ENGINE_DLL CNavigation final : public CComponent
{
private:
	CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CNavigation(const CNavigation& rhs);
	virtual ~CNavigation() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pNaviDataFilePath);
	virtual HRESULT NativeConstruct(void* pArg) override;

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
	// ���� ��ü�� �� �ε���
	_uint				m_iCurrentIndex = 0;

private:
	HRESULT SetUp_Neighbor();
	void ReadyDefault();
public:
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pNaviDataFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END