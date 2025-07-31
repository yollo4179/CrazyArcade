#include"pch.h"
#include "ObjectBuilder.h"
#include"UIObject.h"
namespace Client {
	void ObjectBuilderSystem::BuildObject(json NODE, shared_ptr<CGameObject> NodePtr)
	{
		static weak_ptr<CGameInstance> m_pGameInstance{ CGameInstance::GetInstance() };
		//객체 만들때 부르므로 생성시점 고려 x ,  전역변수 함수화 했고 , 연관 객체 인자가 없으므로 고려 x 
		/*Object Tag*/
		string	ProtoObjectTag = {};
		_uint		ProtoObjectLevel = {};
		string		ProtoBufferTag = {};
		_uint		ProtoBufferLevel = {};
		string		ProtoTextureTag = {};
		_uint		ProtoTextureLevel = {};
		string		ProtoShaderTag = {};
		_uint		ProtoShaderLevel = {};
		_float4x4	SRTMatrix = {};
		_float4		AtlasInfo = {};

		NODE["SRT_MATRIX"].get_to(SRTMatrix);

		NODE["OBJECT_TAG"].get_to(ProtoObjectTag);
		NODE["OBJECT_LEVEL"].get_to(ProtoObjectLevel);

		NODE["TEXTURE_TAG"].get_to(ProtoTextureTag);
		NODE["TEXTURE_LEVEL"].get_to(ProtoTextureLevel);

		NODE["SHADER_TAG"].get_to(ProtoShaderTag);
		NODE["SHADER_LEVEL"].get_to(ProtoShaderLevel);

		NODE["BUFFER_TAG"].get_to(ProtoBufferTag);
		NODE["BUFFER_LEVEL"].get_to(ProtoBufferLevel);

		NODE["ATLASINFO"].get_to(AtlasInfo);


		_wstring wObjTag = MBCS2WBCS(ProtoObjectTag);
		_wstring wTexTag = MBCS2WBCS(ProtoTextureTag);
		_wstring wBufTag = MBCS2WBCS(ProtoBufferTag);
		_wstring wShaTag = MBCS2WBCS(ProtoShaderTag);

		NodePtr->SetProtoObjTagInfo(wObjTag, ProtoObjectLevel);
		NodePtr->SetProtoTextureInfo(wTexTag, ProtoTextureLevel);
		NodePtr->SetProtoBufferInfo(wBufTag, ProtoBufferLevel);
		NodePtr->SetProtoShaderInfo(wShaTag, ProtoShaderLevel);
		NodePtr->MakeComponentsFromTagInAnyLevel();
		NodePtr->SetAtlasInfo(AtlasInfo);

		CUIObject::UIOBJECT_DESC  tDesc = {};
		tDesc.fRotationPerSec = XMConvertToRadians(90.F);
		tDesc.fSpeedPerSec = 10.f;
		tDesc.ObjectPosition = { 0.f,0.f,1.f };
		tDesc.ObjectSize = { g_iWinSizeX,g_iWinSizeY,0.f };
		tDesc._ZOrder = 0;
		tDesc._bSelected = false;
		tDesc._UIMode = UIMODE::UI_FIXED;
		tDesc._UIState = UISTATE::UI_RELEASED;
		/*부모 따로 처리*/
		if (!NodePtr->HasParent())
		{
			/*컴포넌트는 공통 사항 -> 객체 만들고 컴포넌트 만든 후에 매트릭스 설정*/
			NodePtr->GetTransform()->SetWorld(XMLoadFloat4x4(&SRTMatrix));

		}

		//Children노드가 존재하고 배열인 경우에만 ( 자식이 존재할때만 수행 
		if (NODE.contains("CHILDREN") && NODE["CHILDREN"].is_array())
		{

			for (json& child : NODE["CHILDREN"])
			{
				string ObjectTag = {};
				_uint ObjectLevel = {};
				_float4x4 ChildMatrix = {};
				child["OBJECT_TAG"].get_to(ObjectTag);
				child["OBJECT_LEVEL"].get_to(ObjectLevel);

				shared_ptr<CGameObject> pChildNodePtr;
				child["SRT_MATRIX"].get_to(ChildMatrix);

				pChildNodePtr = dynamic_pointer_cast<CGameObject>(m_pGameInstance.lock()->Clone_Prototype(PROTOTYPE::PROTO_OBJ, ObjectLevel, MBCS2WBCS(ObjectTag), &tDesc));
				
				NodePtr->AddChildObject(pChildNodePtr);
				BuildObject(child, pChildNodePtr);
				pChildNodePtr->GetTransform()->SetWorld(XMLoadFloat4x4(&ChildMatrix));
				pChildNodePtr->ComputeWorldMatrixAsChild();
			}

		}


	}



}