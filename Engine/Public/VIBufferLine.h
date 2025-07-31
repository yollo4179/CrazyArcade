#pragma once
#include"Component.h"
BEGIN(Engine)
class ENGINE_DLL CDebugLine final:public CComponent
{
private:
	CDebugLine(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CDebugLine(const CDebugLine& Prototype);
public:
	virtual ~CDebugLine() = default;
public:
	virtual HRESULT Initialize_Prototype() ;
	virtual HRESULT Initialize(void* pArg) ;
public:
	HRESULT DrawBox(_float X, _float Y, _float Size_X, _float Size_Y, _float4 Color);

#ifdef _DEBUG
	virtual void Inspector()override {};
#endif

private:
	shared_ptr<PrimitiveBatch<DirectX::VertexPositionColor>> m_PrimitiveBatch;
	shared_ptr<DirectX::BasicEffect>m_BasicEffect;
	ComPtr<ID3D11InputLayout> m_InputLayout;
	
public:
	shared_ptr<CComponent> Clone(void* pArg);
	static shared_ptr<CDebugLine> Create(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext>pContext);

};
END
