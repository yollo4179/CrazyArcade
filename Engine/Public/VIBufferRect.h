#pragma once
#include"VIBuffer.h"
BEGIN(Engine)
class ENGINE_DLL CVIBufferRect final : public CVIBuffer
{
private:
		CVIBufferRect(ComPtr<ID3D11Device>pDevice, ComPtr<ID3D11DeviceContext>pContext);
protected:
		CVIBufferRect(const CVIBufferRect& Prototype);
public:
		virtual ~CVIBufferRect() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
#ifdef _DEBUG
	virtual void Inspector()override {};
#endif

public:
	static shared_ptr<CVIBufferRect> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	virtual shared_ptr<CComponent> Clone (void* pArg) override;

};
END

