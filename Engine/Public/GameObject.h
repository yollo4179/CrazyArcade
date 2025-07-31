#pragma once
#include"Base.h"
#include"Transform.h"
BEGIN(Engine)

/*Json ->트랜스폼 + 텍스처 태그 + 버퍼 , 셰이더 태그 저장용*/
typedef  struct OBJECT_NODE {
	/*Root*/
	DirectX::XMFLOAT4X4 SRT_MATRIX;
	DirectX::XMFLOAT4 ATLASINFO;
	/*오브젝트 태그*/
	string OBJECT_TAG;
	_uint OBJECT_LEVEL;
	/*컴포넌트 태그*/
	string  TEXTURE_TAG;
	_uint   TEXTURE_LEVEL;
	string  BUFFER_TAG;
	_uint   BUFFER_LEVEL;
	string  SHADER_TAG;
	_uint   SHADER_LEVEL;

	/*Child*/
	vector<OBJECT_NODE>CHILDREN;
	
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(\
		OBJECT_NODE,\
		SRT_MATRIX,\
		ATLASINFO,\
		OBJECT_TAG,\
		OBJECT_LEVEL,\
		TEXTURE_TAG,\
		TEXTURE_LEVEL,\
		BUFFER_TAG,\
		BUFFER_LEVEL,\
		SHADER_TAG,\
		SHADER_LEVEL,\
		CHILDREN);

}OBJECTNODE;
enum class COMPONENT_TAG{
	TAG_OBJECT,
	TAG_TEXTURE,
	TAG_BUFFER,
	TAG_SHADER,
	TAG_END
};

class ENGINE_DLL CGameObject abstract :public CBase, public std::enable_shared_from_this<CGameObject>
{
	/*ObjectDesc*/
public:
	typedef struct Objectdesc : public CTransform::TransformDesc
	{
		//모든 게임 오브젝트는 트랜스폼을 가진다 . 정보 필요 
		/*Transform Informaion*/
		_float3 ObjectPosition;
		_float3 ObjectSize;

	}OBJECT_DESC;
	

	/*사본 관리 */
protected:
	CGameObject(const ComPtr<ID3D11Device>& pDevice, const ComPtr<ID3D11DeviceContext>& pContext);
	CGameObject(const CGameObject& Prototype);

public:
	virtual ~CGameObject() = default;
protected:
	virtual HRESULT ReadyProto();
	virtual HRESULT ReadyChildren();

protected:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void					SettingInitializeBeforeFirstUpdate();
	_bool					m_HasUpdateStarted = { false };
public:
	virtual void			Priority_Update(_float fTimeDelta);
	virtual void			Update(_float fTimeDelta);
	virtual void			Late_Update(_float fTimeDelta);
	virtual HRESULT			Render();
	virtual HRESULT			BindBufferDesc();
	virtual void			Enable();
	virtual void			Disalbe();
public:
	virtual shared_ptr<CGameObject>			GetShared() { return shared_from_this(); }
	_wstring GetName() { return m_strName; };
	void SetName(const _wstring strName) { m_strName = strName; };
	virtual void Active() { m_IsActive = true;  }
	virtual void InActive() { m_IsActive = false; }

#ifdef _DEBUG
	virtual void Inspector();
#endif
	/*Util*/
	_bool IsMouseInDomain();
	const _float4& ComputeCollisionRect();
	shared_ptr<CTransform>& GetTransform() { return m_pTransformCom; };
public:
	/*Child*/
	void AddChildObject(shared_ptr<CGameObject>  pGameObject);
	void SetParent(weak_ptr<CGameObject> pParent);
	_bool HasParent() { return (m_pParent.lock().get() != nullptr); };
	const weak_ptr<CGameObject> GetParent() { return m_pParent; };
	shared_ptr<CGameObject> GetRootParent();
	void SetTransformBase();
	const _float4x4& ComputeWorldMatrixAsChild();
	const _matrix GetTransformWorldMatrix();
	_matrix GetChildWorldMatrix();
	/*Child*/
	void RearrangeChildGrid(
		_float2 Padding,
		_float2 Space,
		_float2 CellSize,
		_uint NumColumns = 1
	);
	/*Tool*/
	void ResetTexture(shared_ptr<CTexture> NewTexture);
	void SetProtoTextureInfo(const _wstring& newProtoTag, _uint iLevel) 
	{ m_PtrotoTextureTag = newProtoTag;  m_ProtoTextureLevel = iLevel; };

	void SetProtoBufferInfo(const _wstring& newProtoTag, _uint iLevel)
	{
		m_ProtoBufferTag = newProtoTag;   m_ProtoBufferLevel = iLevel;};
	
	void SetProtoShaderInfo(const _wstring& newProtoTag, _uint iLevel) 
	{
		m_ProtoShaderTag = newProtoTag;   m_ProtoShaderLevel = iLevel;};
	void SetProtoObjTagInfo(const _wstring& newProtoTag, _uint iLevel)
	{
		m_ProtoObjTag = newProtoTag; m_ProtoObjLevel = iLevel; 
	}


	void SetAtlasInfo(_float4 NewInfo) { m_AtlasInfo; };
	void WriteJsonFile(json& j);
	void ReconstructFromJson(json& j);
	HRESULT MakeComponentsFromTagInToolLevel();
	HRESULT MakeComponentsFromTagInAnyLevel();
	_wstring GetTag(COMPONENT_TAG Tag);

	template<typename T>
	void GetCloneObjectFromThis(shared_ptr<CGameObject>& pInstance, void* pArg);
public:
	template< typename T>
	shared_ptr<CComponent> GetComponent()
	{
		for (auto Component : m_Components)
		{
			if (nullptr != dynamic_cast<T*>(Component.second.get()))
				return Component.second;
		}
		return nullptr;
	}
	shared_ptr<CComponent> FindComponentByName(const _wstring Tag);
	shared_ptr<CGameObject> FindChildByIndex(_uint Index);
	shared_ptr<CGameObject> FindChildOnMouse();
	vector<shared_ptr<CGameObject> >& GetChildren() { return m_Children;  }
	
protected:
	//HRESULT Add_CCComponent();

	map<const _wstring, shared_ptr<CComponent> > m_Components;//컴포넌트는 map으로 관리 셀제로 tag 로 관리가능 
	vector<shared_ptr<CGameObject> > m_Children;
	ComPtr<ID3D11Device> m_pDevice;
	ComPtr<ID3D11DeviceContext> m_pContext;
	shared_ptr<CGameInstance> m_pGameInstance;

	/*BaseComponents*/
	shared_ptr<CShader> m_pShaderCom = {};
	shared_ptr<CTransform> m_pTransformCom = {}; 
	shared_ptr <CVIBuffer> m_pBufferCom = {};  
	shared_ptr <CTexture> m_pTextureCom = {}; 
	_int m_TexIndex = {}; 
#ifdef _DEBUG
	shared_ptr <CDebugLine> m_pDebugLineCom = {};
#endif
	/*For Tool*/
	_wstring m_ProtoObjTag = {};
	_wstring m_PtrotoTextureTag = {};
	_wstring m_ProtoBufferTag = {};
	_wstring m_ProtoShaderTag = {};
	_uint m_ProtoObjLevel = {};
	_uint m_ProtoTextureLevel = {}; 
	_uint m_ProtoBufferLevel = {};
	_uint m_ProtoShaderLevel = {};
	_float4  m_AtlasInfo = {};

	json jsonInfo = {};
	_bool m_HasUpdateStarted_Derived = {false};

protected:
	_bool m_IsActive = { true };
	_wstring m_strName ;
protected:
	/*ChildThing */
	weak_ptr<CGameObject>m_pParent{}; 
	_float3 BaseSizeFloat3 = { 0.2f,0.2f,1.f };
	_float4x4 m_ChildeWorldMatrix = {};

private:
	_float4 m_CollisionRect = {};
public:
	virtual shared_ptr<CGameObject> Clone(void* pArg) = 0;


};
END

template<typename T>
inline void  CGameObject::GetCloneObjectFromThis(shared_ptr<CGameObject>&pInstance,void* pArg)
{
	
	pInstance=this->Clone(pArg);

	/*컴포넌트 채우기 */
	pInstance->SetProtoObjTagInfo(m_ProtoObjTag, m_ProtoObjLevel);
	pInstance->SetProtoTextureInfo(m_PtrotoTextureTag, m_ProtoTextureLevel);
	pInstance->SetProtoBufferInfo(m_ProtoBufferTag, m_ProtoBufferLevel);
	pInstance->SetProtoShaderInfo(m_ProtoShaderTag, m_ProtoShaderLevel);
	pInstance->SetAtlasInfo(m_AtlasInfo);
	pInstance->MakeComponentsFromTagInToolLevel();
	/*Owner 지정하기 */
	/*복사 대상에 부모가 있다면 걔도 부모로 만든다.  이름 짓기*/
	/*if (m_pParent.lock())
		m_pParent.lock()->AddChildObject(pInstance);*/
	/*트랜스폼 조정 후에 */
	
	shared_ptr<CGameObject>ChildPtr;
	for (auto Child : m_Children)
	{
		 Child->GetCloneObjectFromThis<T>(ChildPtr,pArg);
		pInstance->AddChildObject(ChildPtr);
		ChildPtr->GetTransform()->SetWorld(Child->GetTransformWorldMatrix());
	}
	


}
