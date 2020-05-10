// WindowsProject10.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "WindowsProject10.h"
#include "commdlg.h"
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <commctrl.h>
#pragma comment(lib,"ComCtl32.Lib")



#define MAX_LOADSTRING 100
#define C_COLUMNS 3 //количество столбцов
struct EXAMPLE 
{
    TCHAR name[50];
    int count;
    double price;

};

EXAMPLE arr[50], ex;

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
OPENFILENAME ofn;                               //переменная структуры, позволяющая работать с файлами

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    ProcAdd(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    ProcShow(HWND, UINT, WPARAM, LPARAM);
BOOL InitListViewColumns(HWND);
BOOL InsertListViewItems(HWND, int, EXAMPLE);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT10, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT10));

    MSG msg;

    // Цикл основного сообщения:
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
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT10));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT10);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

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
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
FILE* hFile;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static TCHAR szFile[260] = _T("");//буфер для имени файла
    switch (message)
    {
    case WM_CREATE:
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hInstance = hInst;
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = 260;
        ofn.lpstrFilter = TEXT("ALL\0*.*\0Text\0*.TXT\0");
        ofn.lpstrInitialDir = TEXT(".\\");
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case ID_SHOW:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOGSHOW), hWnd, ProcShow);
                break;
            case ID_ADD:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ADD), hWnd, ProcAdd);
                break;
            case ID_OPEN:
                if (GetOpenFileName(&ofn))
                {
                    MessageBox(hWnd, ofn.lpstrFile, TEXT("Файл открывается с именем"), MB_OK);
                    //открытие файла в режиме чтение-запись
                    if ((_wfopen_s(&hFile, ofn.lpstrFile, TEXT("r+"))) == (errno_t)INVALID_HANDLE_VALUE)
                    {
                        MessageBox(hWnd, TEXT("Отказ или ошибка открытия файла"), TEXT("Ошибка"), MB_OK);
                        return 1;
                    }
                }
                break;
            case ID_SAVE:
                if (GetSaveFileName(&ofn))
                {
                    MessageBox(hWnd, ofn.lpstrFile, TEXT("Файл сохраняется с именем"), MB_OK);
                }
                else
                {
                    MessageBox(hWnd, TEXT("Отказ или ошибка сохранения файла"), TEXT("Ошибка"), MB_OK);
                }
                break;
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
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
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

// Обработчик сообщений для окна "О программе".
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

// InitListViewColumns: Adds columns to a list-view control.
// hWndListView:        Handle to the list-view control. 
// Returns TRUE if successful, and FALSE otherwise. 
BOOL InitListViewColumns(HWND hWndListView)
{
    WCHAR szText[256];     // Temporary buffer.
    LVCOLUMN lvc;
    int iCol;

    // Initialize the LVCOLUMN structure.
    // The mask specifies that the format, width, text,
    // and subitem members of the structure are valid.
    lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

    // Add the columns.
    for (iCol = 0; iCol < C_COLUMNS; iCol++)
    {
        lvc.iSubItem = iCol;
        lvc.pszText = szText;
        lvc.cx = 100;               // Width of column in pixels.

        if (iCol < 2)
            lvc.fmt = LVCFMT_LEFT;  // Left-aligned column.
        else
            lvc.fmt = LVCFMT_RIGHT; // Right-aligned column.

        // Load the names of the column headings from the string resources.
        LoadString(hInst,
            IDS_FIRSTCOLUMN + iCol,
            szText,
            sizeof(szText) / sizeof(szText[0]));

        // Insert the columns into the list view.
        if (ListView_InsertColumn(hWndListView, iCol, &lvc) == -1)
            return FALSE;
    }

    return TRUE;
}

BOOL InsertListViewItems(HWND hWndListView, int cItems,EXAMPLE ex)
{
    LVITEM lvI;

    // Initialize LVITEM members that are common to all items.
    lvI.pszText = ex.name; // Sends an LVN_GETDISPINFO message.
    lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
    lvI.stateMask = 0;
    lvI.iSubItem = 0;
    lvI.state = 0;
    lvI.iItem = cItems;

          // Insert items into the list.
    if (ListView_InsertItem(hWndListView, &lvI) == -1)
            return FALSE;
    

    return TRUE;
}
int n = 0;//количество элементов в массиве arr
INT_PTR CALLBACK ProcShow(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hWndLV = NULL;
    int i = 0;
    wchar_t buf[50];
    BOOL noSwap;

    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        SendDlgItemMessage(hDlg, IDC_RADIO_ALL, BM_SETCHECK, WPARAM(BST_CHECKED), 0);
        hWndLV = GetDlgItem(hDlg, IDC_LISTVIEW);
        //получение дескриптора ListView
        InitListViewColumns(hWndLV);
        //определение колонок в ListView
        fseek(hFile, 0, SEEK_SET);//перемещение указателя чтения и записи на начало файла
        while (fread(&ex, sizeof(ex), 1, hFile) > 0)
        {
            InsertListViewItems(hWndLV, i, ex);//значение в 0 столбец i строки
            _itow_s(ex.count, buf, 10);//преобразование целого числа в строку
            ListView_SetItemText(hWndLV, i, 1, buf);//выводим строку в i строку 1 столбец
            swprintf(buf, 50, TEXT("%g"), ex.price);//преобразование вещественного значения в строку
            ListView_SetItemText(hWndLV, i, 2, buf);//выводим строку в i строку 2 столбец
            arr[i++] = ex;
            n = i;
        }
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDC_RADIO_ALL)
        {

        }
        else if (LOWORD(wParam) == IDC_RADIO_SORT)
        {
            ListView_DeleteAllItems(hWndLV);//очистка ListView
            //сортировка пузырьком, сортировка по полю count(количество)
            for (int i = n - 1; i >= 0; i--)
            {   
                noSwap = 1;
                for (int j = 0; j <i; j++)
                {
                    if (arr[j].count > arr[j + 1].count)
                    {
                        ex = arr[j];
                        arr[j] = arr[j + 1];
                        arr[j + 1] = ex;
                        noSwap = 0;
                    }

                }
                if (noSwap == 1)
                    break;

            }

           for(i=0;i<n;i++)
            {
                InsertListViewItems(hWndLV, i, arr[i]);//значение в 0 столбец i строки
                _itow_s(arr[i].count, buf, 10);//преобразование целого числа в строку
                ListView_SetItemText(hWndLV, i, 1, buf);//выводим строку в i строку 1 столбец
                swprintf(buf, 50, TEXT("%g"), arr[i].price);//преобразование вещественного значения в строку
                ListView_SetItemText(hWndLV, i, 2, buf);//выводим строку в i строку 2 столбец
             }
        }
        break;
    }
    return (INT_PTR)FALSE;
}

TCHAR pr[50];
INT_PTR CALLBACK ProcAdd(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_CLOSE:
        EndDialog(hDlg, LOWORD(wParam));
        return (INT_PTR)TRUE;
    case WM_INITDIALOG:
        fseek(hFile, 0, SEEK_END);//перемещение указателя чтения и записи в конец файла
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_ADD)
        {
            GetDlgItemText(hDlg, IDC_EDIT1, ex.name, sizeof(ex.name));
            //получить строки из Edit
            ex.count=GetDlgItemInt(hDlg, IDC_EDIT2, NULL, FALSE);
            //получаем переменную целого типа из Edit
            GetDlgItemText(hDlg, IDC_EDIT3, pr, sizeof(pr));
            ex.price = _wtof(pr);
            fwrite(&ex, sizeof(ex), 1, hFile);
            EndDialog(hDlg, LOWORD(wParam));
        }
        break;
    }
    return (INT_PTR)FALSE;
}
