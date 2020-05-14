// Lab6_2.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Lab6_2.h"
#include <string>
#include <stdio.h>
using namespace std;

#define MAX_LOADSTRING 100
#define TEXT_SIZE 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

DWORD substringThreadId;
HANDLE substringThread;

TCHAR text[TEXT_SIZE];
TCHAR substring[TEXT_SIZE];

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    SearchSubstring(HWND, UINT, WPARAM, LPARAM);
DWORD               KMPSubstring(LPVOID);    // Алгоритм Кнута-Морриса-Пратта (КМП)

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
    LoadStringW(hInstance, IDC_LAB62, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB62));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB62));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB62);
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
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case ID_SUBSTRING_SEARCH:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_SUBSTRING), hWnd, SearchSubstring);
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

INT_PTR CALLBACK SearchSubstring(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_BUTTON1)
        {
            GetDlgItemText(hDlg, IDC_EDIT_TEXT, text, TEXT_SIZE);
            GetDlgItemText(hDlg, IDC_EDIT_SUBSTRING, substring, TEXT_SIZE);
            substringThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)KMPSubstring, (LPVOID) hDlg, 0, &substringThreadId);
            return (INT_PTR)TRUE;
        }
        if (LOWORD(wParam) == IDCANCEL)
        {
            TerminateThread(substringThread, 0);     //прерывает выполнения потока
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

DWORD KMPSubstring(LPVOID param)
{
    HWND hList = GetDlgItem((HWND)param, IDC_EDIT_RESULT);
    int pi[ARRAYSIZE(text)];
    wchar_t a[50];

    pi[0] = 0;     // в i-м элементе (его индекс i-1) количество совпавших 
             // символов в начале образца и в конце подстроки длины i. 
             // p[0]=0 всегда, p[1]=1, если начинается с двух одинаковых 
    int l;    // будет длина образца
    // заполняем массив длин префиксов для образца
    for (l = 1; substring[l]; ++l)
    {
        int j = pi[l - 1];
        while ((j > 0) && (substring[l] != substring[j])) // не равны
            j = pi[j - 1];	         // берем ранее рассчитанное значение (начиная с максимально возможных)
        if (substring[l] == substring[j])      // равны 
            ++j;
        pi[l] = j;
    }
    int j = 0; // количество совпавших символов, оно же индекс сравниваемого 
    // символа в образце. В строке сравниваемый символ будет иметь индекс i
    for (int i = 0; text[i]; ++i)
    {
        while ((j > 0) && (text[i] != substring[j]))
            // Очередной символ строки не совпал с символом в образце. Сдвигаем образец, 
            // причем точно знаем, что первые j символов образца совпали с символами строки 
            // и надо сравнить j+1й символ образца (его индекс j) с i+1м символом строки.
            j = pi[j - 1];    // если j=0, то достигли начала образца и цикл следует прервать

        if (text[i] == substring[j]) // есть совпадение очередного символа 
            ++j;              // увеличиваем длину совпавшего фрагмента на 1
        if (j == l)
        {
            swprintf_s(a, L"%d", i - l + 1);
            SendMessage(hList, LB_ADDSTRING, 0, LPARAM(a));
            Sleep(500);
        }
    }
    return 0;
}