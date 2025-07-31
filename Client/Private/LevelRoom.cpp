#include"pch.h"
#include"LevelRoom.h"
#include"ChattingSocketManager.h"
CLevelRoom::CLevelRoom(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	:CLevel{pDevice, pContext}
{
}

CLevelRoom::~CLevelRoom()
{
}

HRESULT CLevelRoom::Initialize()
{
	ChattingSocketManager::Get().Initialize(_wstring(L"0.tcp.jp.ngrok.io"), _wstring(L"12051"));
	return S_OK;
}

void CLevelRoom::Update(float fTimeDelta)
{
}

CLevelRoom* CLevelRoom::Create(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext>pContext)
{
	CLevelRoom* pInstance = new CLevelRoom(pDevice, pContext);
	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Create LevelRoom");
			Safe_Delete(pInstance);
	}
	return pInstance;
}
