#pragma once
#include"Base.h"
BEGIN(Engine)
enum {
	RENDER_UI,/*For Crazy Arcade Only*/
	RENDER_FLOOR,
	RENDER_OBJECTS,
	RENDER_CHARACTER,
	RENDER_GLOW,
	RENDER_END,
};
class CRenderer final :public CBase
{
	
private:
	CRenderer(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext>pContext);
public:
	virtual ~CRenderer();
public:
	HRESULT Initialize();
public:
	HRESULT DrawObjects();
	HRESULT AddRenderObject(_uint RenderGroup, shared_ptr<CGameObject>& pGameObject);
	HRESULT RenderUI(); 

private:
	ComPtr<ID3D11Device>m_pDevice;
	ComPtr<ID3D11DeviceContext>m_pContext;
	list<shared_ptr<CGameObject> >m_pGameObjects[RENDER_END];
public:
	static CRenderer* Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext>);

};
END
