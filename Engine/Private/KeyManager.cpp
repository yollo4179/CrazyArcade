
#include "KeyManager.h"
int g_arrKeys[(int)KEY::KEY_END] = {
    DIK_LEFT,
    DIK_RIGHT,
    DIK_UP,
    DIK_DOWN,
    DIK_1,
    DIK_2,
    DIK_3,
    DIK_4,
    DIK_5,
    DIK_6,
    DIK_7,
    DIK_8,
    DIK_9,
    DIK_0,
    DIK_NUMPAD8,
    DIK_NUMPAD4,
    DIK_NUMPAD5,
    DIK_NUMPAD6,
    DIK_NUMPAD2,
    DIK_Q,
    DIK_W,
    DIK_E,
    DIK_R,
    DIK_T,
    DIK_Y,
    DIK_U, // 
    DIK_H, // 추가
    DIK_J, // 추가
    DIK_K, // 추가
    DIK_M, // 추가
    DIK_I,
    DIK_O,
    DIK_P,
    DIK_A,
    DIK_S,
    DIK_D,
    DIK_F,
    DIK_G,
    DIK_Z,
    DIK_X,
    DIK_C,
    DIK_V,
    DIK_B,
    DIK_LALT,     // ALT,
    DIK_LCONTROL, // CTRL,
    DIK_LSHIFT,   // LSHIFT,
    DIK_SPACE,    // SPACE,
    DIK_RETURN,   // ENTER,
    DIK_ESCAPE,   // ESC,
    DIK_TAB,       // TAB,
    DIK_BACKSPACE 
};


CKeyManager::CKeyManager()
    : m_pInputSDK{},
    m_pKeyBoard{},
    m_pMouse{},
    m_hWnd{},
    m_hInst{},
    m_isPrevFocused(false)
{

    ZeroMemory(m_byKeyState, sizeof(m_byKeyState));
    ZeroMemory(&m_tMouseState, sizeof(m_tMouseState));
}


CKeyManager::~CKeyManager()
{

    if (m_pInputSDK)
	{
		m_pInputSDK->Release();
		m_pInputSDK = nullptr;
	}

	if (m_pKeyBoard)
	{
		m_pKeyBoard->Unacquire();
		m_pKeyBoard->Release();
		m_pKeyBoard = nullptr;
	}

	if (m_pMouse)
	{
		m_pMouse->Unacquire();
		m_pMouse->Release();
		m_pMouse = nullptr;
	}
}

HRESULT CKeyManager::Initialize(HINSTANCE hInst, HWND hWnd, _float2 ViewportSize)
{

    m_hWnd = hWnd;
    m_hInst = hInst;
    

    m_ViewportSize = ViewportSize;
    // DInput 컴객체를 생성하는 함수
    if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputSDK, NULL)))
        return E_FAIL;

    // 키보드 객체 생성
    if (FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
        return E_FAIL;

    // 생성된 키보드 객체의 대한 정보를 컴 객체에게 전달하는 함수
    m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

    // 장치에 대한 독점권을 설정해주는 함수, (클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수)
    m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

    // 장치에 대한 aCess 버전을 받아오는 함수
    m_pKeyBoard->Acquire();

    // 마우스 객체 생성
    if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
        return E_FAIL;


    // 생성된 마우스 객체의 대한 정보를 컴 객체에게 전달하는 함수
    //m_pMouse->SetDataFormat(&c_dfDIMouse);
    m_pMouse->SetDataFormat(&c_dfDIMouse2);
    // 장치에 대한 독점권을 설정해주는 함수, 클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수
    m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

    // 장치에 대한 aCess 버전을 받아오는 함수
    m_pMouse->Acquire();

   

    m_KeyStates.resize((_uint)KEY::KEY_END);
    m_MouseStates.resize((_uint)MOUSE::MOUSE_END);


    return S_OK;
}

void CKeyManager::Update()
{
    if (GetFocus() != m_hWnd || GetForegroundWindow() != m_hWnd)
    {

        m_pMouse->Unacquire();
        m_isPrevFocused = false;
        return;
    }

    else if (!m_isPrevFocused)
    {
        DWORD dwElements = INFINITE;
        DIDEVICEOBJECTDATA od;
        while (m_pMouse->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), &od, &dwElements, 0) == DI_OK && dwElements > 0);
        m_pMouse->Acquire();
        m_isPrevFocused = true;
    }
    HWND hWnd = GetFocus();// 포커싱된 윈도우를 찾는 작업

    if (hWnd)
    {
        m_pKeyBoard->GetDeviceState(sizeof(m_byKeyState), m_byKeyState);
        m_pMouse->GetDeviceState(sizeof(m_tMouseState), &m_tMouseState);


        Update_KeyState();
        Update_MouseState();
    }
    /*포커스  해제 */
    else {

        // 키보드
        for (int i = 0; i < (int)KEY::KEY_END; ++i)
        {
            if (m_KeyStates[i].PrevPushed)
            {
                m_KeyStates[i].eState = INPUT_STATE::UP;
            }
            else
            {
                m_KeyStates[i].eState = INPUT_STATE::RELEASED;
            }
            m_KeyStates[i].PrevPushed = false;
        }

        // 마우스 
        for (int i = 0; i < (int)MOUSE::MOUSE_END; ++i)
        {
            if (m_MouseStates[i].PrevPushed)
            {
                m_MouseStates[i].eState = INPUT_STATE::UP;
            }
            else
            {
                m_MouseStates[i].eState = INPUT_STATE::RELEASED;
            }
            m_MouseStates[i].PrevPushed = false;
        }
    }
 
}

void CKeyManager::Update_KeyState()
{
   
        for (int i = 0; i < (_uint)KEY::KEY_END; ++i)
        {
            /*현재 키가 눌린 상태 */
          if ( m_byKeyState[g_arrKeys[i]] & 0x80)
            {
                _bool PrevState = m_KeyStates[i].PrevPushed;
                if (true == PrevState)
                {
                    m_KeyStates[i].eState = INPUT_STATE::PRESSED;
                }
                else if ( false == PrevState)
                {
                    m_KeyStates[i].eState = INPUT_STATE::DOWN;
                }
                m_KeyStates[i].PrevPushed = true;
            }
            else
            {
                /*현재 키가 눌리지 않은 상태 */
                _bool PrevState = m_KeyStates[i].PrevPushed;
                if (true == PrevState)
                {
                    m_KeyStates[i].eState = INPUT_STATE::UP;
                }
                else if (false == PrevState)
                {
                    m_KeyStates[i].eState = INPUT_STATE::RELEASED;
                }
                m_KeyStates[i].PrevPushed = false;
            }
           
        }

  
}

void CKeyManager::Update_MouseState()
{
    /*For Mouse*/
    for (int i = 0; i < (_uint)MOUSE::MOUSE_END; ++i)
    {
        /*현재 키가 눌린 상태 */
        if (m_tMouseState.rgbButtons[i] & 0x80)
        {
            _bool PrevState = m_MouseStates[i].PrevPushed;
            if (true == PrevState)
            {
                m_MouseStates[i].eState = INPUT_STATE::PRESSED;
            }
            else if (false == PrevState)
            {
                m_MouseStates[i].eState = INPUT_STATE::DOWN;
            }
            m_MouseStates[i].PrevPushed = true;
        }
        else
        {
            /*현재 키가 눌리지 않은 상태 */
            _bool PrevState = m_MouseStates[i].PrevPushed;
            if (true == PrevState)
            {
                m_MouseStates[i].eState = INPUT_STATE::UP;
            }
            else if (false == PrevState)
            {
                m_MouseStates[i].eState = INPUT_STATE::RELEASED;
            }
            m_MouseStates[i].PrevPushed = false;
        }

    }
}

_bool CKeyManager::CheckMousePos(RECT tRect)
{
    //In ScreenSpace;  
    POINT   MousePos =  GetMousePos(); 
    return  (tRect.left  <= MousePos.x && MousePos.x <= tRect.right)
		&& (tRect.top <= MousePos.y && MousePos.y <= tRect.bottom);
}

CKeyManager* CKeyManager::Create(HINSTANCE _hInst, HWND _hWnd, _float2 ViewPortSize)
{
    CKeyManager* pInstance = new CKeyManager();

    if (FAILED(pInstance->Initialize(_hInst, _hWnd, ViewPortSize)))
    {
        MSG_BOX("Failed to Created : CKey_Manager");
        Safe_Delete(pInstance);
    }

    return pInstance;
}
