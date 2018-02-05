#pragma once

#include <iostream>
#include <Windows.h>

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

#define WM_COMPLETE (WM_USER + 0)

using namespace std;
using namespace Gdiplus;

class SuperJump
{
private:
	BOOL m_bStop;

	int m_Distant;
	float m_Delta;
	int m_Time;

	HWND m_hWnd;

public:
	SuperJump();
	~SuperJump();

public:
	BOOL begin();
	void mainProc();
	void stop();
	BOOL isStopped();

	int getDelta();
	void setDelta(int delta);

	HWND getHWnd();
	void setHWnd(HWND hwnd);

private:
	void takeShot();
	BOOL getDistant();
	void getTime();
	void operate();
};

DWORD WINAPI mainThreadProc(LPVOID lpParameter);
