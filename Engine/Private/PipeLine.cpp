#include "PipeLine.h"

HRESULT CPipeLine::Initialize(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
    m_pContext = pContext; 
    m_pDevice = pDevice; 
    _uint ViewPortNum = {1};
    D3D11_VIEWPORT VIEWPORT_DESC = {};
    m_pContext->RSGetViewports(&ViewPortNum, &VIEWPORT_DESC);
  

    /*직교 투영 for all objects*/
    XMStoreFloat4x4(&m_Matrices[(_uint)PIPELINE_MATRICES::D3DTS_PROJ] , XMMatrixOrthographicLH(VIEWPORT_DESC.Width, VIEWPORT_DESC.Height, VIEWPORT_DESC.MinDepth, VIEWPORT_DESC.MaxDepth));
    XMStoreFloat4x4(&m_Matrices[(_uint)PIPELINE_MATRICES:: D3DTS_VIEW ] , XMMatrixIdentity());

    return S_OK;
}
void CPipeLine::Update()
{
    for (int i = 0; i < (_uint)PIPELINE_MATRICES::D3DTS_END; ++i)
    {
        XMStoreFloat4x4(
            (_float4x4*)(&m_InverseMatrices[i]),
            XMMatrixInverse(nullptr, XMLoadFloat4x4((_float4x4*)(&m_Matrices[i]))));
    }
}
      

CPipeLine* CPipeLine::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{

    CPipeLine* pInstance = new CPipeLine();
    if (FAILED(pInstance->Initialize(pDevice,pContext)))
    {
        MSG_BOX("Failed to Create PipeLine");
        Safe_Delete(pInstance);
        return nullptr;
    }
    return pInstance;
 
}


