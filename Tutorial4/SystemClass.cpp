#include "Systemclass.h"


SystemClass::SystemClass()
{
	m_applicationClass = 0;
}


SystemClass::SystemClass(const SystemClass& other)
{
}


SystemClass::~SystemClass()
{
}


bool SystemClass::Initialize()
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int screenwidth, screenheight;
	int posX, posY;
	bool result;

	//메시지 핸들
	ApplicationHandle = this;

	//윈도우 핸들 가져오기
	m_hinstance = GetModuleHandle(NULL);

	//프로그램 이름
	m_applicationName = L"Tutorial4";

	//윈도우 클래스 세팅
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	//윈도우 클래스 등록
	RegisterClassEx(&wc);

	//스크린크기 고정
	screenwidth = 1200;
	screenheight = 800;

	//화면 중앙에 정렬
	posX = (GetSystemMetrics(SM_CXSCREEN) - screenwidth) / 2;
	posY = (GetSystemMetrics(SM_CYSCREEN) - screenheight) / 2;


	//프로그램 윈도우 생성
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, WS_OVERLAPPEDWINDOW,
		posX, posY, screenwidth, screenheight, NULL, NULL, m_hinstance, NULL);

	//윈도우 포커스 가져오기
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	m_applicationClass = new ApplicationClass();
	if (!m_applicationClass)
	{
		result = false;
	}

	result = m_applicationClass->Initialize(m_hinstance, m_hwnd, VSYNC_ENABLED, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR, screenwidth, screenheight);

	return result;
}


void SystemClass::Shutdown()
{
	//윈도우 관련 자원반환
	ShutdownWindows();

	return;
}


void SystemClass::Run()
{
	MSG msg;
	bool done, result;


	//메모리 0으로 초기화
	ZeroMemory(&msg, sizeof(MSG));


	done = false;
	while (!done)
	{
		//메시지 전달확인
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//종료 메시지 확인
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			//종료가 아니면 사이클반복
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}

	}

	return;
}


bool SystemClass::Frame()
{
	bool result;

	result = m_applicationClass->Frame();
	if (!result)
	{
		return false;
	}

	return true;
}


LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}




void SystemClass::ShutdownWindows()
{

	//윈도우 삭제
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	//윈도우 클래스 등록해제
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	//핸들 반환
	ApplicationHandle = NULL;

	return;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		//Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	//All other messages pass to the message handler in the system class.
	default:
	{
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}