// Lab4.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Lab4.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB4, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB4));

    MSG msg;

    // Main message loop:
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
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB4));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB4);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
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
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

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
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
int maxX, maxY;
HDC memdc;
HBITMAP hbit;
HBRUSH hbrush, hGreenBrush, hRedBrush, hOldBrush;
HPEN hpen, hBlackpen;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    switch (message)
    {
    case WM_CREATE:
       
        maxX = GetSystemMetrics(SM_CXSCREEN);
        maxY = GetSystemMetrics(SM_CYSCREEN);
        hdc = GetDC(hWnd);
        memdc = CreateCompatibleDC(hdc);
        hbit = CreateCompatibleBitmap(hdc, maxX, maxY);
        SelectObject(memdc, hbit);
        hbrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
        PatBlt(memdc, 0, 0, maxX, maxY, PATCOPY);


        hGreenBrush = CreateSolidBrush(RGB(0, 255, 0));
        hRedBrush = CreateSolidBrush(RGB(255, 0, 0));

        hBlackpen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
        

        hpen = (HPEN)SelectObject(memdc, hBlackpen);
        (HPEN)SelectObject(memdc, hpen);

        ReleaseDC(hWnd, hdc);//высвобождаем котекст устройства
        break;

    case WM_COMMAND:
        {

            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case ID_FIGURE:
                hOldBrush = (HBRUSH)SelectObject(memdc, hRedBrush);
                
                POINT ArrayPoints1[3] , ArrayPoints2[3], ArrayPoints3[3], ArrayPoints4[3];

                ArrayPoints1[0].x = 100; ArrayPoints1[0].y = 200;
                ArrayPoints1[1].x = 140; ArrayPoints1[1].y = 210;
                ArrayPoints1[2].x = 130; ArrayPoints1[2].y = 170;
                SelectObject(memdc, hBlackpen);
                SelectObject(memdc, hRedBrush);
                Polygon(memdc, ArrayPoints1, 3);

                ArrayPoints2[0].x = 120; ArrayPoints2[0].y = 160;
                ArrayPoints2[1].x = 160; ArrayPoints2[1].y = 170;
                ArrayPoints2[2].x = 150; ArrayPoints2[2].y = 130;
                SelectObject(memdc, hBlackpen);
                Polygon(memdc, ArrayPoints2, 3);

                ArrayPoints3[0].x = 150; ArrayPoints3[0].y = 205;
                ArrayPoints3[1].x = 270; ArrayPoints3[1].y = 205;
                ArrayPoints3[2].x = 210; ArrayPoints3[2].y = 140;
                SelectObject(memdc, hBlackpen);
                Polygon(memdc, ArrayPoints3, 3);

                (HBRUSH)SelectObject(memdc, hGreenBrush);
                SelectObject(memdc, hGreenBrush);
                ArrayPoints4[0].x = 290; ArrayPoints4[0].y = 130;
                ArrayPoints4[1].x = 275; ArrayPoints4[1].y = 200;
                ArrayPoints4[2].x = 340; ArrayPoints4[2].y = 210;
                SelectObject(memdc, hBlackpen);
                Polygon(memdc, ArrayPoints4, 3);
                (HBRUSH)SelectObject(memdc, hOldBrush);
                InvalidateRect(hWnd, NULL, 1);
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
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            BitBlt(hdc, 0, 0, maxX, maxY, memdc, 0, 0, SRCCOPY);//перерисовка окна  
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
