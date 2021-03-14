#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <windowsx.h>
#include <atlstr.h>

#define IDD_CLEAR 1332
bool start;
const int nWeight = 600, nHeight = 600;
WORD xPos, yPos, nSize;
void Rendering(HDC, HWND);
void Fill(HWND, HDC);
void RenderLine(HDC, HWND);
int xText, yText;
int provX, provY;
POINT pt;
RECT r;
HINSTANCE hInst;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.lpszClassName = L"WindowClass";

	hInst = wc.hInstance;

	if (!RegisterClassEx(&wc))
		return EXIT_FAILURE;

	HWND hWnd = CreateWindowEx(0, L"WindowClass", L"paint", WS_SYSMENU | WS_MINIMIZEBOX,
		(GetSystemMetrics(SM_CXSCREEN) - nWeight) / 2, (GetSystemMetrics(SM_CYSCREEN) - nHeight) / 2,
		nWeight, nHeight, 0, 0, hInstance, 0);
	ShowWindow(hWnd, nCmdShow);
	MSG msg;

	HDC hdc;

	while (true)
	{
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT) return 0;
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc; // контекст устройства
	PAINTSTRUCT ps; 

	switch (message)
	{
	case WM_MOUSEMOVE:
	{
		RECT r2;
		hdc = GetDC(hWnd);
		TCHAR szBuf[20];

		GetCursorPos(&pt);
	    ScreenToClient(hWnd, &pt);

		xPos = pt.x;
		yPos = pt.y;

		xText = 10;
		yText = 0;
		
		r2.left = xText;
		r2.top = yText;
		r2.right = 80;
		r2.bottom = 16;

		SelectObject(hdc, GetStockObject(DC_PEN));
		SetDCBrushColor(hdc, RGB(255, 255, 255));
		Rectangle(hdc, r2.left, r2.top, r2.right, r2.bottom);

		if (xPos <= nWeight && yPos <= nHeight) {
			nSize = wsprintf(szBuf, TEXT("%d, %d"), xPos, yPos);
			TextOut(hdc, xText, yText, szBuf, nSize);
		}

		short Sboolean;
		if (start == true)
		{
			Sboolean = 1;
		}
		else Sboolean = 0;

		nSize = wsprintf(szBuf, TEXT("pen = %hx"), Sboolean);
		TextOut(hdc, 20, 50, szBuf, nSize);

		if (start == true) 
		{
			Rendering(hdc, hWnd);
			Fill(hWnd, hdc);
		}

		DeleteObject(hdc);
	}
	return 0;

	case WM_LBUTTONUP:
	{
		start = false;
	}
	return 0;

	case WM_LBUTTONDOWN:
	{
		hdc = GetDC(hWnd);
		Fill(hWnd, hdc);
		start = true;
	}
	return 0;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	return 0;

	case WM_CLOSE:
	{
		DestroyWindow(hWnd);
	}
	return 0;

	case WM_COMMAND: 
	{
		int wnId = LOWORD(wParam);

		switch (wnId)
		{
		case IDD_CLEAR: 
		{
			hdc = GetDC(hWnd);
			RECT r;

			GetClientRect(hWnd, &r);
			FillRect(hdc, &r, (HBRUSH)(COLOR_WINDOW + 1));
		}
		break;

		default:
		    break;
		}
	}

	case WM_CREATE: 
	{
		HWND hwBtn = CreateWindow(L"button", L"Clear",
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			100, 0, 120, 30, hWnd, reinterpret_cast<HMENU>(IDD_CLEAR), hInst, NULL);
		ShowWindow(hwBtn, SW_SHOWNORMAL);
	}
	return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

BOOL Line(HDC hdc, int x1, int y1, int x2, int y2)
{
	MoveToEx(hdc, x1, y1, NULL); //сделать текущими координаты x1, y1
	return LineTo(hdc, x2, y2);
}

void Fill(HWND hWnd, HDC hdc)
{
	GetCursorPos(&pt);
	ScreenToClient(hWnd, &pt);

	r.left = pt.x - 10;
	r.top = pt.y - 10;
	r.right = pt.x;
	r.bottom = pt.x;

	provX = pt.x;
	provY = pt.y;
}

void Rendering(HDC hdc, HWND hWnd)
{
	RenderLine(hdc, hWnd);
}

void RenderLine(HDC hdc, HWND hWnd)
{
	GetCursorPos(&pt);
	ScreenToClient(hWnd, &pt);

	Line(hdc, provX, provY, pt.x, pt.y);

	DeleteObject(hdc);
}
