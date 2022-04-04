#include "Tool/Imgui_Base.h"

BEGIN(Client)

// IMGUI ����� �͸� ������Ʈ

class CImgui_UI final:
	public CImgui_Base
{

private:
	explicit CImgui_UI(ID3D11Device* device, ID3D11DeviceContext* context);
	explicit CImgui_UI(const CImgui_UI& rhs);
	virtual ~CImgui_UI() = default;
	
public:
	// CImgui_Base��(��) ���� ��ӵ�
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT Update(_double time) override;
public:
	void DemoTest();
	void UISaver();
	void TextureSaver();


private:
	bool bDemo;

	float f;
public:
	static CImgui_UI* Create(ID3D11Device* deviec, ID3D11DeviceContext* context);
	virtual void Free() override;





};

END