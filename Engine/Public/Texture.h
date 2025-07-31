#pragma once

#include "Component.h"
BEGIN(Engine)
class ENGINE_DLL CTexture final : public CComponent
{
private:
	CTexture(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext );
	CTexture(const CTexture& Prototype);
public:
	virtual ~CTexture() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pTextureFilePath, _uint iNumTexture);
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Bind_ShaderResource(class CShader* pShader, const _char* pConstantName, _uint iSRVIndex = 0);
	HRESULT Bind_ShaderResources(class CShader* pShader, const _char* pConstantName);

public:
	/*For MapTool*/
	const vector<ComPtr<ID3D11ShaderResourceView>>& GetSRVs() { return m_SRVs; };
#ifdef _DEBUG
	virtual void Inspector()override;
#endif
	inline void SetName(_wstring ProtoTag) { m_ProtoTag = ProtoTag; };
	_wstring GetName() { return m_ProtoTag; };
private:
	_uint										m_iNumSRVs = { 0 };
	/* 화면에 그려내기 위한 텍스쳐 타입. */
	vector<ComPtr<ID3D11ShaderResourceView>>			m_SRVs;
	/*ForMaptool */
	_wstring m_ProtoTag; 

public:
	static shared_ptr<CTexture> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, const _tchar* pTextureFilePath, _uint iNumTextures = 1);
	virtual shared_ptr<CComponent> Clone(void* pArg);
};
END

