#pragma once
#include "KeyManager.h"
#include"PrototypeManager.h"
#include "Renderer.h"
#include "PipeLine.h"
#include"Texture.h"
#include"Transform.h"
#include"VIBuffer.h"
#include"VIBufferLine.h"
#include"Shader.h"
#include"ImguiManager.h"
#include"UIManager.h"
#include"Base.h"
BEGIN(Engine)
class ENGINE_DLL CGameInstance final :public CBase 
{
	DECLARE_SINGLETON(CGameInstance)
public:
	CGameInstance();
	virtual ~CGameInstance();
	HRESULT InitialzeEngine(const ENGINE_DESC& EngineDesc, ComPtr<ID3D11Device>& ppDevice, ComPtr<ID3D11DeviceContext>& ppContext);


public:
	void Update_Engine(_float fTimeDelta);
	/*Util*/
	wstring Mbcs2Wbcs(string wstr);
	string Wbcs2Mbcs(wstring str);
	_float  GetHeight() { return m_WinSizeY; };
	_float  GetWidth() { return m_WinSizeX; };
	_float2 GetViewPortSize() {return  _float2(m_WinSizeX, m_WinSizeY); }
	/*GraphicDevice */
	HRESULT Render_Begin(_float4 ClearColor); 
	HRESULT Render_End();
	HRESULT Render();
	/*Renderer*/

	/*For Timer*/
	void UpdateTimer(const _wstring& strTimerTag);
	class CTimer* FindTimer(const _wstring& strTimerTag);
	_float GetTimeDelta(const _wstring& strTimerTag);
	HRESULT	AddTimer(const _wstring& strTimerTag);

	/*For Prototype Manager*/
	HRESULT AddProtoType(_uint iLevel, const _wstring strPrototypeTag, shared_ptr<CBase> pPrototype);
	HRESULT ErasePrototype(_uint iLevel, const _wstring strPrototype);
	shared_ptr<CBase>FindPrototype(_uint iLevelIndex, const _wstring& strPrototypeTag);
	shared_ptr<CBase>Clone_Prototype(PROTOTYPE eType, _uint iLevel, const _wstring strPrototag, void* pArg);
	void ClearPrototypesOnLevel(_uint iLevelIndex);
	void ClearPrototypesOnAllLevels();
	
	/*For Object Manager*/
	HRESULT AddCopyObjectToLayer(
		_uint iPrototypeLevel,
		const _wstring& PrototypeTag,
		_uint iLayerLevel,
		const _wstring& strLayerTag,
		void* pArg = nullptr,
		shared_ptr<CGameObject>* m_pGameObject =nullptr
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
	void ClearObjectsOnLevel(_uint iLevelIndex);
	void ClearObjectsOnAllLevels();
	/*For Level Manager*/
	HRESULT OpenLevel(_int iNewLevelID, const shared_ptr<CLevel>&pNewLevel);
	void    ClearLevelManager();
	_uint GetNowLevel();
	/*For KeyManager */

	INPUT_STATE GetKeyState(KEY eKey);
	INPUT_STATE GetMouseState(MOUSE eKey);
	_long GetMouseDIInput(MOUSEMOVE eMouseMove);
	POINT	GetMousePos();
	_bool		CheckMousePos(RECT _tRect);
	/*For Renderer*/
	HRESULT AddRenderObject(_uint RenderGroup, shared_ptr<CGameObject>& pGameObject);


	/*For PipeLine */
	const _float4x4* GetPipeLineMatrixPtr(PIPELINE_MATRICES _Matrix);
	const _float4x4* GetInverseMatrixPtr(PIPELINE_MATRICES _Matrix);
	void SetPipeLine(PIPELINE_MATRICES _MatrixToSet, _matrix _Matrix);
	/*For UIManager*/
	void ClearUIObjects();
	void AddUIObject(_wstring UITag, shared_ptr<CUIObject> pUIObject);
	shared_ptr<CUIObject> GetUIObjectByTag(_wstring UITag);
	shared_ptr<CUIObject> GetSelectedObject();
	void DeleteUIObjectByName(_wstring UITag);
	void SetToolMode(_bool Ret);

	/*Fomt Manager*/
	HRESULT AddFont(const _wstring& strFontTag, const _tchar* pFontFilePath);
	HRESULT RenderFont(const _wstring& strFontTag, const _tchar* pText, const _float2& vPosition, _fvector vColor, _float fRotation = 0.f, const _float2& vOrigin = _float2(0.f, 0.f), _float f
		= 1.f);
	shared_ptr<SpriteFont> GetSpriteFont(const _wstring& strFontTag);
	shared_ptr <SpriteBatch> GetSpriteBatch(const _wstring& strFontTag);
private:
	_int m_WinSizeX; 
	_int m_WinSizeY; 
	
private:
	/*ÀåÄ¡*/
	ComPtr<ID3D11Device> m_pDevice = {}; 
	ComPtr <ID3D11DeviceContext>m_pContext = {}; 
	/*Managers*/
private:
	shared_ptr < CTimerManager>		m_pTimerManager = {}; 
	shared_ptr < CGraphicDevice>	m_pGrapicDevice = {}; 
	
	shared_ptr <CPrototypeManager>	m_pPrototypeManager = {}; 
	shared_ptr<CObjectManager>		m_pObjectManager = {};
	shared_ptr <CLevelManager>		m_pLevelManager = {};
	shared_ptr<CKeyManager>			m_pKeyManager = {};	
	shared_ptr<CRenderer>			m_pRenderer = {};
	shared_ptr<CPipeLine>			m_pPipeLine = {}; 
#ifdef _DEBUG
	shared_ptr<CImguiManager>		m_pImguiManager = {};
#endif
	shared_ptr<CUIManager>			m_pUIManager = {};
	shared_ptr<CFontManager>		m_pFontManager = {}; 
}; 
END

