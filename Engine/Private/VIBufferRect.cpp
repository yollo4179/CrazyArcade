#include "VIBufferRect.h"

CVIBufferRect::CVIBufferRect(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
    :CVIBuffer{pDevice, pContext}
{
}

CVIBufferRect::CVIBufferRect(const CVIBufferRect& Prototype)
    :CVIBuffer{Prototype}
{
}

HRESULT CVIBufferRect::Initialize_Prototype()
{

    m_iNumVertexBuffers = 1;

    m_iVertexStride = sizeof(VTXPOSTEX);
    m_iNumVertices = 4; 

    m_iIndexStride = sizeof(_ushort);  //0~6

    m_iNumIndices = 6;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
    m_ePrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;


#pragma region VERTEX_BUFFER
		ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	/* 정점버퍼를 몇 바이트 할당할까요? */
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.StructureByteStride = m_iVertexStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXPOSTEX* pVertices = new VTXPOSTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXPOSTEX) * m_iNumVertices);

	pVertices[0].vPosition = XMFLOAT3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexcoord = XMFLOAT2(0.f, 0.f);

	pVertices[1].vPosition = XMFLOAT3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexcoord = XMFLOAT2(1.f, 0.f);

	pVertices[2].vPosition = XMFLOAT3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexcoord = XMFLOAT2(1.f, 1.f);

	pVertices[3].vPosition = XMFLOAT3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexcoord = XMFLOAT2(0.f, 1.f);

	ZeroMemory(&m_SubResourceDesc, sizeof m_SubResourceDesc);
	m_SubResourceDesc.pSysMem = pVertices;


	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

#pragma endregion

#pragma region INDEX_BUFFER
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	/* 정점버퍼를 몇 바이트 할당할까요? */
	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.StructureByteStride = /*m_iInstanceStride*/0;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	_ushort* pIndices = new _ushort[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndices);

	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;

	pIndices[3] = 0;
	pIndices[4] = 2;
	pIndices[5] = 3;

	ZeroMemory(&m_SubResourceDesc, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceDesc.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion

	return S_OK;

}

HRESULT CVIBufferRect::Initialize(void* pArg)
{
    return S_OK;
}



shared_ptr<CVIBufferRect> CVIBufferRect::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
    shared_ptr<CVIBufferRect> pInstance ( new CVIBufferRect(pDevice, pContext));
    if (pInstance->Initialize_Prototype())
    {
        MSG_BOX("Failed To CVIBufferRect::Clone");
       
        return nullptr;
    }
    return pInstance;
}

shared_ptr<CComponent> CVIBufferRect::Clone(void* pArg)
{
    shared_ptr<CVIBufferRect> pInstance (new CVIBufferRect(*this));
    if(pInstance->Initialize(pArg))
    {
		MSG_BOX("Failed To CVIBufferRect::Clone");
		
		return nullptr;
	}
    return pInstance;
}
