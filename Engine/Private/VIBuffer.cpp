#include "VIBuffer.h"

CVIBuffer::CVIBuffer(ComPtr<ID3D11Device> pDevice,
    ComPtr<ID3D11DeviceContext> pContext)
    : CComponent{ pDevice, pContext },
	m_pVB{ nullptr },
	m_pIB{ nullptr },
	m_iVertexStride{ 0 },
	m_iNumVertices{ 0 },
	m_iNumVertexBuffers{ 0 },
	m_iIndexStride{ 0 },
	m_iNumIndices{ 0 },
	m_eIndexFormat{ DXGI_FORMAT_UNKNOWN },
	m_ePrimitiveTopology{ D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST }
{
}

CVIBuffer::CVIBuffer(const CVIBuffer& Prototype):
	CComponent{ Prototype },
	m_pVB{ Prototype.m_pVB },
	m_pIB{ Prototype.m_pIB },
	m_iVertexStride{ Prototype.m_iVertexStride },
	m_iNumVertices{ Prototype.m_iNumVertices },
	m_iNumVertexBuffers{ Prototype.m_iNumVertexBuffers },
	m_iIndexStride{ Prototype.m_iIndexStride },
	m_iNumIndices{ Prototype.m_iNumIndices },
	m_eIndexFormat{ Prototype.m_eIndexFormat },
	m_ePrimitiveTopology{ Prototype.m_ePrimitiveTopology }
{
}

HRESULT CVIBuffer::Initialize_Prototype(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
    return S_OK;
}

HRESULT CVIBuffer::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT CVIBuffer::Render()
{
	m_pContext->DrawIndexed(m_iNumIndices, 0, 0);
    return S_OK;
}

HRESULT CVIBuffer::BindBufferDesc()
{

	ID3D11Buffer* pVertexBuffers[] = {
		m_pVB.Get(),
	};

	_uint					iVertexStrides[] = {
		m_iVertexStride,

	};

	_uint					iOffsets[] = {
		0,
	};
	/*In PutAssembler*/
	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iVertexStrides, iOffsets);
	m_pContext->IASetIndexBuffer(m_pIB.Get(), m_eIndexFormat, 0);
	m_pContext->IASetPrimitiveTopology(m_ePrimitiveTopology);



    return S_OK;
}

HRESULT CVIBuffer::Create_Buffer(ID3D11Buffer** ppOut)
{
	m_pDevice->CreateBuffer(&m_BufferDesc, &m_SubResourceDesc, ppOut);
	return S_OK;

}
