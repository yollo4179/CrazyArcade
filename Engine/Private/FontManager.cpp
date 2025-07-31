#include "FontManager.h"
#include "Font.h"

CFontManager::CFontManager(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
    : m_pDevice{ pDevice }
    , m_pContext{ pContext }
{
}

HRESULT CFontManager::AddFont(const _wstring& strFontTag, const _tchar* pFontFilePath)
{
    if (nullptr != Find_Font(strFontTag))
        return E_FAIL;

    CFont* pFont = CFont::Create(m_pDevice, m_pContext, pFontFilePath);
    if (nullptr == pFont)
        return E_FAIL;

    m_Fonts.emplace(strFontTag, pFont);

    return S_OK;
}

HRESULT CFontManager::RenderFont(const _wstring& strFontTag, const _tchar* pText, const _float2& vPosition, _fvector vColor, _float fRotation, const _float2& vOrigin, _float fScale)
{
    CFont* pFont = Find_Font(strFontTag).get();
    if (nullptr == pFont)
        return E_FAIL;


    return pFont->Render(pText, vPosition, vColor, fRotation, vOrigin, fScale);
}

shared_ptr<SpriteFont> CFontManager::GetSpriteFont(const _wstring& strFontTag)
{
    const auto& pSpriteFont = m_Fonts.find(strFontTag);
    if (pSpriteFont != m_Fonts.end())
    {
        return (*pSpriteFont).second.get()->Get_SpriteFont();
    }
    return nullptr;
}

shared_ptr<SpriteBatch> CFontManager::GetSpriteBatch(const _wstring& strFontTag)
{
    const auto& pSpriteFont = m_Fonts.find(strFontTag);
    if (pSpriteFont != m_Fonts.end())
    {
        return (*pSpriteFont).second.get()->Get_SpriteBatch();
    }
    return nullptr;
}

shared_ptr<CFont> CFontManager::Find_Font(const _wstring& strFontTag)
{
    auto	iter = m_Fonts.find(strFontTag);

    if (iter == m_Fonts.end())
        return nullptr;

    return iter->second;
}

CFontManager* CFontManager::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
    return new CFontManager(pDevice, pContext);
}
