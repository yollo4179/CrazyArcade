#pragma once
#include"Base.h"
BEGIN(Engine)
class ENGINE_DLL CLevel abstract :public CBase
{
	protected:
	CLevel(const ComPtr<ID3D11Device>& pDevice,const ComPtr<ID3D11DeviceContext>& pContext);
public:
	virtual ~CLevel() = default;

	virtual HRESULT Initialize();
	virtual void Update(_float fTimeDelta);

#ifdef _DEBUG
	void Render() {};
#endif	

protected:
	ComPtr<ID3D11Device> m_pDevice = { nullptr };
	ComPtr<ID3D11DeviceContext> m_pContext = { nullptr };
	weak_ptr<CGameInstance> m_pGameInstance = {  };



};
END
