#pragma once
#include"Base.h"
BEGIN(Engine)
enum class PIPELINE_MATRICES
{
	D3DTS_VIEW,
	D3DTS_PROJ,
	D3DTS_END,
};
class CPipeLine final : public CBase
{
	
private:
	CPipeLine() {};
public:
	virtual ~CPipeLine() {};
	HRESULT Initialize(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext); 
	void Update();
public:
	const _float4x4* GetPipeLineMatrixPtr(PIPELINE_MATRICES _Matrix) { return &m_Matrices[(_uint)_Matrix]; };
	const _float4x4* GetInverseMatrixPtr(PIPELINE_MATRICES _Matrix) { return &m_InverseMatrices[(_uint)_Matrix]; };
	void SetPipeLine(PIPELINE_MATRICES _MatrixToSet, _matrix _Matrix) { XMStoreFloat4x4((_float4x4*)(&m_Matrices[(_uint)_MatrixToSet]) , _Matrix); }

private:
	_float4x4 m_Matrices[(_uint)PIPELINE_MATRICES::D3DTS_END ] = {};
	_float4x4 m_InverseMatrices[(_uint)PIPELINE_MATRICES::D3DTS_END] = {};

	_float m_Far = {}, m_Near = {};
	ComPtr <ID3D11DeviceContext> m_pContext; 
	ComPtr<ID3D11Device> m_pDevice; 
public:
	static CPipeLine* Create(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext> pContext);

}; 
END
