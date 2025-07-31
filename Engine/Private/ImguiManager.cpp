#include "ImguiManager.h"

#ifdef _DEBUG
CImguiManager::CImguiManager(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
    :m_pDevice{pDevice},
    m_pContext{pContext}
{

}

CImguiManager::~CImguiManager()
{
	ImGui_ImplDX11_Shutdown();     // Renderer 백엔드 정리
	ImGui_ImplWin32_Shutdown();    // Platform 백엔드 정리
	ImGui::DestroyContext();
}

HRESULT CImguiManager::Initialize(HWND _hWnd)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	//ImNodes::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableKeyboard;

	ImFont* font= io.Fonts->AddFontFromFileTTF("../../EngineSDK/Font/NanumGothicBold.ttf", 18.0f, nullptr,
	io.Fonts->GetGlyphRangesKorean());
	if (font == nullptr)
		std::cerr << "Dis" << std::endl;
	io.FontDefault = font;
	io.Fonts->Build();
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();
	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(_hWnd);
	ImGui_ImplDX11_Init(m_pDevice.Get(), m_pContext.Get());

	return S_OK;
	
}

void CImguiManager::Update()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	//ImGuizmo::BeginFrame();

	ImGui::Begin("텍스트");
	_float f;
	ImGui::SliderFloat("Fart", &f, 0.f,1.f);
	ImGui::End();
}

void CImguiManager::Render()
{
	// Rendering
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// UpdateBounding and Render additional Platform Windows
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

CImguiManager* CImguiManager::Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext, HWND _hWnd)
{
	CImguiManager* pInstance = new CImguiManager(pDevice, pContext); 
	if (pInstance->Initialize(_hWnd))
	{
		MSG_BOX("임구이 생성 실패 ");
		Safe_Delete(pInstance);
		return nullptr;
	}

    return pInstance;
}

#endif