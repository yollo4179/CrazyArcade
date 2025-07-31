#include "pch.h"
#include "LevelMapTool.h"
#include"GameInstance.h"
#include"LevelLogin.h"
#ifdef _DEBUG

CLevelMapTool::CLevelMapTool(const ComPtr<ID3D11Device>& pDevice, const ComPtr<ID3D11DeviceContext>& pContext)
	:CLevel{pDevice, pContext}
{
}

CLevelMapTool::~CLevelMapTool()
{
}

void CLevelMapTool::CopyTexture()
{
	/*Login*/
	AddTextureToContaner(L"PROTO.TEXTURE.UI_LOGIN");
	/*Room*/
	AddTextureToContaner(L"PROTO.TEXTURE.ROOM");

	AddTextureToContaner(L"PROTO.TEXTURE.PLAYER_SELECT");
	AddTextureToContaner(L"PROTO.TEXTURE.SELECT_BG");
	AddTextureToContaner(L"PROTO.TEXTURE.START_BUTTON");

	
	AddTextureToContaner(L"PROTO.TEXTURE.PLAYER_BG");
	AddTextureToContaner(L"PROTO.TEXTURE.PLAYER_FRAME");


	AddTextureToContaner(L"PROTO.TEXTURE.CHAT_BG");
	AddTextureToContaner(L"PROTO.TEXTURE.CHAT_SCROLL");

	AddTextureToContaner(L"PROTO.TEXTURE.PLAYER_IN_FRAME");

	m_Textures.emplace(L"None", nullptr);

}

HRESULT CLevelMapTool::Initialize()
{
	CopyTexture();
	RegisterTagLevelPair();
	return S_OK;
}

void CLevelMapTool::Update(_float fTimeDelta)
{
	ManageObjects();
	ManageTextures();
	
}

void CLevelMapTool::UpdateDemo()
{

	
}

shared_ptr<CToolObject> CLevelMapTool::CreateObject()
{

		shared_ptr<CGameObject> pUIObject;
		CToolObject::TOOLOBJECTDESC tDesc = { GetToolObjectDesc()};
		
		/*Add GameObject To Layer */
		m_pGameInstance.lock()->AddCopyObjectToLayer((_uint)LEVEL_ID::LEVEL_MAPTOOL, L"PROTO.OBJECT.TOOLOBJECT", (_uint)LEVEL_ID::LEVEL_MAPTOOL, L"MAPTOOLOBJECT", &tDesc, &pUIObject);
		m_NowObject = dynamic_pointer_cast<CToolObject>(pUIObject);

		return m_NowObject;
}

void CLevelMapTool::CreateObjectButton()
{
	if (ImGui::Button("오브젝트 생성")) {
		vecObjects.push_back(dynamic_pointer_cast<CToolObject>(CreateObject()));
	}

}

void CLevelMapTool::DisplayAllObjects()
{
	for (auto Object : vecObjects) {
			Object->Inspector();
	}
}

void CLevelMapTool::DisplayNowObject()
{

	if (m_NowObject.get()) {
		ImGui::Text("현재 가리키고 있는 오브젝트");
		ImGui::Separator();
		ImGui::BeginChild("NowObject");
		m_NowObject->Inspector();
		ImGui::Separator();
		/*컴포넌트의 터입과 레벨을 입력*/

		ImGui::EndChild();
	}
}

void CLevelMapTool::DeleteNowObject()
{
	if (ImGui::Button("현재 석택된 오브젝트 삭제")) {
		if (m_NowObject.get())
		{
			/*ObjectManager에서 삭제*/
			m_pGameInstance.lock()->DeleteGameObjectFromLayer((_uint)LEVEL_ID::LEVEL_MAPTOOL, L"MAPTOOLOBJECT", m_NowObject->GetName());
			/*UIManager에서 삭제*/
			m_pGameInstance.lock()->DeleteUIObjectByName(m_NowObject->GetName());
			/*Vector에서 삭제 */
			auto iter = vecObjects.begin();
			for (; iter != vecObjects.begin(); ++iter)
			{
				if ((*iter).get() == m_NowObject.get())
					break;
			}
			vecObjects.erase(iter);

			m_NowObject.reset();
			/*나 삭제 */
		}
	}

}

void CLevelMapTool::AddChildToNowObject()
{
	
	if(ImGui::Button("현재 객체에 자식 추가하기"))
	if (m_NowObject.get())
	{
		CToolObject::TOOLOBJECTDESC tDesc{ GetToolObjectDesc() };
		shared_ptr<CGameObject> pChildPtr = dynamic_pointer_cast<CGameObject>(
			m_pGameInstance.lock()->Clone_Prototype(
			PROTOTYPE::PROTO_OBJ,
			(_uint)LEVEL_ID::LEVEL_MAPTOOL,
			L"PROTO.OBJECT.TOOLOBJECT",
			&tDesc
		)
		);
		m_NowObject->AddChildObject(pChildPtr);


	}
}

void CLevelMapTool::ReArrangeChildrenOf()
{
	if (ImGui::TreeNode("현재 오브젝트 자식 정렬")) {
		if (m_NowObject) {
			ImGui::Text("현재 가리키고 있는 오브젝트의 자식들을 정렬합니다.");
			ImGui::Separator();
			static _float2 Space = { 1.f,1.f };
			static _float2 Padding = { 1.f,1.f };
			static _uint Colunms = { 1 };
			static  _float2 CellSize = { 10.f,10.f };

			ImGui::SliderFloat2("패딩", (_float*)(&Padding), 0.f, 300.f);
			ImGui::SliderFloat2("사이 공간", (_float*)(&Space), 0.f, 300.f);
			ImGui::SliderInt("열 개수", (_int*)&Colunms, 1, 10);
			ImGui::SliderFloat2("셀의 크기", (_float*)(&CellSize), 0.1f, 1.f);
			if(ImGui::Button("정렬"))
				m_NowObject->RearrangeChildGrid(Padding, Space, CellSize, Colunms);
		}
		ImGui::TreePop(); 
	}
}

void CLevelMapTool::CloneNowObjectButton()
{
	if (ImGui::Button("현재 오브젝트 복사해서 부모에 넣기"))
	{

		if (m_NowObject)
		{
			CToolObject::TOOLOBJECTDESC tDesc = { GetToolObjectDesc() };
			shared_ptr<CGameObject> pNewObject;
			m_NowObject->GetCloneObjectFromThis<CToolObject>(pNewObject,&tDesc);
			if (!m_NowObject->HasParent())
			{
				/*부모가 없으면 내가 관리*/
				m_pGameInstance.lock()->AddCopyObjectToLayer((_uint)LEVEL_ID::LEVEL_MAPTOOL, L"PROTO.OBJECT.TOOLOBJECT", (_uint)LEVEL_ID::LEVEL_MAPTOOL, L"MAPTOOLOBJECT",dynamic_pointer_cast<CGameObject>(pNewObject) );
				vecObjects.push_back(dynamic_pointer_cast<CToolObject>(pNewObject));
				/*채팅도 아니고 툴인데 뭐 ..*/
				
			}
			else
			{
				m_NowObject->GetParent().lock()->AddChildObject(pNewObject);
			}
			pNewObject->GetTransform()->SetWorld(m_NowObject->GetTransformWorldMatrix());
			m_NowObject =dynamic_pointer_cast<CToolObject>(pNewObject);
		}

	}
}

CToolObject::TOOLOBJECTDESC  CLevelMapTool::GetToolObjectDesc()
{

	CToolObject::TOOLOBJECTDESC tDesc = {};
	tDesc.fRotationPerSec = XMConvertToRadians(90.F);
	tDesc.fSpeedPerSec = 10.f;
	tDesc.ObjectPosition = { 0.f,0.f,1.f };
	tDesc.ObjectSize = { 100,100,1.f };
	tDesc._ZOrder = 10;
	tDesc._bSelected = false;
	tDesc._UIMode = UIMODE::UI_MOVABLE;
	tDesc._UIState = UISTATE::UI_RELEASED;

	return tDesc;
}

void CLevelMapTool::LoadObject()
{
	static char jsonPath[MAX_PATH] = "../Resources/Json/xxx.json";

	ImGui::InputText("불러올 객체의 json파일 주소",jsonPath,sizeof(char)*MAX_PATH);
	if(ImGui::Button("객체 불러오기"))
	{
		json ObjectInfo = {};
		ifstream In; 

		try {
			
			In.exceptions(std::ofstream::failbit | std::ofstream::badbit);
			In.open(jsonPath);
			In >> ObjectInfo;
		}
		catch (const std::ios_base::failure& e) {
			MSG_BOX("저장 에러 발생");
			e;
		}
		In.close();
		/*객체 생성*/
		shared_ptr<CGameObject> pUIObject;
		CToolObject::TOOLOBJECTDESC tDesc = { GetToolObjectDesc() };
		m_pGameInstance.lock()->AddCopyObjectToLayer((_uint)LEVEL_ID::LEVEL_MAPTOOL, L"PROTO.OBJECT.TOOLOBJECT", (_uint)LEVEL_ID::LEVEL_MAPTOOL, L"MAPTOOLOBJECT", &tDesc, &pUIObject);
		vecObjects.push_back(dynamic_pointer_cast<CToolObject>(pUIObject));
		m_NowObject = dynamic_pointer_cast<CToolObject>(pUIObject);
		ObjectBuilder(ObjectInfo, pUIObject);
	}


}



void CLevelMapTool::ObjectBuilder(json& node, shared_ptr<CGameObject>pGameObject)
{
	XMFLOAT4X4 matSRT = {};
	string ObjTag = {}; 
	string	TextureTag = {};
	string ShaderTag = {};
	string BufferTag = {};
	_uint ObjLevel = {}; 
	_uint TextureLevel = {};
	_uint ShaderLevel = {};
	_uint BufferLevel = {};
	_float4 AtlasInfo = {};

	node.at("SRT_MATRIX").get_to(matSRT);

	node.at("OBJECT_TAG").get_to(ObjTag);
	node.at("OBJECT_LEVEL").get_to(ObjLevel);

	node.at("TEXTURE_TAG").get_to(TextureTag);
	node.at("TEXTURE_LEVEL").get_to(TextureLevel);
	node.at("SHADER_TAG").get_to(ShaderTag);
	node.at("SHADER_LEVEL").get_to(BufferLevel);
	node.at("BUFFER_TAG").get_to(BufferTag);
	node.at("BUFFER_LEVEL").get_to(BufferLevel);
	node.at("ATLASINFO").get_to(AtlasInfo);
	pGameObject->SetAtlasInfo(AtlasInfo);
	

	_wstring wTexTag = MBCS2WBCS(TextureTag);
	_wstring wBufTag = MBCS2WBCS(BufferTag);
	_wstring wShaTag = MBCS2WBCS(ShaderTag);
	_wstring wObjTag = MBCS2WBCS(ObjTag);
	
	pGameObject->SetProtoObjTagInfo(wObjTag, ObjLevel);
	pGameObject->SetProtoTextureInfo(wTexTag, TextureLevel);
	pGameObject->SetProtoBufferInfo(wBufTag, BufferLevel);
	pGameObject->SetProtoShaderInfo(wShaTag, ShaderLevel);
	pGameObject->MakeComponentsFromTagInToolLevel();
	pGameObject->GetTransform()->SetWorld(XMLoadFloat4x4(&matSRT));
	
	

	if (node.contains("CHILDREN") && node["CHILDREN"].is_array())
	{

		for (json& child : node["CHILDREN"])
		{
			CToolObject::TOOLOBJECTDESC tDesc = { GetToolObjectDesc() };
			/*오브젝트 먼저 생성*/
			shared_ptr<CGameObject> pChild = dynamic_pointer_cast<CGameObject>(m_pGameInstance.lock()->Clone_Prototype(PROTOTYPE::PROTO_OBJ, (_uint)LEVEL_ID::LEVEL_MAPTOOL, L"PROTO.OBJECT.TOOLOBJECT", &tDesc));
			pGameObject->AddChildObject(pChild); //자식으로 추가해놓고 다음에 바꾼다 . 
			ObjectBuilder(child, pChild);
			
			
		}
	}
}

void CLevelMapTool::TagLevelPairMapper()
{
	ShowComponentList("Object TagMapper", m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_OBJECT], m_NowObjectKey, COMPONENT_TAG::TAG_OBJECT);
	ShowComponentList("Texture TagMapper",m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_TEXTURE],m_NowTextureKey,COMPONENT_TAG::TAG_TEXTURE);
	ShowComponentList("Buffer TagMapper", m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_BUFFER],m_NowBufferKey, COMPONENT_TAG::TAG_BUFFER);
	ShowComponentList("Shader TagMapper", m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_SHADER],m_NowShaderKey, COMPONENT_TAG::TAG_SHADER);


}
void CLevelMapTool::RegisterTagLevelPair()
{
	/*셰이더*/
	m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_SHADER].emplace(L"PROTO.SHADER.RECT", (_uint)LEVEL_ID::LEVEL_STATIC);
	m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_SHADER].emplace(L"PROTO.SHADER.RECTPOINT", (_uint)LEVEL_ID::LEVEL_STATIC);
	/*버퍼*/
	m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_BUFFER].emplace(L"PROTO.BUFFER.RECT", (_uint)LEVEL_ID::LEVEL_STATIC);
	m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_BUFFER].emplace(L"PROTO.BUFFER.RECTPOINT", (_uint)LEVEL_ID::LEVEL_STATIC);
	m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_BUFFER].emplace(L"PROTO.BUFFER.DEBUGLINE", (_uint)LEVEL_ID::LEVEL_STATIC);

	/*텍스처*/
	/*Login*/
	m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_TEXTURE].emplace(L"PROTO.TEXTURE.UI_LOGIN", (_uint)LEVEL_ID::LEVEL_LOGIN);
	
	/*Room*/
	m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_TEXTURE].emplace(L"PROTO.TEXTURE.PLAYER_SELECT", (_uint)LEVEL_ID::LEVEL_ROOM);
	m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_TEXTURE].emplace(L"PROTO.TEXTURE.SELECT_BG", (_uint)LEVEL_ID::LEVEL_ROOM);
	m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_TEXTURE].emplace(L"PROTO.TEXTURE.START_BUTTON", (_uint)LEVEL_ID::LEVEL_ROOM);
	m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_TEXTURE].emplace(L"PROTO.TEXTURE.ROOM", (_uint)LEVEL_ID::LEVEL_ROOM);
	m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_TEXTURE].emplace(L"PROTO.TEXTURE.PLAYER_FRAME", (_uint)LEVEL_ID::LEVEL_ROOM);
	m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_TEXTURE].emplace(L"PROTO.TEXTURE.PLAYER_IN_FRAME", (_uint)LEVEL_ID::LEVEL_ROOM);
	m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_TEXTURE].emplace(L"PROTO.TEXTURE.PLAYER_BG", (_uint)LEVEL_ID::LEVEL_ROOM);
	m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_TEXTURE].emplace(L"PROTO.TEXTURE.CHAT_BG", (_uint)LEVEL_ID::LEVEL_ROOM);
	m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_TEXTURE].emplace(L"PROTO.TEXTURE.CHAT_SCROLL", (_uint)LEVEL_ID::LEVEL_ROOM);
	m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_TEXTURE].emplace(L"None", (_uint)LEVEL_ID::LEVEL_STATIC);




	/*Object*/
	m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_OBJECT].emplace(L"PROTO.OBJECT.LOGIN.BG", (_uint)LEVEL_ID::LEVEL_LOGIN);
	m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_OBJECT].emplace(L"PROTO.OBJECT.LOGIN.PANNEL", (_uint)LEVEL_ID::LEVEL_LOGIN);
	m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_OBJECT].emplace(L"PROTO.OBJECT.LOGIN.BUTTON", (_uint)LEVEL_ID::LEVEL_LOGIN);
	m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_OBJECT].emplace(L"PROTO.OBJECT.LOGIN.TEXT_ID", (_uint)LEVEL_ID::LEVEL_LOGIN);
	m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_OBJECT].emplace(L"PROTO.OBJECT.LOGIN.TEXT_PASSWORD", (_uint)LEVEL_ID::LEVEL_LOGIN);

	/*ROOM*/
	m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_OBJECT].emplace(L"PROTO.OBJECT.ROOM.BG", (_uint)LEVEL_ID::LEVEL_ROOM);
	m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_OBJECT].emplace(L"PROTO.OBJECT.ROOM.PLAYER_BG", (_uint)LEVEL_ID::LEVEL_ROOM);
	m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_OBJECT].emplace(L"PROTO.OBJECT.ROOM.PLAYER_FRAME", (_uint)LEVEL_ID::LEVEL_ROOM);
	m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_OBJECT].emplace(L"PROTO.OBJECT.ROOM.PLAYER_IN_FRAME", (_uint)LEVEL_ID::LEVEL_ROOM);
	m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_OBJECT].emplace(L"PROTO.OBJECT.ROOM.PLAYER_NAME_IN_FRAME", (_uint)LEVEL_ID::LEVEL_ROOM);

	m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_OBJECT].emplace(L"PROTO.OBJECT.ROOM.SELECT_BUTTON_BG", (_uint)LEVEL_ID::LEVEL_ROOM);
	m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_OBJECT].emplace(L"PROTO.OBJECT.ROOM.SELECT_BUTTON", (_uint)LEVEL_ID::LEVEL_ROOM);
	m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_OBJECT].emplace(L"PROTO.OBJECT.ROOM.CHAT_BG", (_uint)LEVEL_ID::LEVEL_ROOM);
	m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_OBJECT].emplace(L"PROTO.OBJECT.ROOM.CHAT_SCROLL", (_uint)LEVEL_ID::LEVEL_ROOM);
	m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_OBJECT].emplace(L"PROTO.OBJECT.ROOM.CHAT_SCROLL_Range", (_uint)LEVEL_ID::LEVEL_ROOM);
	m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_OBJECT].emplace(L"PROTO.OBJECT.ROOM.CHAT_INPUTBOX", (_uint)LEVEL_ID::LEVEL_ROOM);
	m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_OBJECT].emplace(L"PROTO.OBJECT.ROOM.CHAT_CHATTINGBOX", (_uint)LEVEL_ID::LEVEL_ROOM);


	m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_OBJECT].emplace(L"PROTO.OBJECT.ROOM.SELECT_PANNEL", (_uint)LEVEL_ID::LEVEL_ROOM);
	m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_OBJECT].emplace(L"PROTO.OBJECT.ROOM.SELECT_BUTTON", (_uint)LEVEL_ID::LEVEL_ROOM);




}

void CLevelMapTool::ShowComponentList(const string& ListName, map<_wstring, _uint>& TagMapper,  _wstring& Key,COMPONENT_TAG Tag)
{
	_bool isSelected = false;
	if (ImGui::BeginCombo(ListName.c_str(),(Key.size()>0)? WBCS2MBCS(Key).c_str():(m_NowObject!=nullptr&& m_NowObject->GetTag(Tag).size())? WBCS2MBCS(m_NowObject->GetTag(Tag)).c_str() : WBCS2MBCS(Key).c_str()))
	{
		for (auto Tag : TagMapper)
		{

			if (ImGui::Selectable(
				m_pGameInstance.lock()->Wbcs2Mbcs(Tag.first).c_str(),
				isSelected
			))
			{
				Key = Tag.first;
			}

			if (isSelected)                                       // 기본 포커스
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
}


void CLevelMapTool::DecideTagInfoButton()
{
	if (ImGui::Button("현재 태그정보 현재 객체에 저장"))
	{
		if (m_NowObject)
		{
			m_NowObject->SetProtoObjTagInfo(m_NowObjectKey, m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_OBJECT].find(m_NowObjectKey)->second);
			m_NowObject->SetProtoBufferInfo(m_NowBufferKey , m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_BUFFER].find(m_NowBufferKey)->second);
			m_NowObject->SetProtoShaderInfo(m_NowShaderKey, m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_SHADER].find(m_NowShaderKey)->second);
			m_NowObject->SetProtoTextureInfo(m_NowTextureKey, m_TagLevelPairs[(_uint)COMPONENT_TYPE::TYPE_TEXTURE].find(m_NowTextureKey)->second);
			m_NowTexture = m_Textures[m_NowTextureKey];
			m_NowObject->SetTexture(m_Textures.find(m_NowTextureKey)->second);


		}
	}

}

void CLevelMapTool::ManageTextures()
{
	ImGui::Begin("텍스처 관리하기");
	ShowTextureListBox();
	ShowSelectedTexture();

	ImGui::End(); 

}

void CLevelMapTool::AddTextureToContaner(_wstring ProtoTag)
{
	m_Textures.emplace(ProtoTag,
		shared_ptr<CTexture>(dynamic_pointer_cast<CTexture>(m_pGameInstance.lock()->Clone_Prototype(
			PROTOTYPE::PROTO_COM,
			(_uint)LEVEL_ID::LEVEL_MAPTOOL,
			ProtoTag,
			nullptr
		)))
	);
}

void CLevelMapTool::ShowSelectedTexture()
{
	if (m_NowTexture)
	{
		m_NowTexture->Inspector();
	}
}

void CLevelMapTool::SetNowTexture2NowObject()
{
	if (ImGui::Button("현재 오브젝트 텍스처 세팅"))
	{
		m_NowObject->SetTexture(m_NowTexture);
		/*m_NowObject->SetProtoTextureInfo(m_NowTexture->GetName());*/
	}
}

void CLevelMapTool::ShowTextureListBox()
{
	_bool isSelected = false;  // 선택값을 바인딩하려면 배열/벡터 대신 콤보처럼 사용

	if (ImGui::BeginListBox("TexturesList",
		ImVec2(-FLT_MIN,   // 너비: 부모에 맞춤
			8 * ImGui::GetTextLineHeightWithSpacing()))) // 높이: 8행
	{
		for (auto Tex : m_Textures)
		{

			if (ImGui::Selectable(
				WBCS2MBCS(Tex.first).c_str(),
				isSelected
				))

			{
				m_NowTexture = Tex.second;
				m_NowTextureKey = Tex.first;
			}


		}
		ImGui::EndListBox();
	}
}

void CLevelMapTool::ManageObjects()
{
	/*이동중일때만  (이전에 가리키던 객체가 다를 때 혹은  null이었다가 새로 생길때 ) */
	if (nullptr != m_pGameInstance.lock()->GetSelectedObject().get() )
		m_NowObject = dynamic_pointer_cast<CToolObject>(m_pGameInstance.lock()->GetSelectedObject());


	ImGui::Begin("오브젝트 매니지먼트");
	CreateObjectButton();
	AddChildToNowObject();
	SetNowTexture2NowObject();
	TagLevelPairMapper();
	DecideTagInfoButton();
	DeleteNowObject();
	ReArrangeChildrenOf();
	CloneNowObjectButton();
	LoadObject();
	
	DisplayNowObject();
	DisplayAllObjects();
	//DisplayAllObjects();
	

	ImGui::End();


}

CLevelMapTool* CLevelMapTool::Create(const ComPtr<ID3D11Device>& pDevice,const  ComPtr<ID3D11DeviceContext>& pContext)
{
	CLevelMapTool* pInstance = new CLevelMapTool(pDevice, pContext);
	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created LevelMapTool");
		Safe_Delete(pInstance);
	}
	return pInstance;
}
#endif // DEBUG