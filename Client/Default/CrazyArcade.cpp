// CrazyArcade.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "CrazyArcade.h"
#include"MainApp.h"
#include"GameInstance.h"
#include"SessionManager.h"
#include"Client_Defines.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE g_hInst;                                // 현재 인스턴스입니다.
HWND g_hWnd;
HIMC hImc;
bool g_End = false;
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

TextSystem::TEXT_INPUT_STATE* g_Text = nullptr;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

#endif
    
    
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CRAZYARCADE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }


    std::shared_ptr<CMainApp> pMainApp = std::shared_ptr<CMainApp>(CMainApp::Create());
    //Shared Ptr로 관리
    std::shared_ptr<CGameInstance> m_pGameInstance = CGameInstance::GetInstance();
   
   
   

    if (FAILED(ADD_TIMER(L"Timer_Default")))
        return E_FAIL;
    if (FAILED(ADD_TIMER(L"Timer_60")))
        return E_FAIL;
   
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CRAZYARCADE));

    MSG msg;
    _float fTimeAcc = { 0.f };
   
    // 기본 메시지 루프입니다:
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (WM_QUIT == msg.message)
                break;

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            

        }

        if (g_End == true)
        {
            break;
        }

       
        UPDATE_TIMER(TEXT("Timer_Default"));

        fTimeAcc += GET_TIMEDELTA(TEXT("Timer_Default"));

        if (/*fTimeAcc > 1.f / 60.0f*/1)
        {
            fTimeAcc = 0.f;

           UPDATE_TIMER(TEXT("Timer_60"));

            pMainApp->Update(GET_TIMEDELTA(TEXT("Timer_60")));
            pMainApp->Render();
        }
    }
    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex = {};

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CRAZYARCADE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = nullptr;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    g_hInst = hInstance;  // 인스턴스 핸들을 전역 변수에 저장합니다.

   RECT rcWindow = { 0,0,g_iWinSizeX,g_iWinSizeY };
   AdjustWindowRect(&rcWindow, WS_OVERLAPPEDWINDOW, FALSE);

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   g_hWnd = hWnd;
   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//


#ifdef _DEBUG
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
#ifdef _DEBUG
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;
#endif

    switch (message)
    {
#ifdef _DEBUG
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        //g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
       // g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DPICHANGED:
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
        {
            UINT dpiX = LOWORD(wParam);      // X‑축 DPI
            UINT dpiY = HIWORD(wParam);      // Y‑축 DPI
            _float g_DpiScale = float(dpiX) / 96.0f;

            const RECT* suggested_rect = (RECT*)lParam;
            ::SetWindowPos(hWnd, 
                nullptr,
                suggested_rect->left,
                suggested_rect->top,
                suggested_rect->right - suggested_rect->left,
                suggested_rect->bottom - suggested_rect->top,
                SWP_NOZORDER | SWP_NOACTIVATE);


           
        
        }
        break;
#endif

    case WM_CHAR:
    {
        wchar_t ch = static_cast<wchar_t>(wParam);//유니코드로 
        if (!iswcntrl(ch)) /*제어코드*/
        {
            if(g_Text)g_Text->Buffer.insert(g_Text->CaretIndex++, 1, ch);//뮨자 한개 삽입
        }
        break;
    }
    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case VK_LEFT: 
        {
            if (g_Text && g_Text->CaretIndex)
            {
               --g_Text->CaretIndex;
            }       
            break;
        }
        case VK_RIGHT:
        {
            if (g_Text && g_Text->CaretIndex < g_Text->Buffer.length())
            {
                ++g_Text->CaretIndex;
            }
            break;
        }
        case VK_END:  if (g_Text) g_Text->CaretIndex = g_Text->Buffer.size(); break;
        case VK_BACK:
            if (g_Text && g_Text->CaretIndex) { g_Text->Buffer.erase(--g_Text->CaretIndex, 1); }
            break;
        case VK_DELETE:
            if (g_Text && g_Text->CaretIndex < g_Text->Buffer.size()) { g_Text->Buffer.erase(g_Text->CaretIndex, 1); }
            break;
        
        }
        return 0;
    }
    case VK_ESCAPE:
    {
        DestroyWindow(g_hWnd);
        g_End = true;
        return 0;;

    }
    case WM_IME_STARTCOMPOSITION:
    {
        if (g_Text) {
            g_Text->CharacterFactory.clear();
        }
        return 0;
    }
    case WM_IME_COMPOSITION:
    {
        hImc = ImmGetContext(hWnd);
        
        /*문자 조합 중*/
        LONG len = ImmGetCompositionStringW(hImc, GCS_COMPSTR, nullptr, 0);
        if (len > 0)
        {
            if (g_Text) {
                size_t Cnt = len / sizeof(wchar_t);
                wstring tmp(Cnt, L'\0');
                ImmGetCompositionStringW(hImc, GCS_COMPSTR, tmp.data(), len);
                g_Text->CharacterFactory.swap(tmp);
            }
        }
        else
        {
            /*문자 완성*/
            if (g_Text) {
                g_Text->CharacterFactory.clear();
                
            }
        }

        len = ImmGetCompositionStringW(hImc, GCS_RESULTSTR, nullptr, 0);
        if (len > 0) {
            if (g_Text) {

                std::wstring result(len / sizeof(wchar_t), L'\0');
                ImmGetCompositionStringW(hImc, GCS_RESULTSTR, result.data(), len);

              
                g_Text->Buffer.insert(g_Text->CaretIndex, result);
                g_Text->CaretIndex += result.size();
                g_Text->CharacterFactory.clear();        // 조합 종료
              
            }
        }
        ImmReleaseContext(hWnd, hImc);
        return 0;
    }
    case WM_IME_ENDCOMPOSITION:
    {
        if(g_Text)g_Text->CharacterFactory.clear();            // 보조 스트링 정리
        return 0;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        g_End = true;
        return 0;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
