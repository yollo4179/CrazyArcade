#pragma once
#include"Component.h"
BEGIN(Engine)
class  ENGINE_DLL CShader final : public CComponent
{
private:
	CShader(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CShader(const CShader& Prototype);
public:
	virtual ~CShader() ;

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements);
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Begin(_uint iPassIndex);
	HRESULT Begin_NoInput(_uint iPassIndex);
	HRESULT Bind_RawValue(const _char* pConstantName, const void* pData, _uint iLength);
	HRESULT Bind_Matrix(const _char* pConstantName, const _float4x4* pMatrix);
	HRESULT Bind_Matrices(const _char* pConstantName, const _float4x4* pMatrix, _uint iNumMatrices);
	HRESULT Bind_SRV(const _char* pConstantName, ID3D11ShaderResourceView* pSRV);
	HRESULT Bind_SRVs(const _char* pConstantName, ID3D11ShaderResourceView** ppSRV, _uint iNumSRVs);

#ifdef _DEBUG
	virtual void Inspector()override {};
#endif

private:
	ComPtr<ID3DX11Effect> m_pEffect = {};
	_uint						m_iNumLayouts = { 0 };
	vector<ComPtr<ID3D11InputLayout>>	m_InputLayouts;


public:
	static shared_ptr<CShader> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements);
	virtual shared_ptr<CComponent> Clone(void* pArg) override;

};
END
