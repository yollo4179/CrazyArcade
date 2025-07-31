#ifdef _DEBUG

#pragma once
#include "Base.h"
#include "Engine_Defines.h"

BEGIN(Engine)
class CImguiManager : public CBase
{
private:
	CImguiManager(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
public:
	virtual ~CImguiManager();

public:
	HRESULT		Initialize(HWND _hWnd);

public:
	void Update();
	void Render();

private:
	ComPtr<ID3D11Device> m_pDevice = { nullptr };
	ComPtr<ID3D11DeviceContext> m_pContext = { nullptr };

public:
	static CImguiManager* Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, HWND _hWnd);

};

END
#endif 