#include "Tool/Imgui_Base.h"

BEGIN(Client)

// IMGUI UI ��

class CIMGUI_Terrain final:
	public CImgui_Base
{

private:
	explicit CIMGUI_Terrain(ID3D11Device* device, ID3D11DeviceContext* context);
	explicit CIMGUI_Terrain(const CIMGUI_Terrain& rhs);
	virtual ~CIMGUI_Terrain() = default;
	
public:
	// CImgui_Base��(��) ���� ��ӵ�
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT Update(_double time) override;

public:	
	HRESULT Render_UI();
	
private:


	CGameObject*	mCurrentUIObject = nullptr;



	// ������Ʈ ������
	class CObjectIO*		mObjectSaverClass = nullptr;


public:
	static CIMGUI_Terrain* Create(ID3D11Device* deviec, ID3D11DeviceContext* context);
	virtual void Free() override;


};

END
