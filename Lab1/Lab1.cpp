// Lab1.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Lab1.h"
#include <Commdlg.h>

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
    LoadStringW(hInstance, IDC_LAB1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB1));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg); //обработка из клавиатуры
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
ATOM MyRegisterClass(HINSTANCE hInstance) // рекистрация класса окна
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;   // имя процедуры в которой обрабатываются все меседжи
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)); // загрузка иконка
    wcex.hCursor        = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1)); //определение собственного окна
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB1); //имя меню
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1)); 

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
const DWORD MaxLength = 250;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) // обработка всех сообщений приходящихв окно
{
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;
    static OPENFILENAME file;
    static TCHAR name[MAX_PATH];
    static HANDLE hFile;
    BOOL success;
    int i = 0;
    int y = 0;
    DWORD dw = 0;
    static char text[MaxLength];

    HMENU hSubMenu, hMenu;
    int xpos, ypos;
    switch (message)
    {
    case WM_CREATE:
    {
        file.lStructSize = sizeof(OPENFILENAME);
        file.hInstance = hInst;
        file.lpstrFilter = L"Text\0*.txt";
        file.lpstrFile = name;
        file.nMaxFile = 256;
        file.lpstrInitialDir = L"D:\study\gstu\2_sem\system-programming\test-data";
        file.lpstrDefExt = L"xtx";
        break;
    }
    //контекстное меню
    case WM_RBUTTONDOWN:
    {
        xpos = LOWORD(lParam);
        ypos = HIWORD(lParam);
        hMenu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU1));
        hSubMenu = GetSubMenu(hMenu, 0);
        TrackPopupMenu(hSubMenu, TPM_LEFTALIGN, xpos + 40, ypos + 80, 0, hWnd, NULL);
        break;
    }
    case WM_COMMAND: //при кликах
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            //open file
            case ID_OPEN_FILE:
            {
                file.lpstrTitle = _T("Открыть файл для чтения");
                file.Flags = OFN_HIDEREADONLY;
                if (!GetOpenFileName(&file)) return 1;
                hFile = CreateFile(name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
                hdc = GetDC(hWnd);
                while (ReadFile(hFile, &text, sizeof(text), &dw, NULL) && dw != 0)
                {
                    TextOutA(hdc, 0, y, text, MaxLength);
                    y += 16;
                }
                ReleaseDC(hWnd, hdc);
                break;
            }
            case ID_SAVE_FILE:
            {
                success = GetSaveFileName(&file);
                if (success) 
                {
                    MessageBox(hWnd, file.lpstrFile, TEXT("Файл сохраняется с именем:"), MB_OK);

                }
                else
                {
                    MessageBox(hWnd, TEXT("Отказ или ошибка"), TEXT("Ошибка"), MB_ICONWARNING);
                }
                break;
            }
            //right button menu actions
            case ID_RIGHT_CLICK_RAM:
                MessageBox(hWnd, L"Объём памяти 16GB", TEXT("Объём памяти"), MB_OK);
                break;
            case ID_RIGHT_CLICK_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case ID_RIGHT_CLICK_EXIT:
                DestroyWindow(hWnd);
                break;
                //main menu
            case ID_RAM:
                MessageBox(hWnd, L"Объём памяти 16GB", TEXT("Объём памяти"), MB_OK);
                break;
            case ID_VIN:
                MessageBox(hWnd, L"Объём винчестера 256GB", TEXT("Объём винчестера"), MB_OK);
                break;
            case ID_OPEN_BASE:
                MessageBox(hWnd, L"Open Base", TEXT("Open Base"), MB_OK);
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
    case WM_PAINT:  // при перерисовке окна
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
