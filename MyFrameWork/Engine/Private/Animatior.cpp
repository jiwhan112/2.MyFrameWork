#include "Animatior.h"


CAnimatior::CAnimatior()
{
}



HRESULT CAnimatior::NativeConstruct()
{
	// �ִϸ��̼� �ʱ�ȭ

	return S_OK;
}

CAnimatior * CAnimatior::Create()
{
	CAnimatior*	pInstance = new CAnimatior();

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Created CAnimatior");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CAnimatior::Free()
{
	
}