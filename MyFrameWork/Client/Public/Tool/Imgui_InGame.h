#include "Tool/Imgui_Base.h"

BEGIN(Client)

// Terrain��

class CImgui_InGame final :
	public CImgui_Base
{
public:


private:
	explicit CImgui_InGame(ID3D11Device* device, ID3D11DeviceContext* context);
	explicit CImgui_InGame(const CImgui_InGame& rhs);
	virtual ~CImgui_InGame() = default;

public:
	// CImgui_Base��(��) ���� ��ӵ�
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT Update(_double time) override;

public:
	HRESULT Render_UI();

private:

private:
	HRESULT Edit_TERRAIN(); // TARRAIN

//	HRESULT Edit_Texture();

private:
	// ī�޶�
	class CCamera_Client*			mCameraClient = nullptr;

	// ���� ������Ʈ
	class CGameObject_MyTerrain*	mTerrainObject = nullptr;



public:
	static CImgui_InGame* Create(ID3D11Device* deviec, ID3D11DeviceContext* context);
	virtual void Free() override;
};

END
