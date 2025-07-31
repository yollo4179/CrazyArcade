#include"pch.h"
#include "LevelLogin.h"
#include"GameInstance.h"
#include"LevelLoading.h"
#include"SessionManager.h"
#include"LoginIDBox.h"
#include"LoginButton.h"
CLevelLogin::CLevelLogin(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	:CLevel{pDevice, pContext}
	,m_isLogined{false}

{
}

CLevelLogin::~CLevelLogin()
{
}

HRESULT CLevelLogin::Initialize()
{
	return S_OK;
}


void CLevelLogin::Update(float fTimeDelta)
{
	//LevelManager에서 돌릴것
	
	/*로그인 아이디 입력하고 엔터누ㄱ르거나 버튼 클릭하면 */
	/*ID를 세션 매니저 (싱글톤에 저장하고 다음 레벨로 넘어가자)*/
	_wstring m_NowMassage;


	if ( (KEY_PRESSED(KEY::ENTER)|| CLoginButton::GetClicked() )&& CLoginIDBox::GetTextState()->FullText.size())
	{
		SessionManager::Get().AccountID = CLoginIDBox::GetTextState()->FullText;
		m_isLogined = true;
	}
	


	/*Todo*/

	/*End*/
	if (IsLogined())
	{
		m_pGameInstance.lock()->
			OpenLevel(
				(_uint)LEVEL_ID::LEVEL_LOADING,
				shared_ptr<CLevelLoading>(CLevelLoading::Create(m_pDevice,m_pContext,LEVEL_ID::LEVEL_ROOM))
			);
	}
}

CLevelLogin* CLevelLogin::Create(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext>pContext)
{
	CLevelLogin* pInstance = new CLevelLogin(pDevice, pContext);
	if (pInstance-> Initialize())
	{
		MSG_BOX("레벨 로그인 생성실패");
	}
	return pInstance;
}
