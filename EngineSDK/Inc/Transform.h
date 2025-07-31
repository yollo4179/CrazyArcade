#pragma once
#include"Component.h"
BEGIN(Engine)
class ENGINE_DLL CTransform final :public CComponent
{
public:
	typedef struct TransformDesc
	{
		_float fSpeedPerSec = {};
		_float fRotationPerSec = {}; 

	}TRANSFORM_DESC ;

	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

private:
	CTransform(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext>pContext);
	CTransform(const CTransform& Prototype);
public:
	virtual ~CTransform() = default;

	HRESULT Initialize(void * pArg);
	HRESULT Initialize_Prototype(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext>pContext);
public:
	void SetWorld(_fmatrix matWorld){
		XMStoreFloat4x4(&m_WorldMatrix, matWorld);
	}
	void SetState(STATE eState, _fvector vState)
	{
		XMStoreFloat4((_float4*)(&m_WorldMatrix.m[eState]),vState);
	}
	_fvector GetState(STATE eState)
	{
		return XMLoadFloat4x4(&m_WorldMatrix).r[eState];
	}
	_matrix GetWorldMatrix()
	{
		return XMLoadFloat4x4(&m_WorldMatrix);
	}
	const _float4x4* GetWorldMatrixPtr()
	{
		return &m_WorldMatrix;
	}
	_float4x4& GetWorldMatrixRef() { return m_WorldMatrix; }
	_matrix Get_WorldMatrixInverse() {
		return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix));
	}
	_float3 ComputeScale();//트랜스폼의 각 길이(크기)는 스케일 
	

	void SetRotation(_float3 RotationXYZ);
	void Scailing(_float fX, _float fY,_float fZ=1);
	void Scailing(_float3 size);
	void GoLeft(_float fTimeDelta);
	void GoRight(_float fTimeDelta);
	void GoUp(_float fTimeDelta);
	void GoDown(_float fTimeDelta);

public:
	/*World Matrix*/
	HRESULT Bind_ShaderResource(class CShader* pShader, const _char* pConstantName);
#ifdef _DEBUG
	virtual void Inspector()override;
#endif

private:
	_float4x4 m_WorldMatrix = {};

	_float m_fSpeedPerSec = {};
	_float m_fRotationPerSec = {}; 

#ifdef _DEBUG
#endif

public:
	static shared_ptr<CTransform> Create(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext>pContext);
	virtual shared_ptr<CComponent> Clone(void* pArg) override;

};
END
