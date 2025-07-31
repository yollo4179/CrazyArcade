#pragma once
//#include<memory>
//#include<map>
//#include<string>
//#include<functional>
//#include"Base.h"
//#include"GameInstance.h"
//namespace AstractFactory
//{
//	/*만들고 제어권 넘겨주기 */
//	
//	//기본 버전 
//	template<typename T, typename ...Args>
//	std::unique_ptr<T> MakeProto() { return std::unique_ptr<T>(Args...); }
//
//	/*초기화 구조체 오버로딩 버전*/
//	template<typename T, typename ...Args>
//	std::unique_ptr<T> MakeCopy(void* pArg = nullptr) { return std::unique_ptr<T>(Args..., pArg); } 
//
//
//	std::map<_wstring, std::unique_ptr<CBase>(*)()> gFactory;
//	std::map<_wstring, std::unique_ptr<CBase>(*)(void*)> gCopyObjFactory;
//
//
//
//	template<typename T, typename ...Args>
//	void Register(const _wstring& Key,_uint ProtoLevelID, _wstring ProtoObjTag,_uint CurLevelID,_wstring LayerName,  Args... )
//	{
//		g_Factory.emplace(Key,[&]()
//		{
//			weak_ptr<CGameInstance>(m_pGameInstance).lock()->AddCopyObjectToLayer(ProtoLevelID, ProtoObjTag, LayerLevelID, LayerName, L"MAPTOOLOBJECT", Args...);
//		}
//		)
//	}
//
//
//
//	std::unique_ptr<CBase> GetInstance(_wstring Key,void * pArg)
//	{
//		auto iter = gCopyObjFactory.find(Key);
//		if (iter == gCopyObjFactory.end())return nullptr;
//		return iter->second(pArg);
//	}
//	std::unique_ptr<CBase> GetInstance(_wstring Key)
//	{
//		auto iter = gFactory.find(Key);
//		if (iter == gFactory.end())return nullptr;
//		return iter->second();
//	}
//
//
//}