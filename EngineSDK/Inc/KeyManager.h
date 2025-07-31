#pragma once
#include"Engine_Defines.h"
#include"Base.h"
BEGIN(Engine)

enum class INPUT_STATE
{
	UP,
	DOWN,
	PRESSED,
	RELEASED,
	INPUT_END
};

enum  class MOUSE
{
	LB,
	RB,
	MB,
	MOUSE_END,
};

enum  class MOUSEMOVE
{
	X,
	Y,
	Z,
	MOUSEMOVE_END,
};
 enum class KEY
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	NUM1,
	NUM2,
	NUM3,
	NUM4,
	NUM5,
	NUM6,
	NUM7,
	NUM8,
	NUM9,
	NUM0,
	NUMPAD8,
	NUMPAD4,
	NUMPAD5,
	NUMPAD6,
	NUMPAD2,
	Q,
	W,
	E,
	R,
	T,
	Y,
	U,
	H,
	J,
	K,
	M,
	I,
	O,
	P,
	A,
	S,
	D,
	F,
	G,
	Z,
	X,
	C,
	V,
	B,
	ALT,
	CTRL,
	LSHIFT,
	SPACE,
	ENTER,
	ESC,
	TAB,
	BACK_SPACE,
	KEY_END, 
};
struct INPUTSTATE_INFO
{
	INPUT_STATE eState = { INPUT_STATE ::RELEASED};
	_bool PrevPushed = {false};
};



class CKeyManager final :public CBase
{

private:
		CKeyManager();
public:
		virtual ~CKeyManager() ;
public:
	HRESULT Initialize(HINSTANCE hInst, HWND hWnd,_float2 ViewportSize);
	void Update(); //입력 상태를 업데이트한다. 
	void Update_KeyState();
	void Update_MouseState();
public:
	INPUT_STATE GetKeyState(KEY eKey) { return m_KeyStates[(_uint)eKey].eState; };
	INPUT_STATE GetMouseState(MOUSE eKey) { return m_MouseStates[(_uint)eKey].eState; };
	_long GetMouseDIInput(MOUSEMOVE eMouseMove)
	{
		return *( (_long*)(&m_tMouseState) + (int)eMouseMove); //마우스 상채 그대로 가져오기 
	}
	POINT	GetMousePos()
	{
		
		RECT rcClient;
		GetClientRect(m_hWnd, &rcClient);
		float windowW = rcClient.right - rcClient.left;   // 클라이언트 가로
		float windowH = rcClient.bottom - rcClient.top; 
		
		float backW = m_ViewportSize.x;
		float backH = m_ViewportSize.y;


		POINT tPos;
		GetCursorPos(&tPos);
		ScreenToClient(m_hWnd, &tPos);
		tPos.x = tPos.x * backW / windowW;
		tPos.y = tPos.y * backH / windowH;
		return tPos;
	}
	
	_bool		CheckMousePos(RECT _tRect);


private:
	std::vector<INPUTSTATE_INFO>m_KeyStates;
	std::vector<INPUTSTATE_INFO>m_MouseStates;
	
	LPDIRECTINPUT8		m_pInputSDK = nullptr;
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse = nullptr;
	BYTE							m_byKeyState[256];		// 키보드에 있는 모든 키값을 저장하기 위한 변수
	DIMOUSESTATE2					m_tMouseState;	//마우스 상태를 저장하기 위한 변수
	HWND			m_hWnd;
	HINSTANCE		m_hInst;
	_float2			m_ViewportSize{};
private:
	_float		m_fAxis[2] = { 0.f, 0.f };
	_float		m_fACel{};
private:
	bool m_isPrevFocused{};

public:
	static CKeyManager* Create(HINSTANCE _hInst, HWND _hWnd , _float2 ViewPortSize);
	

};
END
