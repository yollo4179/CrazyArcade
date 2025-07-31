#pragma once

#include "Base.h"
BEGIN(Engine)
class CFontManager final : public CBase
{
private:
	CFontManager(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
public:
	virtual ~CFontManager() = default;

public:
	HRESULT AddFont(const _wstring& strFontTag, const _tchar* pFontFilePath);
	HRESULT RenderFont(const _wstring& strFontTag, const _tchar* pText, const _float2& vPosition, _fvector vColor, _float fRotation = 0.f, const _float2& vOrigin = _float2(0.f, 0.f), _float fScale = 1.f);
	shared_ptr <SpriteFont> GetSpriteFont(const _wstring& strFontTag);
	shared_ptr <SpriteBatch> GetSpriteBatch(const _wstring& strFontTag);
private:
	ComPtr<ID3D11Device> m_pDevice = { nullptr };
	ComPtr<ID3D11DeviceContext> m_pContext = { nullptr };


	map<const _wstring, shared_ptr<class CFont>>			m_Fonts;

private:
	shared_ptr<class CFont> Find_Font(const _wstring& strFontTag);

public:
	static CFontManager* Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);


};
END
