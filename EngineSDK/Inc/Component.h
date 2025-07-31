#pragma once
#include"Base.h"
BEGIN(Engine)
class ENGINE_DLL CComponent abstract :public CBase
{

public:
	typedef struct ComDesc
	{
		shared_ptr<CGameObject>  pOwner = { nullptr };
	}CComponent_DESC;


protected:
	CComponent(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	CComponent(const CComponent& Prorotype);
public:
	virtual ~CComponent() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void	Priority_Update(_float fTimeDelta) {};
	virtual void	Update(_float fTimeDelta) {};
	virtual void	Late_Update(_float fTimeDelta) {};

	virtual void Enable() {};
	virtual void Disable() {};

public:
	
	inline void SetOwner(std::shared_ptr<CGameObject> pOwner) { m_pOwner = pOwner; };

#ifdef _DEBUG
public:
	virtual HRESULT Render() { return S_OK; };
	virtual void SaveInfo() {};
	virtual void Inspector() =0;
#endif



protected:
	ComPtr<ID3D11Device>m_pDevice; 
	ComPtr<ID3D11DeviceContext> m_pContext;
	shared_ptr<CGameInstance> m_pGameInstance;

	_bool m_isCloned = { false };
	_bool m_isActive = { true  };
	std::weak_ptr<CGameObject> m_pOwner = { };

public:
	virtual shared_ptr<CComponent> Clone(void* pArg)=0;





};
END
