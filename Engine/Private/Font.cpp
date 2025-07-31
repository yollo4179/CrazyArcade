#include "Font.h"

CFont::CFont(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
{
}

HRESULT CFont::Initialize(const _tchar* pFontFilePath)
{
	m_pFont.reset( new SpriteFont(m_pDevice.Get(), pFontFilePath));

	m_pBatch.reset( new SpriteBatch(m_pContext.Get()));

	return S_OK;
}

HRESULT CFont::Render(const _tchar* pText, const _float2& vPosition, _fvector vColor, _float fRotation, const _float2& vOrigin, _float fScale)
{
	m_pContext->GSSetShader(nullptr, nullptr, 0);

	m_pBatch->Begin();

	m_pFont->DrawString(m_pBatch.get(), pText, vPosition, vColor, fRotation, vOrigin, fScale);

	m_pBatch->End();

	return S_OK;
}

CFont* CFont::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, const _tchar* pFontFilePath)
{
	CFont* pInstance = new CFont(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pFontFilePath)))
	{
		MSG_BOX("Failed to Created : CCustomFont");
		Safe_Delete(pInstance);
	}

	return pInstance;
}
