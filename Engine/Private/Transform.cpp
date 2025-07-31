#include "Transform.h"
#include"Shader.h"
#include"GameInstance.h"
#include"GameObject.h"
CTransform::CTransform(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext>pContext)
    :CComponent{pDevice, pContext}
{
}

CTransform::CTransform(const CTransform& Prototype)
    :CComponent(Prototype)
{
}

HRESULT CTransform::Initialize(void* pArg)
{
    if (nullptr == pArg)
        return S_OK;

    TRANSFORM_DESC* pDesc = static_cast<TRANSFORM_DESC*>(pArg);

    m_fSpeedPerSec = pDesc->fSpeedPerSec;
    m_fRotationPerSec = pDesc->fRotationPerSec;

   XMStoreFloat4x4(&m_WorldMatrix,XMMatrixIdentity());
    return S_OK;
}

HRESULT CTransform::Initialize_Prototype(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{

    m_pDevice = pDevice;
    m_pContext = pContext;

    return S_OK;
}

_float3 CTransform::ComputeScale()
{
    _float ScaleX = XMVector3Length(GetState(STATE::STATE_RIGHT)).m128_f32[0];
    _float ScaleY = XMVector3Length(GetState(STATE::STATE_UP)).m128_f32[0];
    _float ScaleZ = XMVector3Length(GetState(STATE::STATE_LOOK)).m128_f32[0];
    return _float3(ScaleX, ScaleY, ScaleZ);
}

void CTransform::SetRotation(_float3 RotationXYZ)
{
    _float3		vScale = ComputeScale();

    _vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScale.x;
    _vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScale.y;
    _vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScale.z;

    _matrix		RotationMatrixX = XMMatrixRotationAxis(_vector{1.f, 0.f, 0.f, 0.f}, XMConvertToRadians(RotationXYZ.x));
    _matrix		RotationMatrixY = XMMatrixRotationAxis(_vector{ 0.f, 1.f, 0.f, 0.f }, XMConvertToRadians(RotationXYZ.y));
    _matrix		RotationMatrixZ = XMMatrixRotationAxis(_vector{ 0.f, 0.f, 1.f, 0.f }, XMConvertToRadians(RotationXYZ.z));
    _matrix     RotationMatrix = RotationMatrixX * RotationMatrixY * RotationMatrixZ;
    SetState(STATE_RIGHT, XMVector3TransformNormal(vRight, RotationMatrix));
    SetState(STATE_UP, XMVector3TransformNormal(vUp, RotationMatrix));
    SetState(STATE_LOOK, XMVector3TransformNormal(vLook, RotationMatrix));
}

void CTransform::Scailing(_float ScaleX, _float ScaleY, _float ScaleZ)
{
    SetState(STATE_RIGHT,ScaleX* XMVector3Normalize(GetState(STATE_RIGHT)));
    SetState(STATE_UP, ScaleY * XMVector3Normalize(GetState(STATE_UP)));
    SetState(STATE_LOOK, ScaleZ * XMVector3Normalize(GetState(STATE_LOOK)));
}

void CTransform::Scailing(_float3 Scale)
{
    SetState(STATE_RIGHT, Scale.x * XMVector3Normalize(GetState(STATE_RIGHT)));
    SetState(STATE_UP, Scale.y * XMVector3Normalize(GetState(STATE_UP)));
    SetState(STATE_LOOK, Scale.z * XMVector3Normalize(GetState(STATE_LOOK)));
}

void CTransform::GoLeft(_float fTimeDelta)
{
    _vector vPosition = GetState(STATE_POSITION);
    _vector vRight = XMVector3Normalize(GetState(STATE_RIGHT));

    vPosition -= vRight * fTimeDelta * m_fSpeedPerSec;
    SetState(STATE_POSITION, vPosition);


}

void CTransform::GoRight(_float fTimeDelta)
{
    _vector vPosition = GetState(STATE_POSITION);
    _vector vRight = XMVector3Normalize(GetState(STATE_RIGHT));
    vPosition -= vRight * fTimeDelta * m_fSpeedPerSec;
    SetState(STATE_POSITION, vPosition);
}

void CTransform::GoUp(_float fTimeDelta)
{
    _vector vPosition = GetState(STATE_POSITION);
    _vector vUp = XMVector3Normalize(GetState(STATE_UP));
    vPosition += vUp * fTimeDelta * m_fSpeedPerSec;
    SetState(STATE_POSITION, vPosition);
}

void CTransform::GoDown(_float fTimeDelta)
{
    _vector vPosition = GetState(STATE_POSITION);
    _vector vUp = XMVector3Normalize(GetState(STATE_UP));
    vPosition -= vUp * fTimeDelta * m_fSpeedPerSec;
    SetState(STATE_POSITION, vPosition);
}

HRESULT CTransform::Bind_ShaderResource(CShader* pShader, const _char* pConstantName)
{
    return pShader->Bind_Matrix(pConstantName,&m_WorldMatrix);
}
#ifdef _DEBUG

void CTransform::Inspector()
{
   
    /*스케일링*/
    ImGui::Separator();
    ImGui::Text("트랜스폼 인스펙터");
    ImGui::Separator();
    _float3 ScalingFloat3 = ComputeScale();
    if (m_pOwner.lock() && m_pOwner.lock()->HasParent())
    {
        ImGui::SliderFloat3("스케일링", (_float*)(&ScalingFloat3), 0.01f, 1.f );
    }
    else
    {
        ImGui::SliderFloat3("스케일링", (_float*)(&ScalingFloat3), 0.1f, VIEWPORT_WIDTH());
    }
    Scailing(ScalingFloat3);
    ImGui::Separator();
    /* 크기*/
   static _float3 RotationSet = {};
    ImGui::SliderFloat3("회전: X Y Z", (_float*)(&RotationSet), 0.f, 360.f);
    SetRotation(RotationSet);
    ImGui::Separator();
   // /* 이동(위치)*/

   _vector vTranslation =  GetState(STATE_POSITION);
   _float3 TransFloat3 = {};
   XMStoreFloat3(&TransFloat3,vTranslation);
   if (m_pOwner.lock() && m_pOwner.lock()->HasParent())
   {
       _float4 CollisionInfo = m_pOwner.lock()->GetParent().lock()->ComputeCollisionRect();
       _float2 ParentCenter = { (CollisionInfo.x + CollisionInfo.y) * 0.5f ,(CollisionInfo.z + CollisionInfo.w) * 0.5f };

       ImGui::SliderFloat("X 이동량", (_float*)(&TransFloat3.x), CollisionInfo.x- ParentCenter.x, CollisionInfo.y- ParentCenter.x);
       ImGui::SliderFloat("Y 이동량 ", (_float*)(&TransFloat3.y), CollisionInfo.z- ParentCenter.y, CollisionInfo.w- ParentCenter.y);
   }
   else
   {
       ImGui::SliderFloat("X 이동량", (_float*)(&TransFloat3.x), -VIEWPORT_WIDTH() / 2, VIEWPORT_WIDTH() / 2);
       ImGui::SliderFloat("Y 이동량 ", (_float*)(&TransFloat3.y), -VIEWPORT_HEIGHT() / 2, VIEWPORT_HEIGHT() / 2);
   }

   
   ImGui::SliderFloat("Z 이동량 ", (_float*)(&TransFloat3.z), 0.f, 1.F);
    SetState(STATE_POSITION, XMVectorSetW(XMLoadFloat3(&TransFloat3),1.f));
    ImGui::Separator();
}

#endif // DEBUG

shared_ptr<CTransform> CTransform::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
{
    shared_ptr<CTransform>pInstance ( new CTransform(pDevice, pContext));

    if (pInstance->Initialize_Prototype(pDevice, pContext))
    {
        MSG_BOX("트랜스폼 원본 초기화 실패");
     
    }
    return pInstance;
}

shared_ptr<CComponent> CTransform::Clone(void* pArg)
{
    shared_ptr<CTransform>pInstance ( new CTransform(*this));

    if (pInstance->Initialize(pArg))
    {
        MSG_BOX("트랜스폼 사본 초기화 실패");
     
    }
    return pInstance;
}
