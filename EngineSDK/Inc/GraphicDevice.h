#pragma once

#include"Base.h"
#include"Engine_Defines.h"
BEGIN(Engine)
class CGraphicDevice final :public CBase
{

private: 
	CGraphicDevice();
public:
	virtual ~CGraphicDevice();
	

public:
	HRESULT Initialize(HWND hWnd, _bool isWindowed, _uint iWinSizeX, _uint iWinSizeY,
		_Inout_  ComPtr<ID3D11Device>* ppDevice, _Inout_ ComPtr<ID3D11DeviceContext>* ppContext);
	
	/*Step 1. 백버퍼를 지운다.*/
	HRESULT STEP1_1_ClearBackBufferView(_float4 vClearColor);
	/*Step 1. 깊이 스텐실 버퍼를 지운다*/
	HRESULT STEP1_2_ClearDepthStencilView();
	/*Step 2. 그림을 백버퍼에 그렸으면 백버퍼를 전면 버퍼와 교체하여 전면버포로 하여 화면에 보여준다. */
	HRESULT STEP2_Present(); 
	

private:
	HRESULT Ready_SwapChain(HWND hWnd, _bool isWindowed, _uint iWinCX, _uint iWinCY);
	HRESULT Ready_BackBufferRenderTargetView();
	HRESULT Ready_DepthStencilView(_uint iWinCX, _uint iWinCY);

	
	
#pragma region Texture
	ComPtr<ID3D11Texture2D> m_pDepthTexture = {}; //전체 화면 텍스처 
	ComPtr<ID3D11RenderTargetView> m_pBackBufferRTV = {}; //텍스처에 접근하여 렌더타겟에 그리는  용도 
	ComPtr<ID3D11DepthStencilView> m_pDSV = {};// 텍스처를 만들려면 항상 요구된다 . 
#pragma endregion Texture
	ComPtr<IDXGISwapChain> m_pSwapChain = {};
	ComPtr<ID3D11Device> m_pDevice = {};
	ComPtr<ID3D11DeviceContext> m_pDC = {};


public:
	/*얘는 그냥 포인터로 하고 매니저에서 쉐어드 포인터 생성해서 관리하자 .*/
	static CGraphicDevice* Create(HWND hWnd, _bool isWindowed, _uint WinSizeX, _uint WinSizeY,
		_Inout_ ComPtr<ID3D11Device>* ppDevice, _Inout_ ComPtr<ID3D11DeviceContext>* ppContext);
};
END
