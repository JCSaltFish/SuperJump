// core.cpp: 应用程序核心算法。
//

#include "stdafx.h"
#include "core.h"

SuperJump::SuperJump()
{
	m_bStop = TRUE;

	m_Distant = 0;
	m_Delta = 1.2f;
	m_Time = 100;
}

SuperJump::~SuperJump() { }

BOOL SuperJump::begin()
{
	m_bStop = FALSE;
	HANDLE hThread = ::CreateThread(NULL, 0, mainThreadProc, (LPVOID)this, 0, NULL);
	if (FAILED(hThread))
		return FALSE;
	return TRUE;
}

void SuperJump::mainProc()
{
	takeShot();
	if (!getDistant())
	{
		stop();
		return;
	}
	getTime();
	operate();
}

void SuperJump::stop()
{
	m_bStop = TRUE;
}

BOOL SuperJump::isStopped()
{
	return m_bStop;
}

int SuperJump::getDelta()
{
	return m_Delta;
}

void SuperJump::setDelta(int delta)
{
	m_Delta = delta;
}

HWND SuperJump::getHWnd()
{
	return m_hWnd;
}

void SuperJump::setHWnd(HWND hWnd)
{
	m_hWnd = hWnd;
}

void SuperJump::takeShot()
{
	ShellExecute(NULL, NULL, TEXT("data\\adb.exe"),
		TEXT("shell screencap -p /sdcard/jump_step.png"), NULL, SW_HIDE);
	Sleep(2000);
	ShellExecute(NULL, NULL, TEXT("data\\adb.exe"),
		TEXT("pull /sdcard/jump_step.png data/jump_step.png"), NULL, SW_HIDE);
	Sleep(500);
	ShellExecute(NULL, NULL, TEXT("data\\adb.exe"),
		TEXT("shell rm /sdcard/jump_step.png"), NULL, SW_HIDE);
	Sleep(500);
}

BOOL SuperJump::getDistant()
{
	// get image
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	wstring imageFile(L"data/jump_step.png");
	Bitmap* pBitmap = new Bitmap(imageFile.c_str());
	if (pBitmap->GetLastStatus() != Ok)
	{
		delete pBitmap;
		GdiplusShutdown(gdiplusToken);
		return FALSE;
	}

	UINT height = pBitmap->GetHeight();
	UINT width = pBitmap->GetWidth();

	Color color;

	UINT yStart = height / 4;
	UINT yEnd = yStart * 3;

	// find origin
	int orR = 54;
	int orG = 60;
	int orB = 102;
	UINT xOrigin = 0;
	UINT yOrigin = 0;
	for (UINT y = yEnd; y > height / 2; y--)
	{
		BOOL bFound = FALSE;
		for (UINT x = 0; x < width; x++)
		{
			pBitmap->GetPixel(x, y, &color);
			int pxR = (int)color.GetRed();
			int pxG = (int)color.GetGreen();
			int pxB = (int)color.GetBlue();
			if (fabs(pxR - orR) + fabs(pxG - orG) + fabs(pxB - orB) < 30)
			{
				xOrigin = x;
				yOrigin = y;
				bFound = TRUE;
				break;
			}
		}
		if (bFound)
			break;
	}

	// find distination
	Color colorBg;
	pBitmap->GetPixel(5, yStart, &colorBg);
	int bgR = (int)colorBg.GetRed();
	int bgG = (int)colorBg.GetGreen();
	int bgB = (int)colorBg.GetBlue();
	UINT xDest = 0;
	UINT yDest = 0;
	for (UINT y = yStart; y < height / 2; y++)
	{
		BOOL bFound = FALSE;
		for (UINT x = 0; x < width; x++)
		{
			pBitmap->GetPixel(x, y, &color);
			int pxR = (int)color.GetRed();
			int pxG = (int)color.GetGreen();
			int pxB = (int)color.GetBlue();
			if (fabs(pxR - bgR) + fabs(pxG - bgG) + fabs(pxB - bgB) > 30)
			{
				if (fabs(x - xOrigin) < 20)
					break;
				xDest = x;
				yDest = y;
				bFound = TRUE;
				break;
			}
		}
		if (bFound)
			break;
	}

	delete pBitmap;
	GdiplusShutdown(gdiplusToken);

	// calculate
	m_Distant = (xDest - xOrigin) * (xDest - xOrigin);
	m_Distant += (yDest - yOrigin) * (yDest - yOrigin);
	m_Distant = sqrt(m_Distant);

	return TRUE;
}

void SuperJump::getTime()
{
	m_Time = m_Distant * m_Delta;
}

void SuperJump::operate()
{
	TCHAR paramBuffer[128] = _T("");
	swprintf_s(paramBuffer, _T("shell input swipe 500 500 500 500 %d"), m_Time);
	ShellExecute(NULL, NULL, TEXT("data\\adb.exe"), paramBuffer, NULL, SW_HIDE);
}

DWORD WINAPI mainThreadProc(LPVOID lpParameter)
{
	SuperJump* pThis = (SuperJump*)lpParameter;

	while (!pThis->isStopped())
	{
		pThis->mainProc();
		Sleep(3000);
	}

	DeleteFile(TEXT("data/jump_step.png"));
	ShellExecute(NULL, NULL, TEXT("data\\adb.exe"),
		TEXT("shell rm /sdcard/jump_step.png"), NULL, SW_HIDE);

	PostMessage(pThis->getHWnd(), WM_COMPLETE, 0, 0);

	return 0;
}
