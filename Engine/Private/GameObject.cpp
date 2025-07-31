#include "GameObject.h"
#include"GameInstance.h"
#include"UIRearrangeSystem.h"
CGameObject::CGameObject(const ComPtr<ID3D11Device>& pDevice,const ComPtr<ID3D11DeviceContext>& pContext)
	:m_pDevice(pDevice),
	m_pContext(pContext),
	m_pGameInstance(CGameInstance::GetInstance())
{
}

CGameObject::CGameObject(const CGameObject& Prototype)
	:m_pDevice(Prototype.m_pDevice),
	m_pContext(Prototype.m_pContext),
	m_pGameInstance(Prototype.m_pGameInstance)
{
}

HRESULT CGameObject::ReadyProto()
{
	return S_OK;
}

HRESULT CGameObject::ReadyChildren()
{
	return S_OK;
}

HRESULT CGameObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::Initialize(void* pArg)
{
	
	OBJECT_DESC* pDesc = static_cast<OBJECT_DESC*>(pArg);

	 m_Components.emplace(L"TRANSFORM.COM", shared_ptr<CTransform>(CTransform::Create(m_pDevice, m_pContext)));
	 m_pTransformCom = dynamic_pointer_cast<CTransform>(m_Components.find(L"TRANSFORM.COM")->second);


	 CTransform* pTransform = dynamic_cast<CTransform*>(GetComponent<CTransform>().get());

	pTransform->Initialize(pArg);
	pTransform->Scailing(pDesc->ObjectSize);
	pTransform->SetState(CTransform::STATE_POSITION, XMVectorSetW(XMLoadFloat3(&pDesc->ObjectPosition),1.f));
	

	return S_OK;
}

void CGameObject::SettingInitializeBeforeFirstUpdate()
{
	if (!m_HasUpdateStarted) {
		m_pTransformCom->SetOwner(shared_from_this());
		m_HasUpdateStarted = true;
	}
}

void CGameObject::Priority_Update(_float fTimeDelta)
{
	SettingInitializeBeforeFirstUpdate();
	for (const auto& Child : m_Children)
	{
		Child->Priority_Update(fTimeDelta);
	}

	for (const auto& Component : m_Components)
	{
		if (Component.second)
		Component.second->Priority_Update(fTimeDelta);
	}

}

void CGameObject::Update(_float fTimeDelta)
{
	for (const auto& Child : m_Children)
	{
		Child->Update(fTimeDelta);
	}

	for (const auto& Component : m_Components)
	{
		if(Component.second)
		Component.second->Update(fTimeDelta);
	}
}

void CGameObject::Late_Update(_float fTimeDelta)
{
	for (const auto& Child : m_Children)
	{
		Child->Late_Update(fTimeDelta);
	}

	for (const auto& Component : m_Components)
	{
		if (Component.second)
		Component.second->Late_Update(fTimeDelta);
	}
}

HRESULT CGameObject::Render()
{
	for (const auto& Child : m_Children)
	{
		Child->Render();
	}

#ifdef _DEBUG
	_float4 RectInfo =ComputeCollisionRect();
	_vector DebugPos = {(RectInfo.x + RectInfo.y)/2.f, (RectInfo.z + RectInfo.w)/2.f };//m_pTransformCom->GetState(CTransform::STATE_POSITION);
	_float3 DebugSize = { (RectInfo.y - RectInfo.x ) , (RectInfo.z - RectInfo.w),1.f }/*m_pTransformCom->ComputeScale()*/;
	m_pDebugLineCom->DrawBox(DebugPos.m128_f32[0], DebugPos.m128_f32[1], DebugSize.x / 2.f, DebugSize.y / 2.f, _float4(1.f, 0.f, 0.f, 1.f));
#endif
	return S_OK;
}

HRESULT CGameObject::BindBufferDesc()
{

	bool IsChild = (nullptr != m_pParent.lock().get());
	switch (IsChild)
	{
	case true:
		if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &ComputeWorldMatrixAsChild())))
			return E_FAIL;
		break;
	case false:
		if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom.get(), "g_WorldMatrix")))
			return E_FAIL;
		break;
	}

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->GetPipeLineMatrixPtr(PIPELINE_MATRICES::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->GetPipeLineMatrixPtr(PIPELINE_MATRICES::D3DTS_PROJ))))
		return E_FAIL;
	/*텍스처*/
	_bool IsEmpty = true;
	if (m_pTextureCom) {
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom.get(), "g_InputTexture", m_TexIndex)))
			return E_FAIL;
		IsEmpty = false;
	}
	
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Empty",&IsEmpty,sizeof(_bool))))
		return E_FAIL;

	return S_OK;
}

void CGameObject::Enable()
{
}

void CGameObject::Disalbe()
{
}
#ifdef _DEBUG

void CGameObject::Inspector()
{

	static char g_Address[MAX_PATH] = "../Resources/Json/";
	static char g_FileName[MAX_PATH] = "xxx";
	
	if (ImGui::TreeNode(m_pGameInstance->Wbcs2Mbcs(m_strName).c_str())) {
		/*ComPonents 먼저 */
		/*Texture + Transform + Collider + (Buffer)/*/
		ImGui::InputText("제이슨 파일 이름(JSON확장자 뺴고)", g_FileName , MAX_PATH * sizeof(char));
		if (ImGui::Button("제이슨 데이터 해당 주소로 저장"))
		{
			try {
				ofstream Out;
				json jsonData = {};
				WriteJsonFile(jsonData);
				Out.exceptions(std::ofstream::failbit | std::ofstream::badbit);
				string GameAdress = string(g_Address) + string(g_FileName) + string(".json");
				
				Out.open(GameAdress.c_str());
				Out << jsonData.dump(4); Out.close();
				
		
			}
			catch(const std::ios_base::failure& e){
				e;
				MSG_BOX("저장 에러 발생");
			
			}
			
		}
		
		
		for (auto Component : m_Components)
		{
			if(Component.second)
			Component.second->Inspector();
		}

		ImGui::TreePop();
	}

}
#endif // DEBUG
_bool CGameObject::IsMouseInDomain()
{
	_uint Num = 1; 
	D3D11_VIEWPORT ViewPort = {};
	m_pContext->RSGetViewports(&Num, &ViewPort);
	_uint WinSizeX = ViewPort.Width;
	_uint WinSizeY = ViewPort.Height;
	POINT MousePoint = m_pGameInstance->GetMousePos();
	/*Screen To NDC*/
	_float2 ndc = {};
	ndc.x = (2.f * MousePoint.x) / WinSizeX - 1.f;
	ndc.y = 1.f - (2.f * MousePoint.y) / WinSizeY;
	/*NDC To World*/
	_float2 WorldMouse = {};
	WorldMouse.x = ndc.x* (WinSizeX / 2.f);
	WorldMouse.y = ndc.y * (WinSizeY / 2.f);

	_float4 CollisionRect = ComputeCollisionRect();

	_bool ret = (
		CollisionRect.x <= WorldMouse.x &&
		WorldMouse.x <= CollisionRect.y &&
		CollisionRect.z <= WorldMouse.y &&
		WorldMouse.y <= CollisionRect.w
		);
	
	return ret;
}


const _float4& CGameObject::ComputeCollisionRect()
{
	_vector vPos;
	_float3 vSize;
	if (!m_pParent.lock().get()) {
		vPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
		vSize = m_pTransformCom->ComputeScale();
	}
	else
	{
		_float4x4 WorldFloat4x4 = ComputeWorldMatrixAsChild(); //or getChildMatrix
		_matrix matWorld = XMLoadFloat4x4(&WorldFloat4x4);
		vPos = matWorld.r[3];
		vSize = { XMVector3Length(matWorld.r[0]).m128_f32[0],
			XMVector3Length(matWorld.r[1]).m128_f32[0],
			XMVector3Length(matWorld.r[2]).m128_f32[0]
		};
	}
	_float2 LT = { vPos.m128_f32[0] - vSize.x / 2.F ,vPos.m128_f32[1] + vSize.y / 2.F };
	_float2 RB = { vPos.m128_f32[0] + vSize.x / 2.F ,vPos.m128_f32[1] - vSize.y / 2.F };
	
	RECT;
	return m_CollisionRect =_float4(LT.x, RB.x,	RB.y ,LT.y);//LEFT  RIGHT BOTTOM TOP 
}

void CGameObject::AddChildObject(shared_ptr<CGameObject> pGameObject)
{

	m_Children.push_back(pGameObject);
	/*SetOwner ->me */
	wstring newTag = wstring(L"Child_");
	_uint Count = 1;
	auto iter = m_Children.begin(); 
	do{
		if ((*iter)->GetName() == newTag + to_wstring(Count))
		{
			++Count;
		}

		iter++;
	} while (iter != m_Children.end());

	newTag += to_wstring(Count);
	pGameObject->SetName(newTag);
	pGameObject->SetParent(weak_from_this());
	pGameObject->SetTransformBase(); 
	//부모 매트릭스 x 자식 매트릭스 
	
}

void CGameObject::SetParent(weak_ptr<CGameObject> pParent)
{
	m_pParent = pParent;
}

shared_ptr<CGameObject> CGameObject::GetRootParent()
{

	if (HasParent())
		return m_pParent.lock()->GetRootParent();

	else
		return shared_from_this(); 


	return shared_ptr<CGameObject>();
}

void CGameObject::SetTransformBase()
{
	_matrix MatBase= XMMatrixIdentity();
	m_pTransformCom->SetWorld(MatBase);
	_float3 ScalingFloat3(BaseSizeFloat3);
	m_pTransformCom->Scailing(ScalingFloat3);
}

const _float4x4& CGameObject::ComputeWorldMatrixAsChild()
{
	_matrix matChild = m_pTransformCom->GetWorldMatrix();
	_matrix matChildSR = matChild;
	matChildSR.r[3] = _vector{ 0.f,0.f,0.f,1.f };

	_matrix matChildT = XMMatrixIdentity();
	matChildT.r[3] = matChild.r[3];

	_matrix retMat = {}; 
	
	
	
	if (m_pParent.lock()->HasParent())
	{
		retMat = matChildSR * m_pParent.lock()->GetChildWorldMatrix()*matChildT;
		//retMat.r[3] += matChild.r[3];

		XMStoreFloat4x4(
			&m_ChildeWorldMatrix,
			retMat 
		);

	}
	else if (m_pParent.lock().get())
	{
		retMat = matChildSR * m_pParent.lock()->GetTransformWorldMatrix() * matChildT;
		//retMat.r[3] += matChild.r[3];
		XMStoreFloat4x4(
			&m_ChildeWorldMatrix,
			retMat
		);
	}
	

	return m_ChildeWorldMatrix;
}

const _matrix CGameObject::GetTransformWorldMatrix()
{
	
	return m_pTransformCom->GetWorldMatrix();
}

_matrix CGameObject::GetChildWorldMatrix()
{
	return XMLoadFloat4x4(&m_ChildeWorldMatrix);
}

void CGameObject::RearrangeChildGrid(_float2 Padding, _float2 Space, _float2 CellSize, _uint NumColumns)
{
	RearrangeUISystem::RearrangeGridUI(shared_from_this(), m_Children, Padding, Space, CellSize, NumColumns);
}

void CGameObject::ResetTexture(shared_ptr<CTexture> NewTexture)
{
	m_pTextureCom = NewTexture;
}

void CGameObject::WriteJsonFile(json& j)
{
	/*게임용*/
	/*if (! IsForTool &&HasParent())
	{
		j["SRT_MATRIX"] =(m_ChildeWorldMatrix);
	}
	else*/
	/*툴베이스*/
	{
		j["SRT_MATRIX"]= *(m_pTransformCom->GetWorldMatrixPtr());
	}
	j["OBJECT_TAG"]	 =	WBCS2MBCS(m_ProtoObjTag);
	j["TEXTURE_TAG"] =  WBCS2MBCS(m_PtrotoTextureTag);
	j["SHADER_TAG"]  =	WBCS2MBCS(m_ProtoShaderTag);
	j["BUFFER_TAG"]  =	WBCS2MBCS(m_ProtoBufferTag);
	

	j["OBJECT_LEVEL"]	=	m_ProtoObjLevel;
	j["TEXTURE_LEVEL"]	=	m_ProtoTextureLevel;
	j["SHADER_LEVEL"]	=	m_ProtoShaderLevel;
	j["BUFFER_LEVEL"]	=	m_ProtoBufferLevel;
	j["ATLASINFO"]		=	m_AtlasInfo;

	
	json Jsonchildren = json::array();
	for (auto Child : m_Children)
	{
		json Node = {};
		Child->WriteJsonFile(Node);
		Jsonchildren.push_back(Node);
	}
	j["CHILDREN"] = std::move(Jsonchildren);
	return; 
}

void CGameObject::ReconstructFromJson(json& j)
{
}

HRESULT CGameObject::MakeComponentsFromTagInToolLevel()
{
	/*Todo Shader*/
	shared_ptr<CComponent> pComponent = {};
	pComponent = dynamic_pointer_cast<CComponent>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COM, m_ProtoShaderLevel, m_ProtoShaderTag, nullptr));
	m_Components[L"SHADER.COM"] = pComponent;
	m_pShaderCom = static_pointer_cast<CShader>(pComponent);

	pComponent = dynamic_pointer_cast<CComponent>( m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COM, m_ProtoBufferLevel, m_ProtoBufferTag, nullptr));
	m_Components[L"BUFFER.COM"] = pComponent;
	m_pBufferCom = dynamic_pointer_cast<CVIBuffer>(pComponent);

	pComponent = dynamic_pointer_cast<CComponent>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COM, 2,m_PtrotoTextureTag, nullptr));//차피 튤용이니가 상수 박자( 저장은 해야되니까
	m_Components[L"TEXTURE.COM"] = pComponent;
	m_pTextureCom = dynamic_pointer_cast<CTexture>(pComponent);

	/*DebugLevel*///생성했으므로 교체할 필요없음

	return S_OK;
}

HRESULT CGameObject::MakeComponentsFromTagInAnyLevel()
{
	shared_ptr<CComponent> pComponent = {};
	pComponent = dynamic_pointer_cast<CComponent>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COM, m_ProtoShaderLevel, m_ProtoShaderTag, nullptr));
	m_Components[L"SHADER.COM"] = pComponent;
	m_pShaderCom = static_pointer_cast<CShader>(pComponent);

	pComponent = dynamic_pointer_cast<CComponent>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COM, m_ProtoBufferLevel, m_ProtoBufferTag, nullptr));
	m_Components[L"BUFFER.COM"] = pComponent;
	m_pBufferCom = dynamic_pointer_cast<CVIBuffer>(pComponent);

	pComponent = dynamic_pointer_cast<CComponent>(m_pGameInstance->Clone_Prototype(PROTOTYPE::PROTO_COM, m_ProtoTextureLevel, m_PtrotoTextureTag, nullptr));//차피 튤용이니가 상수 박자( 저장은 해야되니까
	m_Components[L"TEXTURE.COM"] = pComponent;
	m_pTextureCom = dynamic_pointer_cast<CTexture>(pComponent);
	return S_OK;
}

_wstring CGameObject::GetTag(COMPONENT_TAG Tag)
{

	switch(Tag)
	{
	case COMPONENT_TAG::TAG_OBJECT:
		return m_ProtoObjTag; 
	case COMPONENT_TAG::TAG_TEXTURE:
		return m_PtrotoTextureTag;
	case COMPONENT_TAG::TAG_BUFFER:
		return m_ProtoBufferTag;
	case COMPONENT_TAG::TAG_SHADER:
		return m_ProtoShaderTag;
	}
	return _wstring();
}

shared_ptr<CComponent> CGameObject::FindComponentByName(const _wstring Tag)
{
	auto iter = m_Components.find(Tag);
	if (m_Components.end() != iter)
		return nullptr ;
	
	//태그를 통해 포인트 비교?
	
	return (iter->second);
}

shared_ptr<CGameObject> CGameObject::FindChildByIndex(_uint Index)
{
	/*따라다니는 텍스처나 펫  혹은 무기*/
	if (m_Children.size() <= Index) { 
		MSG_BOX("Out Of Indrx : InGameObject for childen Object");
		return nullptr;
	}
	return m_Children[Index]; 

}

shared_ptr<CGameObject> CGameObject::FindChildOnMouse()
{	
	if (m_Children.empty()) return shared_from_this();
	/*종료 조건 */
	shared_ptr<CGameObject> ret = {}; 
	for (auto& Child : m_Children)
	{
		/*자식 조건*/
		if(Child->IsMouseInDomain())
			ret =Child->FindChildOnMouse();
	}
	if (nullptr == ret)
	{
		/* 나 조건*/
		if (IsMouseInDomain())
			ret = shared_from_this();
	}

	return ret;
}
