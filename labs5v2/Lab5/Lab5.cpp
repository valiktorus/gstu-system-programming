// Lab5.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Lab5.h"
#include <stdio.h>

#define MAX_LOADSTRING 100

struct Calculator {
    int array[50];
    int A;
    int B;
    int mult;
    int negativeNumberCount;
    int arraySize;
};
typedef VOID(WINAPI* PFN) (Calculator&);

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    ArrayProc(HWND, UINT, WPARAM, LPARAM);

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
    LoadStringW(hInstance, IDC_LAB5, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB5));

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
    wcex.hIcon          = hIcon;
    wcex.hCursor        = hCursor;
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB5);
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
        SetClassLong(hWnd, GCL_HICONSM, (LONG)hIcon);
        SetClassLong(hWnd, GCL_HCURSOR, (LONG)hCursor);

        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case ID_CALCULATE:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, ArrayProc);
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
            HDC memDC = CreateCompatibleDC(hdc);
            SelectObject(memDC, hBitmap);
            BitBlt(hdc, 100, 100, 292, 250, memDC, 0, 0, SRCCOPY);

            // TODO: Add any drawing code that uses hdc here...
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

INT_PTR CALLBACK ArrayProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);

    struct Calculator calculator;

    static HINSTANCE hLibrary;
    static HWND hList;
    static PFN Calculate;
    static int i = 0;
    static int n = 0;
    static int inbuf[50];
    TCHAR buf[50];
    TCHAR bufDisplay[200];

    BOOL* pr = 0;
    TCHAR bt[5];

    switch (message)
    {
    case WM_CLOSE:
        FreeLibrary(hLibrary);
        break;
    case WM_INITDIALOG:
        hLibrary = LoadLibrary(_T("Lab5Dll2.dll"));
        hList = GetDlgItem(hDlg, IDC_LIST1);
        if (hLibrary)
        {
            MessageBox(hDlg, _T("Библиотека найдена"), _T("Загрузка"), MB_OK);
            Calculate = (PFN) GetProcAddress(hLibrary, "Calculate");
            if (Calculate != NULL)
            {
                MessageBox(hDlg, _T("Функция найдена"), _T("Загрузка"), MB_OK);
            }
            else
            {
                MessageBox(hDlg, _T("Ошибка: функция не найдена"), _T("Ошибка"), MB_OK);
            }
        }
        else
        {
            MessageBox(hDlg, _T("Ошибка: библиотека не найдена"), _T("Ошибка"), MB_OK);
        }
        return (INT_PTR)TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_BUTTON1:
            inbuf[i++] = GetDlgItemInt(hDlg, IDC_EDIT1, 0, 1);

            GetDlgItemText(hDlg, IDC_EDIT1, buf, wcslen(buf));
            SendMessage(hList, LB_ADDSTRING, 0, LPARAM(buf));
            n = i;
            break;
        case IDC_BUTTON2:
            for (int i = 0; i < n; i++)
            {
                calculator.array[i] = inbuf[i];
            }
            calculator.A = GetDlgItemInt(hDlg, IDC_EDIT2, 0, 1);
            calculator.B = GetDlgItemInt(hDlg, IDC_EDIT3, 0, 1);
            calculator.arraySize = n;
            Calculate(calculator);
            SetDlgItemInt(hDlg, IDC_EDIT4, calculator.mult, 1);
            SetDlgItemInt(hDlg, IDC_EDIT5, calculator.negativeNumberCount, 1);
            break;
        case IDCANCEL:
            n = 0;
            i = 0;
            EndDialog(hDlg, LOWORD(wParam));
            break;
        }
    }
    return (INT_PTR)FALSE;
}
