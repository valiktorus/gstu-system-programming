// Lab6_1.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Lab6_1.h"
#include <string> 

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

TCHAR semaphoreName[] = TEXT("MySemaphore");
HDC hdc, memdc;
HBITMAP hbitmap;
DWORD fillArrayThreadId, arrayCorrectorThreadId, startDisplayingThreadId;  //идентификатор потока для рандома идентификатор для потока замены
HANDLE fillArrayThread, arrayCorrectorThread, startDisplayingThread;
HANDLE hSemaphore = CreateSemaphore(NULL, 0, 1, semaphoreName);

wchar_t str[10];
int x = 0, y = 0;
int arr[10];

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
DWORD               fillArray(LPVOID);
DWORD               ArrayCorrector(LPVOID);
DWORD               StartDisplaying(LPVOID); 

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
    LoadStringW(hInstance, IDC_LAB61, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB61));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB61));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB61);
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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        hdc = GetDC(hWnd); // выделение контекста
        memdc = CreateCompatibleDC(hdc);// обращение к участку виртуальной памяти
        hbitmap = CreateCompatibleBitmap(hdc, 1000, 1000); // размеры 
        SelectObject(memdc, hbitmap); // 
        PatBlt(memdc, 0, 0, 1000, 1000, PATCOPY);
        ReleaseDC(hWnd, hdc);
        fillArrayThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)fillArray, (LPVOID) NULL, 0, &fillArrayThreadId);
        arrayCorrectorThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ArrayCorrector, (LPVOID) NULL, 0, &arrayCorrectorThreadId);
        startDisplayingThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) StartDisplaying , (LPVOID) NULL, 0, &startDisplayingThreadId);
        ReleaseSemaphore(hSemaphore, 1, NULL);
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
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
            BitBlt(hdc, 0, 0, 1000, 1000, memdc, 0, 0, SRCCOPY);
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

DWORD fillArray(LPVOID param)
{
    while (true)
    {
        WaitForSingleObject(hSemaphore, INFINITE);
        for (int i = 0; i < ARRAYSIZE(arr); i++)
        {
            arr[i] = (double)rand() / (RAND_MAX + 1) * 300 - 150;
        }
        ReleaseSemaphore(hSemaphore, 1, NULL);
        Sleep(100);
    }
    return 0;
}

DWORD ArrayCorrector(LPVOID param)
{
    while (true)
    {
        WaitForSingleObject(hSemaphore, INFINITE);
        int sum = 0;
        int size = ARRAYSIZE(arr);
        for (int i = 0; i < size ; i++)
        {
            if ((arr[i] > 99 && arr[i] < 999) || (arr[i] < 99 && arr[i] > 1000))
            {
                arr[i] = arr[i] % 100;
            }
        }
        ReleaseSemaphore(hSemaphore, 1, NULL);
        Sleep(100);
    }
    return 0;
}

DWORD StartDisplaying(LPVOID param) {
    while (true)
    {
        WaitForSingleObject(hSemaphore, INFINITE);
        for (int i = 0; i < ARRAYSIZE(arr); i++)
        {
            swprintf_s(str, TEXT("|  %4d"), arr[i]); 
            TextOut(memdc, x, y, str, wcslen(str));
            InvalidateRect(NULL, NULL, true); 
            x += 50;
        }
        x = 0;
        y += 20;
        ReleaseSemaphore(hSemaphore, 1, NULL);
        Sleep(1000);
    }
   
    return 0;
}
