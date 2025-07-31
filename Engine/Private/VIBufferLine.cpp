#include "VIBufferLine.h"

CDebugLine::CDebugLine(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
    :CComponent{pDevice, pContext}
{
}

CDebugLine::CDebugLine(const CDebugLine& Prototype)
    :CComponent{Prototype},
 m_BasicEffect{ Prototype.m_BasicEffect},
m_PrimitiveBatch{ Prototype.m_PrimitiveBatch},
m_InputLayout{ Prototype.m_InputLayout}
{
}

HRESULT CDebugLine::Initialize_Prototype()
{

    m_PrimitiveBatch = make_shared<PrimitiveBatch<DirectX::VertexPositionColor>>(m_pContext.Get());

    m_BasicEffect = make_shared<DirectX::BasicEffect>(m_pDevice.Get());
    m_BasicEffect->SetVertexColorEnabled(true);
    _uint NumViewPorts = 1;
    D3D11_VIEWPORT ViewPortDesc = {};
    m_pContext->RSGetViewports(&NumViewPorts,&ViewPortDesc);

    m_BasicEffect->SetProjection(DirectX::XMMatrixOrthographicLH(
        ViewPortDesc.Width, ViewPortDesc.Height, 0.0f, 1.0f)); // 2D 좌표계


    // 인풋 레이아웃 생성
    void const* shaderByteCode;
    size_t byteCodeLength;
    m_BasicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
    m_pDevice->CreateInputLayout(VertexPositionColor::InputElements,
        VertexPositionColor::InputElementCount,
        shaderByteCode, byteCodeLength,
        m_InputLayout.GetAddressOf());

    return S_OK;
}

HRESULT CDebugLine::Initialize(void* pArg)
{
    return S_OK;
}

shared_ptr<CComponent> CDebugLine::Clone(void* pArg)
{
    shared_ptr<CDebugLine> pInstance(new CDebugLine(*this));
    if (pInstance->Initialize(pArg))
    {
        MSG_BOX("Failed To Make DebugLine");
      
    }
    return pInstance;
}

shared_ptr<CDebugLine> CDebugLine::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{

    shared_ptr<CDebugLine> pInstance(new CDebugLine(pDevice,pContext));
    if (pInstance->Initialize_Prototype())
    {
        MSG_BOX("Failed To Make DebugLine");
    
    }
    return pInstance;
}

HRESULT CDebugLine::DrawBox(_float X, _float Y, _float W, _float H, _float4 color)
{

    m_BasicEffect->SetWorld(XMMatrixIdentity());
    m_BasicEffect->SetView(XMMatrixIdentity());
    _uint NumViewPorts = 1;
    D3D11_VIEWPORT ViewPortDesc = {};
    m_pContext->RSGetViewports(&NumViewPorts, &ViewPortDesc);

    m_BasicEffect->SetProjection(DirectX::XMMatrixOrthographicLH(
        ViewPortDesc.Width, ViewPortDesc.Height, 0.0f, 1.0f)); // 2D 좌표계


    m_BasicEffect->Apply(m_pContext.Get());
    m_pContext->IASetInputLayout(m_InputLayout.Get());
    m_pContext->GSSetShader(nullptr, nullptr, 0);

    m_PrimitiveBatch->Begin();


    XMFLOAT3 tl = XMFLOAT3(X-W, Y+H, 0);
    XMFLOAT3 tr = XMFLOAT3(X + W, Y+H, 0);
    XMFLOAT3 br = XMFLOAT3(X + W, Y -H, 0);
    XMFLOAT3 bl = XMFLOAT3(X-W,  Y-H, 0);

    m_PrimitiveBatch->DrawLine(VertexPositionColor(tl, color), VertexPositionColor(tr, color));
    m_PrimitiveBatch->DrawLine(VertexPositionColor(tr, color), VertexPositionColor(br, color));
    m_PrimitiveBatch->DrawLine(VertexPositionColor(br, color), VertexPositionColor(bl, color));
    m_PrimitiveBatch->DrawLine(VertexPositionColor(bl, color), VertexPositionColor(tl, color));

    m_PrimitiveBatch->End();

    // 효과 설정
    //m_BasicEffect->SetWorld(XMMatrixIdentity());
    //m_BasicEffect->SetView(XMMatrixIdentity());
    //

    //m_BasicEffect->Apply(m_pContext.Get());
    //m_pContext->IASetInputLayout(m_InputLayout.Get());
    //m_pContext->GSSetShader(nullptr, nullptr, 0); // GS 비우기

    //// 그리기
    //m_PrimitiveBatch->Begin();
    //m_PrimitiveBatch->DrawLine(
    //    { { 10.f, 10.f, 0.f }, Colors::Green },
    //    { { 200.f, 200.f, 0.f }, Colors::Green });
    //m_PrimitiveBatch->End();
    return S_OK;
}
