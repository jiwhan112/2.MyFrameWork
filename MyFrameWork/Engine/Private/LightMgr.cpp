#include "..\Public\LightMgr.h"
#include "..\Public\Light.h"

IMPLEMENT_SINGLETON(CLightMgr)

const LIGHTDESC* CLightMgr::Get_LightDesc(_uint iIndex) const
{
	if (iIndex >= mLights.size())
		return nullptr;

	auto	iter = mLights.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->GetLightDesc();
}

HRESULT CLightMgr::Add_Light(ID3D11Device * device, ID3D11DeviceContext * context, const LIGHTDESC & desc)
{
	CLight* light = CLight::Create(device, context, desc);
	if (light == nullptr)
		return E_FAIL;

	mLights.push_back(light);
	return S_OK;
}

void CLightMgr::Free()
{
	for (auto light : mLights)
	{
		Safe_Release(light);
	}
}