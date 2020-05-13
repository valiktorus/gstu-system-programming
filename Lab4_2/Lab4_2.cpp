// Lab4_2.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Lab4_2.h"
#include <math.h>
//const int SPAN = 10;
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
    LoadStringW(hInstance, IDC_LAB42, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB42));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB42));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB42);
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
int  diameter = 76;
int xPosInBowler = 0, yPosInBowler = diameter / 2;
bool xMoveUp = true, yMoveUp = false;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int x, y, cx, cy, scrx, scry, xSpoonStart, ySpoonStart, xStartBowlerHat, yStartBowlerHat;
    const int delta = 50;//смещение относительно центра
    static double mod, xt, yt;           //координаты изображения, mod-выщитывает расстояние между координатами
    static HBITMAP hObject;
    static int vx = -2, vy = -2;// скорость движения изображения ложки
    static HDC hdc, memDCSpoon, memDCBowlerHat;
    static bool play;                     // определение скорости
    const int sizeX = 200, sizeY = 220;//размер изображения ложки
    int xStep, yStep;
    int mx, my; //определение позиции курсора

    switch (message)
    {
    case WM_CREATE:

        hdc = GetDC(hWnd);//контекст устройства

        hObject = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_SPOON_2));              
        memDCSpoon = CreateCompatibleDC(hdc);                    //выделение памяти под контекст ус-ва
        SelectObject(memDCSpoon, hObject);                        //загрузка в память изображения

        hObject = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BOWLER_HAT));
        memDCBowlerHat = CreateCompatibleDC(hdc);
        SelectObject(memDCBowlerHat, hObject);

        ReleaseDC(hWnd, hdc);
        break;
    case WM_SIZE:
        //Получение размеров окна
        scrx = LOWORD(lParam);
        scry = HIWORD(lParam);
        x = (cx = scrx / 2) + delta ;
        y = (cy = scry / 2) + delta;
        xSpoonStart = x - 280;
        ySpoonStart = y - 215;
        xStartBowlerHat = x - 500;
        yStartBowlerHat = y - 300;
        x = xSpoonStart;
        y = ySpoonStart;
        /*xCicleCenter = x + 20;
        yCicleCenter = y + 20;*/
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case ID_START:
                SetTimer(hWnd, 1, (int)(sqrt(double(cx * cx + cy * cy)) / mod) * 10, NULL);//тик
                xt = x;
                yt = y;
                InvalidateRect(hWnd, NULL, TRUE);   //  перерисовка области
                break;
            case ID_FINISH:
                KillTimer(hWnd, 1);
                x = xSpoonStart;
                y = ySpoonStart;
                xPosInBowler = 0;
                yPosInBowler = diameter / 2;
                xMoveUp = true;
                yMoveUp = false;
                InvalidateRect(hWnd, NULL, TRUE);   //  перерисовка области
                play = false;
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
    
    case WM_TIMER:
        hdc = GetDC(hWnd);
        BitBlt(hdc, x, y, delta, delta, NULL, 0, 0, PATCOPY);
        if(xMoveUp){
            x++;
            xPosInBowler++;
        }
        else
        {
            x--;
            xPosInBowler--;
        }
        if (yMoveUp) {
            y++;
            yPosInBowler++;
        }
        else
        {
            y--;
            yPosInBowler--;
        }
        if (xPosInBowler == diameter) {
            xMoveUp = false;
        }
        if (yPosInBowler == diameter)
        {
            yMoveUp = false;
        }
        if (xPosInBowler == 0) {
            xMoveUp = true;
        }
        if (yPosInBowler == 0)
        {
            yMoveUp = true;
        }
        BitBlt(hdc, xStartBowlerHat, yStartBowlerHat, 500, 500, memDCBowlerHat, 0, 0, SRCCOPY);
        BitBlt(hdc, x, y, sizeX, sizeY - 70, memDCSpoon, 0, 0, SRCCOPY);
        ReleaseDC(hWnd, hdc);
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            BitBlt(hdc, xStartBowlerHat, yStartBowlerHat,500,500, memDCBowlerHat, 0, 0, SRCCOPY);
            BitBlt(hdc, xSpoonStart, ySpoonStart, sizeX, sizeY, memDCSpoon, 0, 0, SRCCOPY);

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
