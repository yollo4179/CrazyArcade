#pragma once
#include"VIBuffer.h"
BEGIN(Engine)
class ENGINE_DLL CVIBufferPointRect final : public CVIBuffer
{
private:
	CVIBufferPointRect(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CVIBufferPointRect(const CVIBufferPointRect& Prototype);
public:
	virtual ~CVIBufferPointRect() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
#ifdef _DEBUG
	virtual void Inspector()override {};
#endif
public:
	static shared_ptr<CVIBufferPointRect> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	virtual shared_ptr<CComponent> Clone(void* pArg) override;
};
END
