#include "Tool/Imgui_Base.h"

BEGIN(Client)


// Model��
// ���� ���� �� FBX �̸����� Dat ������ �����.
// �浹 �𵨼���
class CImgui_Model final:
	public CImgui_Base
{

private:
	explicit CImgui_Model(ID3D11Device* device, ID3D11DeviceContext* context);
	explicit CImgui_Model(const CImgui_Model& rhs);
	virtual ~CImgui_Model() = default;

public:
	// CImgui_Base��(��) ���� ��ӵ�
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT Update(_double time) override;

public:
	HRESULT Render_UI();

private:
	void FBX_SETTINGMODE();
	void FBX_CREATEMODE();


private:
	HRESULT Edit_FBX();
	HRESULT Edit_Texture();

private:
	class CCamera_Client*			mCameraClient = nullptr;
	class CGameObject_3D_Static*	mCurrentModelObject = nullptr;

	list<string>*					mFBXpathList = nullptr;
	list<string>*					mProtoModelList = nullptr;


private:
	bool							mIsModelSetting = false;


public:
	static CImgui_Model* Create(ID3D11Device* deviec, ID3D11DeviceContext* context);
	virtual void Free() override;
};

END
