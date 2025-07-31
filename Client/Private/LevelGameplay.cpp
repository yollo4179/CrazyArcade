#include"pch.h"
#include "LevelGameplay.h"
#include"GameInstance.h"
#include"LevelRoom.h"
CLevelGameplay::CLevelGameplay(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
    :CLevel{pDevice, pContext}
{
}

CLevelGameplay::~CLevelGameplay()
{

}

HRESULT CLevelGameplay::Initialize()
{
    return S_OK;
}

void CLevelGameplay::Update(_float fTimeDelta)
{
    /* 전투 로직 -> 데이터 수신 후 적용  */

    if (m_GameEnd)
    {
      /*  
        m_pGameInstance.lock()->OpenLevel(
            (_uint)LEVEL_ID::LEVEL_ROOM,
            Shared_ptr<CLevelRoom>()
            )*/

    }
}

CLevelGameplay* CLevelGameplay::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
    CLevelGameplay* pInstance = new CLevelGameplay(pDevice, pContext);
    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("게임 플레이 레벨 생성 실패 ");
        Safe_Delete(pInstance);
    }
    return pInstance;
}
