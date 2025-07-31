#include"pch.h"
#include "LevelLoby.h"

CLevelLoby::CLevelLoby(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
    :CLevel{pDevice, pContext}
{
}

CLevelLoby::~CLevelLoby()
{
}

HRESULT CLevelLoby::Initialize()
{
    return S_OK;
}

void CLevelLoby::Update(float fTimeDelta)
{
}

CLevelLoby* CLevelLoby::Create(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext>pContext)
{
    CLevelLoby* pInstanace = new CLevelLoby(pDevice, pContext);
    if (pInstanace->Initialize())
    {
        MSG_BOX("레벨 로비 생성 실패");
    }
    return pInstanace;

    return nullptr;
}
