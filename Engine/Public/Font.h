#pragma once
#include"Base.h"

BEGIN(Engine)

class CFont final : public CBase
{
private:
	CFont(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	public:
	virtual ~CFont() = default;

public:
	HRESULT Initialize(const _tchar* pFontFilePath);
	HRESULT Render(const _tchar* pText, const _float2& vPosition, _fvector vColor, _float fRotation = 0.f, const _float2& vOrigin = _float2(0.f, 0.f), _float fScale = 1.f);
	shared_ptr<SpriteFont> Get_SpriteFont() { return m_pFont; }
	shared_ptr<SpriteBatch> Get_SpriteBatch() { return m_pBatch; }
private:
	ComPtr<ID3D11Device> m_pDevice = { nullptr };
	ComPtr<ID3D11DeviceContext> m_pContext = { nullptr };
	shared_ptr<SpriteFont> m_pFont = { nullptr };
	shared_ptr<SpriteBatch> m_pBatch = { nullptr };

public:

	static CFont* Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, const _tchar* pFontFilePath);

};

END