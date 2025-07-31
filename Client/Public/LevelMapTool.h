#pragma once
#include"Client_Defines.h"
#include"Level.h"
#include"ToolObject.h"
#ifdef _DEBUG
BEGIN(Client)
enum class COMPONENT_TYPE
{
	TYPE_OBJECT,
	TYPE_SHADER,
	TYPE_BUFFER,
	TYPE_TEXTURE,
	TYPE_END,
};
class CLevelMapTool final : public CLevel
{
	typedef struct OBJECT_DESC
	{
		_float		m_fSpeedPerSecond = 10.f;
		_float		m_fRotationPerSec = XMConvertToRadians(10.f);
		_float2		m_vSize;
		_float3		m_vPosition;
		/*Object Name*/
		/*Com1_ Texture*/
		_wstring    m_TextureTag;
		
		/*Com2_ Shader*/
		_wstring    m_ShaderTag;
		/*Com3 Buffer*/
		_wstring    m_BufferTag;

		vector<OBJECT_DESC>m_vecChild = {}; 
	};
	
private:
		CLevelMapTool(const ComPtr<ID3D11Device>& pDevice, const ComPtr<ID3D11DeviceContext>& pContext);

public:	
		virtual ~CLevelMapTool();

private:
	void CopyTexture();
public:
		HRESULT Initialize()  override;
		void Update(_float fTimeDelta)  override;


		void UpdateDemo();
		void ManageObjects();//트리노드로 하나씩 추가 (오브젝트)
		shared_ptr<CToolObject> CreateObject();
		void CreateObjectButton();
		void DisplayAllObjects();
		void DisplayNowObject(); 
		void DeleteNowObject();
		void AddChildToNowObject();
		void ReArrangeChildrenOf(); 
		void CloneNowObjectButton();
		 CToolObject::TOOLOBJECTDESC GetToolObjectDesc();
		vector <shared_ptr< CToolObject>>vecObjects;
		shared_ptr< CToolObject>m_NowObject;
		void LoadObject();
		
		void ObjectBuilder(json& j,shared_ptr<CGameObject>pGameObject);
		void TagLevelPairMapper();
		void RegisterTagLevelPair();
		void ShowComponentList(const string& ListName, map<_wstring, _uint>&TagMapper, _wstring& Key,COMPONENT_TAG Tag);
		map<_wstring, _uint >m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_END];
		_wstring m_NowObjectKey = {};
		_wstring m_NowTextureKey = {}; 
		_wstring m_NowBufferKey = {};
		_wstring m_NowShaderKey = {};
		void DecideTagInfoButton();


		void ManageTextures();
		void AddTextureToContaner(_wstring ProtoTag);
		void ShowTextureListBox();
		void ShowSelectedTexture(); 
		void SetNowTexture2NowObject();
		shared_ptr<class CTexture>m_NowTexture;

		
		
public:
	map<_wstring,shared_ptr<CTexture>>m_Textures;
public:
	static CLevelMapTool* Create(const ComPtr<ID3D11Device>& pDevice,const  ComPtr<ID3D11DeviceContext>& pContext);

		

};
END
#endif