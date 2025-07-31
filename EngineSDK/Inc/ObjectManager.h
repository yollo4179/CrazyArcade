#pragma once
#include"Base.h"
BEGIN(Engine)
class CObjectManager final:public CBase
{
private:
	CObjectManager();
public:
	virtual ~CObjectManager()=default;
public:	
	HRESULT Initialize(_uint iNumLevels);
public:
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	
	void Late_Update(_float fTimeDelta);
	void ClearObjectsOnLevel(_uint iLevelIndex);
	void ClearObjectsOnAllLevels();
public:
	/*Utility*/
	HRESULT AddCopyObjectToLayer(
		_uint iPrototypeLevel,
		const _wstring& PrototypeTag,
		_uint iLayerLevel,
		const _wstring& strLayerTag,
		void* pArg = nullptr,
		shared_ptr<CGameObject>* m_pGameObject=nullptr
		);
	HRESULT AddCopyObjectToLayer(
		_uint iPrototypeLevel,
		const _wstring& PrototypeTag,
		_uint iLayerLevel,
		const _wstring& strLayerTag,
		shared_ptr<CGameObject> GameObjectPtr
	);

	HRESULT AddGameObjectToLayer
	(
		_uint iLayerLevel,
		const _wstring& strLayerTag,
		CGameObject* pObject
	);
	HRESULT DeleteGameObjectFromLayer(
		_uint iLayerLevel,
		const _wstring& strLayerTag,
		shared_ptr<CGameObject> m_pGameObject
	);
	HRESULT DeleteGameObjectFromLayer(
		_uint iLayerLevel,
		const _wstring& strLayerTag,
		const _wstring ObjectName
	);
	shared_ptr<CGameObject> FindObjectByName
	(
		_uint iLayerLevel,
		const _wstring& strLayerTag,
		const _wstring ObjectName
	);
		
public:
	/*Util*/
	


private:
	_uint	m_iNumLevels = {};
	typedef map<const _wstring, shared_ptr<CLayer> > LAYERS;
	unique_ptr<LAYERS[]> m_pLayers = {};
	
	weak_ptr<CGameInstance> m_pGameInstance = {}; 
private:
	shared_ptr<CLayer> FindLayer(_uint iLevel, const _wstring& strLayer);

public:
	static CObjectManager* Create(_uint iNumLevels);
};
END
