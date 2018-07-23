// Sketchpad.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Sketchpad.h"

#define MAX_LOADSTRING 100

typedef struct
{
	unsigned int left;
	unsigned int top;
	unsigned int right;
	unsigned int bottom;
	unsigned int colour;
}UserRect;

typedef struct
{
	unsigned int left;
	unsigned int top;
	unsigned int right;
	unsigned int bottom;
	unsigned int colour;
}UserEllipse;

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
POINT				FreeDraw(HWND, POINT, LPARAM, BOOL, int, int);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SKETCHPAD, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SKETCHPAD));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
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
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SKETCHPAD));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_SKETCHPAD);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	static HDC saveDC;
	static HBITMAP saveScreen;
	static RECT client;
	static int clientWidth;
	static int	clientHeight;
	static RECT rectEraser;
	static BOOL eraser = FALSE;
	static BOOL erase = FALSE;
	static POINT point;
	static BOOL freeDraw = FALSE;
	static BOOL draw = FALSE;
	static BOOL lButtonUp = FALSE;
	static BOOL drawLine = FALSE;
	static BOOL line = FALSE;
	static BOOL drawEmpRectangle = FALSE;
	static BOOL userRectangleCheck = FALSE;
	static BOOL lastDrawRect = FALSE;
	static UserRect userRectangle;
	static BOOL drawEmpEllipse = FALSE;
	static BOOL userEllipseCheck = FALSE;
	static BOOL lastDrawEllipse = FALSE;
	static BOOL emptyRect = FALSE;
	static BOOL empCircle = FALSE;
	static UserEllipse userEllipse;
	static int colour = RGB(0,0,0);
	static int lineSize = 1;
	static POINT point2;
	
	


	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_FREEDRAW:
			drawEmpEllipse = FALSE;
			freeDraw = TRUE;
			emptyRect = FALSE;
			empCircle = FALSE;
			drawLine = FALSE;
			eraser = FALSE;
			drawEmpRectangle = FALSE;
			break;
		case IDM_LINE:
			drawEmpEllipse = FALSE;
			emptyRect = FALSE;
			empCircle = FALSE;
			freeDraw = FALSE;
			drawLine = TRUE;
			eraser = FALSE;
			drawEmpRectangle = FALSE;
			break;
		case IDM_EMPRECTANGLE:
			drawEmpRectangle = TRUE;
			emptyRect = TRUE;
			freeDraw = FALSE;
			drawLine = FALSE;
			empCircle = FALSE;
			eraser = FALSE;
			drawEmpEllipse = FALSE;
			break;
		case IDM_FILLRECTANGLE:
			drawEmpRectangle = TRUE;
			emptyRect = FALSE;
			drawEmpEllipse = FALSE;
			empCircle = FALSE;
			drawLine = FALSE;
			freeDraw = FALSE;
			eraser = FALSE;
			break;
		case IDM_EMPCIRCLE:
			drawEmpEllipse = TRUE;
			freeDraw = FALSE;
			drawLine = FALSE;
			empCircle = TRUE;
			emptyRect = FALSE;
			eraser = FALSE;
			drawEmpRectangle = FALSE;
			break;
		case IDM_FILLCIRCLE:
			drawEmpEllipse = TRUE;
			emptyRect = FALSE;
			empCircle = FALSE;
			freeDraw = FALSE;
			drawLine = FALSE;
			eraser = FALSE;
			drawEmpRectangle = FALSE;
			break;
		case IDM_ERASER:
			drawEmpEllipse = FALSE;
			emptyRect = FALSE;
			freeDraw = FALSE;
			drawLine = FALSE;
			drawEmpRectangle = FALSE;
			eraser = TRUE;
			hdc=GetDC(hWnd);
			BitBlt(saveDC, client.left, client.top, clientWidth, clientHeight, hdc, client.left, client.top, SRCCOPY);
			break;
		case IDM_ERASEALL:
			DeleteObject(saveDC);
			DeleteObject(saveScreen);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case IDM_BLACK:
			colour = RGB(0,0,0);
			break;
		case IDM_RED:
			colour = RGB(255,0,0);
			break;
		case IDM_GREEN:
			colour = RGB(0,255,0);
			break;
		case IDM_BLUE:
			colour = RGB(0,0,255);
			break;
		case IDM_ORANGE:
			colour = RGB(255,165,0);
			break;
		case IDM_PURPLE:
			colour = RGB(125,38,205);
			break;
		case IDM_THIN:
			lineSize = 1;
			break;
		case IDM_MEDIUM:
			lineSize = 5;
			break;
		case IDM_THICK:
			lineSize = 12;
			break;

		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_LBUTTONDOWN:
		{
		if(!eraser)
		{
			hdc = GetDC(hWnd);
			saveDC = CreateCompatibleDC(hdc);
			saveScreen = CreateCompatibleBitmap(hdc, clientWidth, clientHeight);
			SelectObject(saveDC, saveScreen);
			BitBlt(saveDC, client.left, client.top, clientWidth, clientHeight, hdc, client.left, client.top, SRCCOPY);
		}
		if(freeDraw)
		{
			draw = TRUE;
			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);
		}
		if(drawLine)
		{
			line = TRUE;
			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);
		}
		if(drawEmpRectangle)
		{
			userRectangleCheck = TRUE;
			userRectangle.left = LOWORD(lParam);
			userRectangle.top = HIWORD(lParam);
			userRectangle.colour = colour;
		}
		if(drawEmpEllipse)
		{
			userEllipseCheck = TRUE;
			userEllipse.left = LOWORD(lParam);
			userEllipse.top = HIWORD(lParam);
			userEllipse.colour = colour;
		}
		if(eraser)
		{
			userEllipseCheck = FALSE;
			userRectangleCheck = FALSE;
			erase = TRUE;
		}
		}
		break;
		

	case WM_MOUSEMOVE:
		if(draw)
		{
			point = FreeDraw(hWnd, point, lParam, lButtonUp, colour, lineSize);
		}
		if(line)
		{
			point2.x = LOWORD(lParam);
			point2.y = HIWORD(lParam);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		if(userRectangleCheck)
		{
			userRectangle.right = LOWORD(lParam);
			userRectangle.bottom = HIWORD(lParam);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		if(userEllipseCheck)
		{
			userEllipse.right = LOWORD(lParam);
			userEllipse.bottom = HIWORD(lParam);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		if(eraser)
		{
			rectEraser.left = (LOWORD(lParam) - 10);
			rectEraser.top = (HIWORD(lParam) - 10);
			rectEraser.right = (LOWORD(lParam) + 10);
			rectEraser.bottom = (HIWORD(lParam)+ 10);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;

	case WM_LBUTTONUP:
		if(draw) 
		{
			lButtonUp = TRUE;
			point = FreeDraw(hWnd, point, lParam, lButtonUp, colour, lineSize);
			draw = FALSE;
		}
		if(line)
		{
			lButtonUp = TRUE;
			point2.x = LOWORD(lParam);
			point2.y = HIWORD(lParam);
			InvalidateRect(hWnd, NULL, TRUE);
			line = FALSE;
		}
		if(userRectangleCheck)
		{
			lastDrawRect = TRUE;
			userRectangle.right = LOWORD(lParam);
			userRectangle.bottom = HIWORD(lParam);
			InvalidateRect(hWnd, NULL, TRUE);
			userRectangleCheck = FALSE;
		}
		if(userEllipseCheck)
		{
			lastDrawEllipse = TRUE;
			userEllipse.right = LOWORD(lParam);
			userEllipse.bottom = HIWORD(lParam);
			InvalidateRect(hWnd, NULL, TRUE);
			userEllipseCheck = FALSE;
		}
		if(erase)
		{
			erase = FALSE;
		}
		lButtonUp = FALSE;
		break;

	case WM_PAINT:
		{

		if (GetClientRect(hWnd, &client))
		{
			clientWidth = client.right - client.left;
			clientHeight = client.bottom - client.top;
		}

		hdc = BeginPaint(hWnd, &ps);
		HDC backbuffDC = CreateCompatibleDC(hdc);
		HBITMAP backbuffer = CreateCompatibleBitmap(hdc, clientWidth, clientHeight);
 		SelectObject(backbuffDC, backbuffer);
		HGDIOBJ brush = SelectObject(backbuffDC, GetStockObject(DC_BRUSH));
		HPEN whitePen = CreatePen(PS_NULL, 1, RGB(255,255,255));	
		HPEN userPen = CreatePen(PS_SOLID, lineSize, colour);
		SelectObject(backbuffDC, whitePen);
		Rectangle(backbuffDC, client.left, client.top, client.right, client.bottom);
		BitBlt(backbuffDC, client.left, client.top, clientWidth, clientHeight, saveDC, client.left, client.top, SRCCOPY);
		SelectObject(backbuffDC, userPen);
		


		if(userRectangleCheck)
		{
			if(!emptyRect)
			{
			SetDCBrushColor(backbuffDC, userRectangle.colour);
			}
			else
			{
				SelectObject(backbuffDC, GetStockObject(NULL_BRUSH));
			}
			Rectangle(backbuffDC, userRectangle.left, userRectangle.top, userRectangle.right, userRectangle.bottom);
		}
		else if(lastDrawRect)
		{		
			if(!emptyRect)
			{
			SetDCBrushColor(backbuffDC, userRectangle.colour);
			}
			else
			{
				SelectObject(backbuffDC, GetStockObject(NULL_BRUSH));
			}
			Rectangle(backbuffDC, userRectangle.left, userRectangle.top, userRectangle.right, userRectangle.bottom);

			lastDrawRect = FALSE;
		}

		if(eraser)
		{
			if(erase)
			{
				SelectObject(backbuffDC, whitePen);
				Rectangle(backbuffDC, rectEraser.left, rectEraser.top, rectEraser.right, rectEraser.bottom);
				BitBlt(saveDC, client.left, client.top, clientWidth, clientHeight, backbuffDC, client.left, client.top, SRCCOPY);
				HPEN userPenLocal = CreatePen(PS_SOLID, 1, RGB(0,0,0));
				SelectObject(backbuffDC, userPenLocal);
				Rectangle(backbuffDC, rectEraser.left, rectEraser.top, rectEraser.right, rectEraser.bottom);
				DeleteObject(userPenLocal);
			}
			else
			{
				HPEN userPenLocal = CreatePen(PS_SOLID, 1, RGB(0,0,0));
				SelectObject(backbuffDC, userPenLocal);
				Rectangle(backbuffDC, rectEraser.left, rectEraser.top, rectEraser.right, rectEraser.bottom);
				DeleteObject(userPenLocal);
				InvalidateRect(hWnd, NULL, TRUE);
			}
		}

		if (lButtonUp)
		{ 	
			if (point.x == LOWORD(lParam) && point.y == HIWORD(lParam))
			{
				SetPixel(backbuffDC, point.x, point.y, RGB(0,0,0));
			}
			else
			{
				MoveToEx(backbuffDC, point.x, point.y, NULL); 
				LineTo(backbuffDC, point2.x, point2.y);
			}
		} 
		else
		{	
			if(drawLine)
			{
				MoveToEx(backbuffDC, point.x, point.y, NULL);
				LineTo(backbuffDC, point2.x, point2.y);
			}
		}


		if(userEllipseCheck)
		{
			if(!empCircle)
			{
				SetDCBrushColor(backbuffDC, userEllipse.colour);
			}
			else
			{
				SelectObject(backbuffDC, GetStockObject(NULL_BRUSH));
			}
			Ellipse(backbuffDC, userEllipse.left, userEllipse.top, userEllipse.right, userEllipse.bottom);
		}
		else if(lastDrawEllipse)
		{	
			if(!empCircle)
			{
				SetDCBrushColor(backbuffDC, userEllipse.colour);
			}
			else
			{
				SelectObject(backbuffDC, GetStockObject(NULL_BRUSH));
			}
			Ellipse(backbuffDC, userEllipse.left, userEllipse.top, userEllipse.right, userEllipse.bottom);

			lastDrawEllipse = FALSE;
		}
		
		BitBlt(hdc, client.left, client.top, clientWidth, clientHeight, backbuffDC, client.left, client.top, SRCCOPY);
		DeleteObject(brush);
		DeleteObject(whitePen);
		DeleteObject(userPen);
		DeleteObject(backbuffer);
		DeleteObject(backbuffDC);

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

//Routine to allow user to draw with mouse freely.
POINT FreeDraw(HWND hWnd, POINT point, LPARAM lParam, BOOL lButtonUp, int colour, int lineSize)
{
	HDC hdc = GetDC(hWnd);
	HPEN userPenLocal = CreatePen(PS_SOLID, lineSize, colour);
	SelectObject(hdc, userPenLocal);
	if (lButtonUp)
	{ 	
		if (point.x == LOWORD(lParam) && point.y == HIWORD(lParam))
		{
			SetPixel(hdc, point.x, point.y, colour);
		}
		else
		{
			MoveToEx(hdc, point.x, point.y, NULL); 
			LineTo(hdc, LOWORD(lParam), HIWORD(lParam));
		}
	} 
	else
	{
		MoveToEx(hdc, point.x, point.y, NULL); 
		LineTo(hdc, point.x = LOWORD(lParam), point.y = HIWORD(lParam));
	}	
	DeleteObject(userPenLocal);
	return point;
}

// Message handler for about box.
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
