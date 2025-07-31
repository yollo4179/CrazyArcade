#pragma once
#include"Component.h"
BEGIN(Engine)
class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	CVIBuffer(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CVIBuffer(const CVIBuffer& Prototype);;

public:
	virtual ~CVIBuffer() = default;
public:
	virtual HRESULT Initialize_Prototype(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	virtual HRESULT	Initialize(void* pArg);
	HRESULT Render(); 


public:
	virtual HRESULT BindBufferDesc(); 


protected:
	ComPtr<ID3D11Buffer> m_pVB = { nullptr };
	ComPtr<ID3D11Buffer> m_pIB = { nullptr };

protected:
	D3D11_BUFFER_DESC					m_BufferDesc = {};
	D3D11_SUBRESOURCE_DATA				m_SubResourceDesc = {};
protected:
	_uint								m_iVertexStride = {};
	_uint								m_iNumVertices = {};
	_uint								m_iNumVertexBuffers = {};

	_uint								m_iIndexStride = {};
	_uint								m_iNumIndices = {};
	DXGI_FORMAT							m_eIndexFormat = {};
	D3D11_PRIMITIVE_TOPOLOGY			m_ePrimitiveTopology = {};
	
#ifdef _DEBUG
	virtual void Inspector()override=0;
#endif
protected:
	HRESULT Create_Buffer(ID3D11Buffer** ppOut);

public:
	virtual shared_ptr<CComponent> Clone(void* pArg) = 0;
	
};
END
