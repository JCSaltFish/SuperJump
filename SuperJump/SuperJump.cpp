// SuperJump.cpp: 定义应用程序的入口点。
//

#include "stdafx.h"
#include "SuperJump.h"

#include "core.h"

#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;										// 当前实例
WCHAR szTitle[MAX_LOADSTRING];							// 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];					// 主窗口类名

HWND hWndMain;											// 主窗口句柄
SuperJump g_SuperJumpCore;

WCHAR szStartBtnClass[] = TEXT("StartBtn");				// 开始按钮窗口类名
BOOL g_bStartBtnActivated = FALSE;
BOOL g_bStartBtnPressed = FALSE;
HWND hWndStartBtn;										// 开始按钮窗口句柄

BOOL g_bRunning = FALSE;
BOOL g_bStopping = FALSE;

WCHAR szAboutBtnClass[] = TEXT("AboutBtn");				// 关于按钮窗口类名
BOOL g_bAboutBtnActivated = FALSE;
BOOL g_bAboutBtnPressed = FALSE;

WCHAR szHigherBtnClass[] = TEXT("HigherBtn");			// 增加按钮窗口类名
BOOL g_bHigherBtnActivated = FALSE;
BOOL g_bHigherBtnPressed = FALSE;

float g_fDelta = 1.2f;

WCHAR szLowerBtnClass[] = TEXT("LowerBtn");				// 减少按钮窗口类名
BOOL g_bLowerBtnActivated = FALSE;
BOOL g_bLowerBtnPressed = FALSE;

WCHAR szAboutWndClass[] = TEXT("AboutWnd");				// 关于窗口类名
BOOL g_bAboutShown = TRUE;
HWND hWndAbout;											// 关于窗口句柄

WCHAR szAboutCloseBtnClass[] = TEXT("AboutCloseBtn");	// 关于窗口关闭按钮类名
BOOL g_bAboutCloseBtnActivated = FALSE;
BOOL g_bAboutCloseBtnPressed = FALSE;

BOOL g_bQuit = FALSE;

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK    StartBtnProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    AboutBtnProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    HigherBtnProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    LowerBtnProc(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK	AboutWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	AboutCloseBtnProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SUPERJUMP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SUPERJUMP));

    MSG msg;

    // 主消息循环: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SUPERJUMP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SUPERJUMP);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SUPERJUMP));

	if (!RegisterClassExW(&wcex))
		return FALSE;

	wcex.lpszClassName	= szStartBtnClass;
	wcex.lpszMenuName	= NULL;
	wcex.lpfnWndProc	= StartBtnProc;
	wcex.cbWndExtra		= 8;

	if (!RegisterClassExW(&wcex))
		return FALSE;

	wcex.lpszClassName = szAboutBtnClass;
	wcex.lpszMenuName = NULL;
	wcex.lpfnWndProc = AboutBtnProc;
	wcex.cbWndExtra = 8;

	if (!RegisterClassExW(&wcex))
		return FALSE;

	wcex.lpszClassName = szHigherBtnClass;
	wcex.lpszMenuName = NULL;
	wcex.lpfnWndProc = HigherBtnProc;
	wcex.cbWndExtra = 8;

	if (!RegisterClassExW(&wcex))
		return FALSE;

	wcex.lpszClassName = szLowerBtnClass;
	wcex.lpszMenuName = NULL;
	wcex.lpfnWndProc = LowerBtnProc;
	wcex.cbWndExtra = 8;

	if (!RegisterClassExW(&wcex))
		return FALSE;

	wcex.lpszClassName = szAboutWndClass;
	wcex.lpszMenuName = NULL;
	wcex.lpfnWndProc = AboutWndProc;
	wcex.cbWndExtra = 8;

	if (!RegisterClassExW(&wcex))
		return FALSE;

	wcex.lpszClassName = szAboutCloseBtnClass;
	wcex.lpszMenuName = NULL;
	wcex.lpfnWndProc = AboutCloseBtnProc;
	wcex.cbWndExtra = 8;

	if (!RegisterClassExW(&wcex))
		return FALSE;

	return TRUE;
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle,
	  WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
      CW_USEDEFAULT, 0, 800, 600, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   hWndMain = hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		{
			RECT clientRect;
			GetClientRect(hWnd, &clientRect);

			hWndStartBtn = CreateWindow(szStartBtnClass, NULL, WS_CHILD,
				(clientRect.right - clientRect.left) / 2 - 50,
				(clientRect.bottom - clientRect.top) / 2 - 80,
				100, 100, hWnd, NULL, hInst, nullptr);
			ShowWindow(hWndStartBtn, SW_NORMAL);

			g_SuperJumpCore.setHWnd(hWndStartBtn);

			HWND hWndAboutBtn = CreateWindow(szAboutBtnClass, NULL, WS_CHILD,
				clientRect.right - 60, clientRect.bottom - 40, 60, 40,
				hWnd, NULL, hInst, nullptr);
			ShowWindow(hWndAboutBtn, SW_NORMAL);

			HWND hWndHigherBtn = CreateWindow(szHigherBtnClass, NULL, WS_CHILD,
				clientRect.right - 120, clientRect.bottom - 40, 60, 40,
				hWnd, NULL, hInst, nullptr);
			ShowWindow(hWndHigherBtn, SW_NORMAL);

			HWND hWndLowerBtn = CreateWindow(szLowerBtnClass, NULL, WS_CHILD,
				clientRect.right - 240, clientRect.bottom - 40, 60, 40,
				hWnd, NULL, hInst, nullptr);
			ShowWindow(hWndLowerBtn, SW_NORMAL);

			hWndAbout = CreateWindow(szAboutWndClass, NULL, WS_CHILD,
				(clientRect.right - clientRect.left) / 2 - 200,
				(clientRect.bottom - clientRect.top) / 2 - 180,
				400, 360, hWnd, NULL, hInst, nullptr);
			ShowWindow(hWndAbout, SW_NORMAL);
		}
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

			RECT clientRect;
			GetClientRect(hWnd, &clientRect);

			HPEN pen;
			HBRUSH brush;
			HFONT hFont;

			pen = CreatePen(PS_SOLID, 0, RGB(225, 225, 225));
			brush = CreateSolidBrush(RGB(225, 225, 225));
			SelectObject(hdc, pen);
			SelectObject(hdc, brush);
			Rectangle(hdc, 0, 0, clientRect.right, clientRect.bottom);
			DeleteObject(pen);
			DeleteObject(brush);

			hFont = CreateFontA(28, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY, DEFAULT_PITCH, "微软雅黑");
			SelectObject(hdc, hFont);
			SetTextColor(hdc, RGB(0, 0, 0));
			SetBkColor(hdc, RGB(225, 225, 225));
			RECT upperTextRect;
			SetRect(&upperTextRect, clientRect.left, clientRect.bottom / 2 - 130,
				clientRect.right, clientRect.bottom / 2);
			if (!g_bRunning)
			{
				TCHAR szStopStatus[255] = _T("");
				LoadString(NULL, IDS_STOP_STATUS, szStopStatus, MAX_LOADSTRING);
				DrawText(hdc, szStopStatus, -1, &upperTextRect, DT_CENTER);
			}
			else if (g_bStopping)
				DrawText(hdc, _T("正在停止…"), -1, &upperTextRect, DT_CENTER);
			else
			{
				TCHAR szStartStatus[255] = _T("");
				LoadString(NULL, IDS_START_STATUS, szStartStatus, MAX_LOADSTRING);
				DrawText(hdc, szStartStatus, -1, &upperTextRect, DT_CENTER);
			}
			DeleteObject(hFont);

			hFont = CreateFontA(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE,
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY, DEFAULT_PITCH, "微软雅黑");
			SelectObject(hdc, hFont);
			RECT lowerTextRect;
			SetRect(&lowerTextRect, clientRect.left, clientRect.bottom / 2 + 40,
				clientRect.right, clientRect.bottom);
			if (!g_bRunning)
			{
				TCHAR szStartHint[255] = _T("");
				LoadString(NULL, IDS_START_HINT, szStartHint, MAX_LOADSTRING);
				DrawText(hdc, szStartHint, -1, &lowerTextRect, DT_CENTER);
			}
			else
			{
				TCHAR szStopHint[255] = _T("");
				LoadString(NULL, IDS_STOP_HINT, szStopHint, MAX_LOADSTRING);
				DrawText(hdc, szStopHint, -1, &lowerTextRect, DT_CENTER);
			}
			DeleteObject(hFont);

			pen = CreatePen(PS_SOLID, 0, RGB(210, 210, 220));
			brush = CreateSolidBrush(RGB(210, 210, 220));
			SelectObject(hdc, pen);
			SelectObject(hdc, brush);
			Rectangle(hdc, clientRect.left, clientRect.bottom - 40,
				clientRect.right, clientRect.bottom);
			DeleteObject(pen);
			DeleteObject(brush);

			hFont = CreateFontA(26, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE,
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY, DEFAULT_PITCH, "微软雅黑");
			SelectObject(hdc, hFont);
			SetBkColor(hdc, RGB(210, 210, 220));
			RECT deltaTextRect;
			SetRect(&deltaTextRect, clientRect.right - 180, clientRect.bottom - 36,
				clientRect.right - 120, clientRect.bottom);
			TCHAR szDeltaFloat[255] = _T("");
			swprintf_s(szDeltaFloat, _T("%.1f"), g_fDelta);
			DrawText(hdc, szDeltaFloat, -1, &deltaTextRect,
				DT_CENTER | DT_SINGLELINE | DT_VCENTER);
			DeleteObject(hFont);

			hFont = CreateFontA(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE,
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY, DEFAULT_PITCH, "微软雅黑");
			SelectObject(hdc, hFont);
			RECT deltaTitleTextRect;
			SetRect(&deltaTitleTextRect, clientRect.right - 310, clientRect.bottom - 38,
				clientRect.right - 240, clientRect.bottom);
			TCHAR szDelta[255] = _T("");
			LoadString(NULL, IDS_DELTA, szDelta, MAX_LOADSTRING);
			DrawText(hdc, szDelta, -1, &deltaTitleTextRect,
				DT_CENTER | DT_SINGLELINE | DT_VCENTER);
			DeleteObject(hFont);

            EndPaint(hWnd, &ps);
        }
        break;
	case WM_CLOSE:
		if (g_bRunning)
		{
			g_SuperJumpCore.stop();
			g_bStopping = TRUE;
			g_bQuit = TRUE;
			RECT clientRect;
			GetClientRect(hWnd, &clientRect);
			InvalidateRect(hWnd, &clientRect, TRUE);
		}
		else
			DestroyWindow(hWndMain);
		break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK StartBtnProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT clientRect;
	GetClientRect(hWnd, &clientRect);

	RECT rootClientRect;
	GetClientRect(GetParent(hWnd), &rootClientRect);

	switch (message)
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);

			HPEN pen;
			HBRUSH brush;

			pen = CreatePen(PS_SOLID, 0, RGB(225, 225, 225));
			brush = CreateSolidBrush(RGB(225, 225, 225));
			SelectObject(hdc, pen);
			SelectObject(hdc, brush);
			Rectangle(hdc, 0, 0, clientRect.right, clientRect.bottom);
			DeleteObject(pen);
			DeleteObject(brush);
			
			if (g_bStartBtnPressed)
			{
				pen = CreatePen(PS_SOLID, 0, RGB(80, 115, 80));
				brush = CreateSolidBrush(RGB(80, 115, 80));
			}
			else if (g_bStartBtnActivated)
			{
				pen = CreatePen(PS_SOLID, 0, RGB(100, 150, 105));
				brush = CreateSolidBrush(RGB(100, 150, 105));
			}
			else
			{
				pen = CreatePen(PS_SOLID, 0, RGB(90, 140, 100));
				brush = CreateSolidBrush(RGB(90, 140, 100));
			}
			SelectObject(hdc, pen);
			SelectObject(hdc, brush);
			Ellipse(hdc, 0, 0, 100, 100);
			DeleteObject(pen);
			DeleteObject(brush);

			if (g_bStartBtnPressed)
			{
				pen = CreatePen(PS_SOLID, 0, RGB(225, 225, 225));
				brush = CreateSolidBrush(RGB(225, 225, 225));
			}
			else
			{
				pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
				brush = CreateSolidBrush(RGB(255, 255, 255));
			}
			SelectObject(hdc, pen);
			SelectObject(hdc, brush);
			if (g_bRunning)
			{
				POINT pointRectangle[4];
				pointRectangle[0].x = 35;
				pointRectangle[0].y = 35;
				pointRectangle[1].x = 35;
				pointRectangle[1].y = 65;
				pointRectangle[2].x = 65;
				pointRectangle[2].y = 65;
				pointRectangle[3].x = 65;
				pointRectangle[3].y = 35;
				Polygon(hdc, pointRectangle, 4);
			}
			else
			{
				POINT pointTriangle[3];
				pointTriangle[0].x = 40;
				pointTriangle[0].y = 30;
				pointTriangle[1].x = 40;
				pointTriangle[1].y = 70;
				pointTriangle[2].x = 70;
				pointTriangle[2].y = 50;
				Polygon(hdc, pointTriangle, 3);
			}
			DeleteObject(pen);
			DeleteObject(brush);

			EndPaint(hWnd, &ps);
		}
		break;
	case WM_MOUSEMOVE:
		{
			TRACKMOUSEEVENT trackMouseEvent;
			trackMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
			trackMouseEvent.dwFlags = TME_LEAVE;
			trackMouseEvent.hwndTrack = hWnd;
			TrackMouseEvent(&trackMouseEvent);

			g_bStartBtnActivated = TRUE;
			if (!g_bAboutShown)
				InvalidateRect(hWnd, &clientRect, FALSE);
		}
		break;
	case WM_MOUSELEAVE:
		{
			g_bStartBtnActivated = FALSE;
			if (!g_bAboutShown)
				InvalidateRect(hWnd, &clientRect, FALSE);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			g_bStartBtnPressed = TRUE;
			if (!g_bAboutShown)
				InvalidateRect(hWnd, &clientRect, FALSE);
		}
		break;
	case WM_LBUTTONUP:
		{
			if (g_bAboutShown)
				break;
			if (!g_bRunning)
			{
				g_bStartBtnPressed = FALSE;
				g_SuperJumpCore.begin();
				g_bRunning = TRUE;
				InvalidateRect(hWnd, &clientRect, FALSE);
				InvalidateRect(GetParent(hWnd), &rootClientRect, TRUE);
			}
			else if (!g_bStopping)
			{
				g_SuperJumpCore.stop();
				g_bStopping = TRUE;
				InvalidateRect(GetParent(hWnd), &rootClientRect, TRUE);
			}
		}
		break;
	case WM_COMPLETE:
		{
			g_bRunning = FALSE;
			g_bStartBtnPressed = FALSE;
			g_bStopping = FALSE;

			if (g_bQuit)
			{
				DestroyWindow(hWndMain);
				break;
			}

			if (g_bAboutShown)
				break;
			InvalidateRect(hWnd, &clientRect, FALSE);
			InvalidateRect(GetParent(hWnd), &rootClientRect, TRUE);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK AboutBtnProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT clientRect;
	GetClientRect(hWnd, &clientRect);

	switch (message)
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);

			HPEN pen;
			HBRUSH brush;
			HFONT hFont;

			if (g_bAboutBtnPressed)
			{
				pen = CreatePen(PS_SOLID, 0, RGB(126, 126, 126));
				brush = CreateSolidBrush(RGB(126, 126, 126));
			}
			else if (g_bAboutBtnActivated)
			{
				pen = CreatePen(PS_SOLID, 0, RGB(140, 140, 150));
				brush = CreateSolidBrush(RGB(140, 140, 150));
			}
			else
			{
				pen = CreatePen(PS_SOLID, 0, RGB(210, 210, 220));
				brush = CreateSolidBrush(RGB(210, 210, 220));
			}
			SelectObject(hdc, pen);
			SelectObject(hdc, brush);
			Rectangle(hdc, 0, 0, clientRect.right, clientRect.bottom);
			DeleteObject(pen);
			DeleteObject(brush);

			hFont = CreateFontA(28, 10, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE,
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY, DEFAULT_PITCH, "微软雅黑");
			SelectObject(hdc, hFont);
			SetTextColor(hdc, RGB(0, 0, 0));
			if (g_bAboutBtnPressed)
				SetBkColor(hdc, RGB(126, 126, 126));
			else if (g_bAboutBtnActivated)
				SetBkColor(hdc, RGB(140, 140, 150));
			else
				SetBkColor(hdc, RGB(210, 210, 220));
			DrawText(hdc, TEXT("· · ·"), -1, &clientRect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
			DeleteObject(hFont);

			EndPaint(hWnd, &ps);
		}
		break;
	case WM_MOUSEMOVE:
		{
			if (g_bAboutShown)
				break;
			
			TRACKMOUSEEVENT trackMouseEvent;
			trackMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
			trackMouseEvent.dwFlags = TME_LEAVE;
			trackMouseEvent.hwndTrack = hWnd;
			TrackMouseEvent(&trackMouseEvent);

			g_bAboutBtnActivated = TRUE;
			InvalidateRect(hWnd, &clientRect, FALSE);
		}
		break;
	case WM_MOUSELEAVE:
		{
			if (g_bAboutShown)
				break;

			g_bAboutBtnActivated = FALSE;
			g_bAboutBtnPressed = FALSE;
			InvalidateRect(hWnd, &clientRect, FALSE);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			if (g_bAboutShown)
				break;

			g_bAboutBtnPressed = TRUE;
			InvalidateRect(hWnd, &clientRect, FALSE);
		}
		break;
	case WM_LBUTTONUP:
		{
			if (g_bAboutShown)
				break;

			g_bAboutBtnActivated = FALSE;
			g_bAboutBtnPressed = FALSE;
			InvalidateRect(hWnd, &clientRect, FALSE);

			g_bAboutShown = TRUE;
			ShowWindow(hWndAbout, SW_NORMAL);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK HigherBtnProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT clientRect;
	GetClientRect(hWnd, &clientRect);

	RECT rootClientRect;
	GetClientRect(GetParent(hWnd), &rootClientRect);

	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		HPEN pen;
		HBRUSH brush;
		HFONT hFont;

		if (g_bHigherBtnPressed)
		{
			pen = CreatePen(PS_SOLID, 0, RGB(126, 126, 126));
			brush = CreateSolidBrush(RGB(126, 126, 126));
		}
		else if (g_bHigherBtnActivated)
		{
			pen = CreatePen(PS_SOLID, 0, RGB(140, 140, 150));
			brush = CreateSolidBrush(RGB(140, 140, 150));
		}
		else
		{
			pen = CreatePen(PS_SOLID, 0, RGB(210, 210, 220));
			brush = CreateSolidBrush(RGB(210, 210, 220));
		}
		SelectObject(hdc, pen);
		SelectObject(hdc, brush);
		Rectangle(hdc, 0, 0, clientRect.right, clientRect.bottom);
		DeleteObject(pen);
		DeleteObject(brush);

		hFont = CreateFontA(38, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH, "微软雅黑");
		SelectObject(hdc, hFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		if (g_bHigherBtnPressed)
			SetBkColor(hdc, RGB(126, 126, 126));
		else if (g_bHigherBtnActivated)
			SetBkColor(hdc, RGB(140, 140, 150));
		else
			SetBkColor(hdc, RGB(210, 210, 220));
		DrawText(hdc, TEXT("+"), -1, &clientRect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		DeleteObject(hFont);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_MOUSEMOVE:
	{
		if (g_bAboutShown)
			break;

		TRACKMOUSEEVENT trackMouseEvent;
		trackMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
		trackMouseEvent.dwFlags = TME_LEAVE;
		trackMouseEvent.hwndTrack = hWnd;
		TrackMouseEvent(&trackMouseEvent);

		g_bHigherBtnActivated = TRUE;
		InvalidateRect(hWnd, &clientRect, FALSE);
	}
	break;
	case WM_MOUSELEAVE:
	{
		if (g_bAboutShown)
			break;

		g_bHigherBtnActivated = FALSE;
		g_bHigherBtnPressed = FALSE;
		InvalidateRect(hWnd, &clientRect, FALSE);
	}
	break;
	case WM_LBUTTONDOWN:
	{
		if (g_bAboutShown)
			break;

		g_bHigherBtnPressed = TRUE;
		InvalidateRect(hWnd, &clientRect, FALSE);
	}
	break;
	case WM_LBUTTONUP:
	{
		if (g_bAboutShown)
			break;

		g_bHigherBtnPressed = FALSE;
		InvalidateRect(hWnd, &clientRect, FALSE);

		if (g_fDelta < 2.9f)
		{
			g_fDelta += 0.1f;
			g_SuperJumpCore.setDelta(g_fDelta);
			InvalidateRect(GetParent(hWnd), &rootClientRect, TRUE);
		}
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK LowerBtnProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT clientRect;
	GetClientRect(hWnd, &clientRect);

	RECT rootClientRect;
	GetClientRect(GetParent(hWnd), &rootClientRect);

	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		HPEN pen;
		HBRUSH brush;
		HFONT hFont;

		if (g_bLowerBtnPressed)
		{
			pen = CreatePen(PS_SOLID, 0, RGB(126, 126, 126));
			brush = CreateSolidBrush(RGB(126, 126, 126));
		}
		else if (g_bLowerBtnActivated)
		{
			pen = CreatePen(PS_SOLID, 0, RGB(140, 140, 150));
			brush = CreateSolidBrush(RGB(140, 140, 150));
		}
		else
		{
			pen = CreatePen(PS_SOLID, 0, RGB(210, 210, 220));
			brush = CreateSolidBrush(RGB(210, 210, 220));
		}
		SelectObject(hdc, pen);
		SelectObject(hdc, brush);
		Rectangle(hdc, 0, 0, clientRect.right, clientRect.bottom);
		DeleteObject(pen);
		DeleteObject(brush);

		hFont = CreateFontA(38, 20, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH, "微软雅黑");
		SelectObject(hdc, hFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		if (g_bLowerBtnPressed)
			SetBkColor(hdc, RGB(126, 126, 126));
		else if (g_bLowerBtnActivated)
			SetBkColor(hdc, RGB(140, 140, 150));
		else
			SetBkColor(hdc, RGB(210, 210, 220));
		DrawText(hdc, TEXT("-"), -1, &clientRect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		DeleteObject(hFont);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_MOUSEMOVE:
	{
		if (g_bAboutShown)
			break;

		TRACKMOUSEEVENT trackMouseEvent;
		trackMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
		trackMouseEvent.dwFlags = TME_LEAVE;
		trackMouseEvent.hwndTrack = hWnd;
		TrackMouseEvent(&trackMouseEvent);

		g_bLowerBtnActivated = TRUE;
		InvalidateRect(hWnd, &clientRect, FALSE);
	}
	break;
	case WM_MOUSELEAVE:
	{
		if (g_bAboutShown)
			break;

		g_bLowerBtnActivated = FALSE;
		g_bLowerBtnPressed = FALSE;
		InvalidateRect(hWnd, &clientRect, FALSE);
	}
	break;
	case WM_LBUTTONDOWN:
	{
		if (g_bAboutShown)
			break;

		g_bLowerBtnPressed = TRUE;
		InvalidateRect(hWnd, &clientRect, FALSE);
	}
	break;
	case WM_LBUTTONUP:
	{
		if (g_bAboutShown)
			break;

		g_bLowerBtnPressed = FALSE;
		InvalidateRect(hWnd, &clientRect, FALSE);

		if (g_fDelta > 0.1f)
		{
			g_fDelta -= 0.1f;
			g_SuperJumpCore.setDelta(g_fDelta);
			InvalidateRect(GetParent(hWnd), &rootClientRect, TRUE);
		}
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK AboutWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT clientRect;
	GetClientRect(hWnd, &clientRect);

	switch (message)
	{
	case WM_CREATE:
		{
			HWND hWndAboutCloseBtn = CreateWindow(szAboutCloseBtnClass, NULL, WS_CHILD,
				(clientRect.right - clientRect.left) / 2 - 50, clientRect.bottom - 50,
				100, 30, hWnd, NULL, hInst, nullptr);
			ShowWindow(hWndAboutCloseBtn, SW_NORMAL);
		}
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);

			HPEN pen;
			HBRUSH brush;
			HFONT hFont;

			pen = CreatePen(PS_SOLID, 2, RGB(90, 140, 100));
			brush = CreateSolidBrush(RGB(255, 255, 255));
			SelectObject(hdc, pen);
			SelectObject(hdc, brush);
			Rectangle(hdc, clientRect.left, clientRect.top,
				clientRect.right + 1, clientRect.bottom + 1);
			DeleteObject(pen);
			DeleteObject(brush);

			hFont = CreateFontA(28, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE,
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY, DEFAULT_PITCH, "微软雅黑");
			SelectObject(hdc, hFont);
			SetTextColor(hdc, RGB(0, 0, 0));
			SetBkColor(hdc, RGB(255, 255, 255));
			RECT aboutTitleRect;
			SetRect(&aboutTitleRect, clientRect.left, clientRect.top + 20,
				clientRect.right, clientRect.top + 40);
			DrawText(hdc, TEXT("欢迎使用本软件"), -1, &aboutTitleRect,
				DT_CENTER | DT_SINGLELINE | DT_VCENTER);
			DeleteObject(hFont);

			hFont = CreateFontA(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE,
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY, DEFAULT_PITCH, "微软雅黑");
			SelectObject(hdc, hFont);
			RECT aboutTextRect;
			SetRect(&aboutTextRect, clientRect.left + 25, clientRect.top + 53,
				clientRect.right - 20, clientRect.bottom - 50);
			TCHAR szAboutText[1024] = _T("");
			LoadString(NULL, IDS_ABOUT_TEXT, szAboutText, 1024);
			DrawText(hdc, szAboutText, -1, &aboutTextRect, DT_WORDBREAK | DT_EDITCONTROL);
			DeleteObject(hFont);

			EndPaint(hWnd, &ps);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK AboutCloseBtnProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT clientRect;
	GetClientRect(hWnd, &clientRect);

	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		HPEN pen;
		HBRUSH brush;
		HFONT hFont;

		if (g_bAboutCloseBtnPressed)
		{
			pen = CreatePen(PS_SOLID, 0, RGB(140, 140, 150));
			brush = CreateSolidBrush(RGB(140, 140, 150));
		}
		else if (g_bAboutCloseBtnActivated)
		{
			pen = CreatePen(PS_SOLID, 4, RGB(140, 140, 150));
			brush = CreateSolidBrush(RGB(210, 210, 220));
		}
		else
		{
			pen = CreatePen(PS_SOLID, 0, RGB(210, 210, 220));
			brush = CreateSolidBrush(RGB(210, 210, 220));
		}
		SelectObject(hdc, pen);
		SelectObject(hdc, brush);
		Rectangle(hdc, 0, 0, clientRect.right + 1, clientRect.bottom + 1);
		DeleteObject(pen);
		DeleteObject(brush);

		hFont = CreateFontA(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH, "微软雅黑");
		SelectObject(hdc, hFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		if (g_bAboutCloseBtnPressed)
			SetBkColor(hdc, RGB(140, 140, 150));
		else
			SetBkColor(hdc, RGB(210, 210, 220));
		DrawText(hdc, TEXT("好的"), -1, &clientRect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		DeleteObject(hFont);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_MOUSEMOVE:
	{
		TRACKMOUSEEVENT trackMouseEvent;
		trackMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
		trackMouseEvent.dwFlags = TME_LEAVE;
		trackMouseEvent.hwndTrack = hWnd;
		TrackMouseEvent(&trackMouseEvent);

		g_bAboutCloseBtnActivated = TRUE;
		InvalidateRect(hWnd, &clientRect, FALSE);
	}
	break;
	case WM_MOUSELEAVE:
	{
		g_bAboutCloseBtnActivated = FALSE;
		g_bAboutCloseBtnPressed = FALSE;
		InvalidateRect(hWnd, &clientRect, FALSE);
	}
	break;
	case WM_LBUTTONDOWN:
	{
		g_bAboutCloseBtnPressed = TRUE;
		InvalidateRect(hWnd, &clientRect, FALSE);
	}
	break;
	case WM_LBUTTONUP:
	{
		g_bAboutCloseBtnPressed = FALSE;
		InvalidateRect(hWnd, &clientRect, FALSE);
		
		ShowWindow(GetParent(hWnd), SW_HIDE);
		g_bAboutShown = FALSE;

		RECT startBtnClientRect;
		GetClientRect(hWndStartBtn, &startBtnClientRect);
		InvalidateRect(hWndStartBtn, &startBtnClientRect, TRUE);

		RECT mainWndClientRect;
		GetClientRect(hWndMain, &mainWndClientRect);
		InvalidateRect(hWndMain, &mainWndClientRect, TRUE);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
