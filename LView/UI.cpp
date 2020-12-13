#include "UI.h"
#include "Utils.h"
#include "Structs.h"
#include "LeagueMemoryReader.h"
#include "Dwmapi.h"
#include "Benchmark.h"
#include <string>
#include <list>

LPDIRECT3D9                        UI::pD3D = NULL;
LPDIRECT3DDEVICE9                  UI::pd3dDevice = NULL;
D3DPRESENT_PARAMETERS              UI::d3dpp = {};

std::string RandomString(const int len) {

	std::string tmp_s;
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	srand((time_t)time(0));
	tmp_s.reserve(len);

	for (int i = 0; i < len; ++i)
		tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];


	return tmp_s;
}

UI::UI(std::list<BaseView*> views) {
	this->views = views;
	for (auto it = views.begin(); it != views.end(); ++it)
		miscToolbox.viewBenchmarks[*it] = new ViewBenchmark();
}

void UI::Start() {


	// Create transparent window
	std::string windowClassName = RandomString(10);
	std::string windowName = RandomString(10);
	SetConsoleTitleA(windowName.c_str());
	
	// Create window with random name & class name
	WNDCLASSEXA wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, windowClassName.c_str(), NULL };
	RegisterClassExA(&wc);
	hWindow = CreateWindowExA(
		WS_EX_TOPMOST | WS_EX_NOACTIVATE | WS_EX_LAYERED,
		windowClassName.c_str(), windowName.c_str(),
		WS_POPUP,
		1, 1, 1920, 1080,
		nullptr, nullptr, GetModuleHandle(0), nullptr);

	ShowWindow(hWindow, SW_SHOW);
	
	// Make the window a little bit transparent
	SetLayeredWindowAttributes(hWindow, RGB(0, 0, 0), 200, LWA_COLORKEY | LWA_ALPHA);

	if (hWindow == NULL) {
		throw WinApiException("Failed to create overlay window");
	}

	// Initialize Direct3D
	if (!CreateDeviceD3D(hWindow))
	{
		CleanupDeviceD3D();
		UnregisterClassA(wc.lpszClassName, wc.hInstance);
		throw std::runtime_error("Failed to create D3D device");
	}

	// Setup imgui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigWindowsMoveFromTitleBarOnly = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hWindow);
	ImGui_ImplDX9_Init(pd3dDevice);

	// Make some fonts
	miscToolbox.fontConfigSmall.SizePixels = 10;
	miscToolbox.fontConfigNormal.SizePixels = 13;
	miscToolbox.fontSmall = io.Fonts->AddFontDefault(&miscToolbox.fontConfigSmall);
	miscToolbox.fontNormal = io.Fonts->AddFontDefault(&miscToolbox.fontConfigNormal);

	// Load configs
	configs.LoadFromFile(configFilePath);
	for (auto it = views.begin(); it != views.end(); ++it) {
		BaseView* view = *it;
		configs.SetPrefixKey(view->GetName());
		view->OnLoadSettings(configs);
	}

	ImGui::GetStyle().Alpha = 1.f;
}

void UI::RenderUI(MemSnapshot& memSnapshot) {

	high_resolution_clock::time_point timeBefore;
	duration<float, std::milli> timeDuration;

	// Draw world space overlay, this is just an overlay over the entire screen.
	// Cheat authors are expected to properly draw into world space.
	auto io = ImGui::GetIO();
	ImDrawList* list;

	ImGui::SetNextWindowSize(io.DisplaySize);
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("##Overlay", nullptr,
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoInputs |
		ImGuiWindowFlags_NoBackground
	);
	list = ImGui::GetWindowDrawList();
	for (auto it = views.begin(); it != views.end(); ++it) {
		BaseView* view = *it;
		if (view->enabled) {
			timeBefore = high_resolution_clock::now();

			miscToolbox.canvas = list;
			view->DrawWorldSpaceOverlay(memSnapshot, miscToolbox);

			timeDuration = high_resolution_clock::now() - timeBefore;
			miscToolbox.viewBenchmarks[view]->drawWorldOverlayMs = timeDuration.count();
		}
	}
	ImGui::End();

	// Draw minimap overlay
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
	ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::Begin("Minimap Overlay", nullptr,
		ImGuiWindowFlags_NoScrollbar
	);
	list = ImGui::GetWindowDrawList();
	for (auto it = views.begin(); it != views.end(); ++it) {
		BaseView* view = *it;
		if (view->enabled) {
			timeBefore = high_resolution_clock::now();

			miscToolbox.canvas = list;
			view->DrawMinimapOverlay(memSnapshot, miscToolbox);

			timeDuration = high_resolution_clock::now() - timeBefore;
			miscToolbox.viewBenchmarks[view]->drawMinimapOverlayMs = timeDuration.count();
		}
	}

	ImGui::End();
	ImGui::PopStyleVar();

	ImGui::Begin("Settings");
	ImGui::TextColored(Colors::Cyan, "LVIEW (External RPM Tool) by leryss");
	ImGui::Separator();

	// Saves the settings to file when button clicked
	if (ImGui::Button("Save Settings")) {
		for (auto it = views.begin(); it != views.end(); ++it) {
			BaseView* view = *it;
			configs.SetPrefixKey(view->GetName());
			view->OnSaveSettings(configs);
		}
		configs.SaveToFile(configFilePath);
	}

	int i = 0;
	for (auto it = views.begin(); it != views.end(); ++it, ++i) {
		BaseView* view = *it;

		// Grey out the header of disabled cheats for readability
		bool shouldPopColor = false;
		if (!view->enabled) {
			ImGui::PushStyleColor(ImGuiCol_Header, (ImVec4)Colors::Grey);
			shouldPopColor = true;
		}
		
		// Draw cheat settings, these will draw even if cheat is disabled
		if (ImGui::CollapsingHeader(view->GetName())) {
			ImGui::PushID(i);
			ImGui::Checkbox("Enabled", &view->enabled);
			ImGui::PopID();
			
			timeBefore = high_resolution_clock::now();

			view->DrawSettings(memSnapshot, miscToolbox);

			timeDuration = high_resolution_clock::now() - timeBefore;
			miscToolbox.viewBenchmarks[view]->drawSettingsMs = timeDuration.count();
		}

		// If cheat enabled, draw all its panels if any
		if (view->enabled) {
			timeBefore = high_resolution_clock::now();

			view->DrawPanel(memSnapshot, miscToolbox);

			timeDuration = high_resolution_clock::now() - timeBefore;
			miscToolbox.viewBenchmarks[view]->drawPanelMs = timeDuration.count();
		}

		if (shouldPopColor)
			ImGui::PopStyleColor();
	}
	ImGui::End();
}

void UI::Update(MemSnapshot& memSnapshot, bool skipRender) {
	
	high_resolution_clock::time_point timeBefore;
	duration<float, std::milli> timeDuration;

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	
	if (skipRender) {
		// Poll and handle messages (inputs, window resize, etc.)
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	// Start the Dear ImGui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::PushFont(miscToolbox.fontNormal);

	timeBefore = high_resolution_clock::now();

	if (skipRender) {
		RenderUI(memSnapshot);
	}

	timeDuration = high_resolution_clock::now() - timeBefore;	
	miscToolbox.generalBenchmarks->processTimeMs = timeDuration.count();

	ImGui::PopFont();
	
	// Render
	timeBefore = high_resolution_clock::now();

	pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
	if (pd3dDevice->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		pd3dDevice->EndScene();

	}
	HRESULT result = pd3dDevice->Present(NULL, NULL, NULL, NULL);

	// Handle loss of D3D9 device
	if (result == D3DERR_DEVICELOST && pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		ResetDevice();

	timeDuration = high_resolution_clock::now() - timeBefore;
	miscToolbox.generalBenchmarks->renderTimeMs = timeDuration.count();
}


// Helper functions
bool UI::CreateDeviceD3D(HWND hWnd)
{
	if ((pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
		return false;

	// Create the D3DDevice
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	//d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;       // Present with vsync
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
	if (pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &pd3dDevice) < 0)
		return false;

	return true;
}

void UI::CleanupDeviceD3D()
{
	if (pd3dDevice) { pd3dDevice->Release(); pd3dDevice = NULL; }
	if (pD3D) { pD3D->Release(); pD3D = NULL; }
}

void UI::ResetDevice()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	HRESULT hr = pd3dDevice->Reset(&d3dpp);
	if (hr == D3DERR_INVALIDCALL)
		IM_ASSERT(0);
	ImGui_ImplDX9_CreateDeviceObjects();
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
LRESULT WINAPI UI::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
		{
			d3dpp.BackBufferWidth = LOWORD(lParam);
			d3dpp.BackBufferHeight = HIWORD(lParam);
			ResetDevice();
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}