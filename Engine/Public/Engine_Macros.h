#pragma once
#ifndef Engine_Macro_h__
#define Engine_Macro_h__
namespace Engine
{
#pragma region 	NameSpace
#define BEGIN(NameSpace) namespace NameSpace {
#define END }
#define USING(NameSpace) using NameSpace;
#pragma endregion 	NameSpace


#ifndef			MSG_BOX
#define			MSG_BOX(_message)			MessageBox(NULL, TEXT(_message), L"System Message", MB_OK)
#endif

#pragma region 	DLL_EXPORT _엔진 클래스에 추가->클라사용
#ifdef ENGINE_EXPORTS
#define ENGINE_DLL	_declspec(dllexport)
#else
#define	ENGINE_DLL _declspec(dllimport)
#endif
#pragma region 	DLL_EXPORT _엔진 클래스에 추가

#define NO_COPY(CLASSNAME)											\
			private:												\
			CLASSNAME(const CLASSNAME&) = delete;					\
			CLASSNAME& operator = (const CLASSNAME&) = delete;		

#define DECLARE_SINGLETON(CLASSNAME)								\
			NO_COPY(CLASSNAME)										\
			private:												\
			static shared_ptr<CLASSNAME>	m_pInstance;						\
			public:													\
			static shared_ptr<CLASSNAME>	GetInstance( void );	\
			static unsigned int DestroyInstance( void );			

#define IMPLEMENT_SINGLETON(CLASSNAME)								\
			shared_ptr<CLASSNAME>	CLASSNAME::m_pInstance;			\
			shared_ptr<CLASSNAME>	CLASSNAME::GetInstance( void )	{			\
			if(nullptr == m_pInstance.get()) {							\
				m_pInstance = std::make_shared<CLASSNAME>();						\
				}													\
				return m_pInstance;									\
			}														\
			unsigned int CLASSNAME::DestroyInstance( void ) {		\
				unsigned int iRefCnt = {0};							\
				if(m_pInstance)m_pInstance.reset();					\
				return iRefCnt;										\
			}


}

#pragma region Gameinstance
#define GET_TIMEDELTA(TAG)	m_pGameInstance->GetTimeDelta(TAG)
#define ADD_TIMER(TAG)	m_pGameInstance->AddTimer(TAG)
#define FIND_TIMER(TAG) m_pGameInstance->FindTimer(TAG)
#define UPDATE_TIMER(TAG) m_pGameInstance->UpdateTimer(TAG)

#define VIEWPORT_WIDTH()  weak_ptr<CGameInstance>(m_pGameInstance).lock()->GetWidth()
#define VIEWPORT_HEIGHT()  weak_ptr<CGameInstance>(m_pGameInstance).lock()->GetHeight()


#define MBCS2WBCS(str)  weak_ptr<CGameInstance>(m_pGameInstance).lock()->Mbcs2Wbcs(str)
#define WBCS2MBCS(str)  weak_ptr<CGameInstance>(m_pGameInstance).lock()->Wbcs2Mbcs(str)
#pragma endregion GameInstance

#pragma region KeyManager

#define KEY_CHECK(key, state)  weak_ptr<CGameInstance>(m_pGameInstance).lock()->GetKeyState(key) == state
// 실제 사용은 아래 4개
#define KEY_PRESSED(key)       KEY_CHECK(key, INPUT_STATE::PRESSED)
#define KEY_DOWN(key)           KEY_CHECK(key, INPUT_STATE::DOWN)
#define KEY_UP(key)				KEY_CHECK(key, INPUT_STATE::UP)
#define KEY_RELEASED(key)           KEY_CHECK(key, INPUT_STATE::RELEASED)

#define MOUSE_CHECK(key, state)  weak_ptr<CGameInstance>(m_pGameInstance).lock()->GetMouseState(key) == state
	// 실제 사용은 아래 4개
#define MOUSE_PRESSED(key)		MOUSE_CHECK(key, INPUT_STATE::PRESSED)
#define MOUSE_DOWN(key)			MOUSE_CHECK(key, INPUT_STATE::DOWN)
#define MOUSE_UP(key)			MOUSE_CHECK(key, INPUT_STATE::UP)
#define MOUSE_RELEASED(key)			MOUSE_CHECK(key, INPUT_STATE::RELEASED)

#define MOUSE_MOVE(MouseMovement) weak_ptr<CGameInstance>(m_pGameInstance).lock()->GetMouseDIInput(MouseMovement)
#pragma endregion KeyManager

#endif