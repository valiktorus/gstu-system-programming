// Lab1.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Lab1.h"
#include "commdlg.h"
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <commctrl.h>
#pragma comment(lib,"ComCtl32.Lib")

#define MAX_LOADSTRING 100
#define C_COLUMNS 4
struct LAPTOP{
    TCHAR name[50];
    TCHAR ram[50];
    int memory;
    int price;
};

LAPTOP laptops[50],sortedLaptops[50], laptop;
TCHAR  selectedRam[50], empty[50];
int selectedMemory, emptyMemory;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    AddLaptop(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    ProcShow(HWND, UINT, WPARAM, LPARAM);
BOOL InitListViewColumns(HWND);
BOOL InsertListViewItems(HWND, int, LAPTOP);
INT_PTR CALLBACK SelectRAM(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK SelectMemory(HWND, UINT, WPARAM, LPARAM);

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
FILE* openFile;
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
        file.lpstrFilter = L"ALL\0*.*\0Text\0*.TXT\0";
        file.lpstrFile = name;
        file.nMaxFile = 256;
        file.lpstrInitialDir = L"C:\projects\gstu\second-sem\gstu-system-programming\files";
        file.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
        //file.lpstrDefExt = L"xtx";
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


            //Кнопка показать список компьютеров
            case ID_BUTTON_SHOW_LAPTOPS:
            {
                DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOGSHOW), hWnd, ProcShow);
                break;
            }
                //lab3
            case ID_ADD:
            {
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ADD), hWnd, AddLaptop);
                break;
            }
            //open file
            case ID_OPEN_FILE:
            {
                if (GetOpenFileName(&file))
				{
                    MessageBox(hWnd, file.lpstrFile, TEXT("Файл открывается с именем"), MB_OK);
                    //открытие файла в режиме чтение-запись
                    if ((_wfopen_s(&openFile, file.lpstrFile, TEXT("r+"))) == (errno_t)INVALID_HANDLE_VALUE)
                    {
                        MessageBox(hWnd, TEXT("Отказ или ошибка открытия файла"), TEXT("Ошибка"), MB_OK);
                        return 1;
                    }
                }
                break;


                //Логика из первой лабы
              /*  file.lpstrTitle = _T("Открыть файл для чтения");
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
                break;*/
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
                DialogBox(hInst, MAKEINTRESOURCE(IDD_SELECT_RAM), hWnd, SelectRAM);
                //MessageBox(hWnd, L"Объём памяти 16GB", TEXT("Объём памяти"), MB_OK);
                break;
            case ID_VIN:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_SELECT_MEMORY), hWnd, SelectMemory);
                //MessageBox(hWnd, L"Объём винчестера 256GB", TEXT("Объём винчестера"), MB_OK);
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

INT_PTR CALLBACK SelectRAM(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        SendDlgItemMessage(hDlg, IDC_RADIO1, BM_SETCHECK, WPARAM(BST_CHECKED), 0);
        return (INT_PTR)TRUE;
    case WM_CLOSE:
        EndDialog(hDlg, LOWORD(wParam));
        return (INT_PTR)TRUE;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_SELECT_RAM)
        {
            LRESULT result1 = SendDlgItemMessage(hDlg, IDC_RADIO1, BM_GETCHECK, 0, 0);
            LRESULT result2 = SendDlgItemMessage(hDlg, IDC_RADIO2, BM_GETCHECK, 0, 0);
            LRESULT result3 = SendDlgItemMessage(hDlg, IDC_RADIO3, BM_GETCHECK, 0, 0);
            LRESULT result4 = SendDlgItemMessage(hDlg, IDC_RADIO4, BM_GETCHECK, 0, 0);
            LRESULT result5 = SendDlgItemMessage(hDlg, IDC_RADIO5, BM_GETCHECK, 0, 0);
            LRESULT result6 = SendDlgItemMessage(hDlg, IDC_RADIO6, BM_GETCHECK, 0, 0);
            if(result1 == BST_CHECKED)
            {
                lstrcpy(selectedRam, TEXT("32Mb"));
            }
            if (result2 == BST_CHECKED)
            {
                lstrcpy(selectedRam, TEXT("64Mb"));
            }
            if (result3 == BST_CHECKED)
            {
                lstrcpy(selectedRam, TEXT("128Mb"));
            }
            if (result4 == BST_CHECKED)
            {
                lstrcpy(selectedRam, TEXT("256Mb"));
            }
            if (result5 == BST_CHECKED)
            {
                lstrcpy(selectedRam, TEXT("512Mb"));
            }
            if (result6 == BST_CHECKED)
            {
                GetDlgItemText(hDlg, IDC_EDIT1, selectedRam, sizeof(selectedRam));
            }
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
           
        }
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK SelectMemory(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        SendDlgItemMessage(hDlg, IDC_RADIO1, BM_SETCHECK, WPARAM(BST_CHECKED), 0);
        return (INT_PTR)TRUE;
    case WM_CLOSE:
        EndDialog(hDlg, LOWORD(wParam));
        return (INT_PTR)TRUE;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_SELECT_MEMORY)
        {
            LRESULT result1 = SendDlgItemMessage(hDlg, IDC_RADIO1, BM_GETCHECK, 0, 0);
            LRESULT result2 = SendDlgItemMessage(hDlg, IDC_RADIO2, BM_GETCHECK, 0, 0);
            LRESULT result3 = SendDlgItemMessage(hDlg, IDC_RADIO3, BM_GETCHECK, 0, 0);
            LRESULT result4 = SendDlgItemMessage(hDlg, IDC_RADIO4, BM_GETCHECK, 0, 0);
            LRESULT result5 = SendDlgItemMessage(hDlg, IDC_RADIO5, BM_GETCHECK, 0, 0);
            LRESULT result6 = SendDlgItemMessage(hDlg, IDC_RADIO6, BM_GETCHECK, 0, 0);
            if (result1 == BST_CHECKED)
            {
                selectedMemory = 10;
            }
            if (result2 == BST_CHECKED)
            {
                selectedMemory = 20;
            }
            if (result3 == BST_CHECKED)
            {
                selectedMemory = 30;
            }
            if (result4 == BST_CHECKED)
            {
                selectedMemory = 40;
            }
            if (result5 == BST_CHECKED)
            {
                selectedMemory = 50;
            }
            if (result6 == BST_CHECKED)
            {
                TCHAR buf[50];
                GetDlgItemText(hDlg, IDC_EDIT1, buf, sizeof(buf));
                selectedMemory = _wtoi(buf);

            }
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;

        }
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

BOOL InsertListViewItems(HWND hWndListView, int cItems, LAPTOP laptop)
{
    LVITEM lvI;

    // Initialize LVITEM members that are common to all items.
    lvI.pszText = laptop.name; // Sends an LVN_GETDISPINFO message.
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
    int i = 0;
    static HWND hWndLV = NULL;
    wchar_t buf[50];
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
       // SendDlgItemMessage(hDlg, IDC_RADIO_ALL, BM_SETCHECK, WPARAM(BST_CHECKED), 0);
        hWndLV = GetDlgItem(hDlg, IDC_LIST_LAPTOPS);
        //получение дескриптора ListView
        InitListViewColumns(hWndLV);
        //определение колонок в ListView
        fseek(openFile, 0, SEEK_SET);//перемещение указателя чтения и записи на начало файла
        while (fread(&laptop, sizeof(laptop), 1, openFile) > 0)
        {
            InsertListViewItems(hWndLV, i, laptop);//значение в 0 столбец i строки
            //swprintf(buf, 50, TEXT("%g"), laptop.name);
            //ListView_SetItemText(hWndLV, i, 1, buf)
           // swprintf(buf, 50, TEXT("%g"), laptop.ram);
            ListView_SetItemText(hWndLV, i, 1, laptop.ram)
            _itow_s(laptop.memory, buf, 10);//преобразование целого числа в строку
            ListView_SetItemText(hWndLV, i, 2, buf);//выводим строку в i строку 1 столбец
            _itow_s(laptop.price, buf, 10);//преобразование целого числа в строк
            ListView_SetItemText(hWndLV, i, 3, buf);//выводим строку в i строку 1 столбец
            sortedLaptops[i] = laptop;
            laptops[i++] = laptop;
            n = i;
        }
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        if (LOWORD(wParam) == IDC_RADIO1) {
            ListView_DeleteAllItems(hWndLV);
            for (int i = 0; i < n; i++) {
                InsertListViewItems(hWndLV, i, laptops[i]);
                ListView_SetItemText(hWndLV, i, 1, laptops[i].ram);
                _itow_s(laptops[i].memory, buf, 10);//преобразование целого числа в строку
                ListView_SetItemText(hWndLV, i, 2, buf);
                _itow_s(laptops[i].price, buf, 10);//преобразование целого числа в строк
                ListView_SetItemText(hWndLV, i, 3, buf);
            }
            selectedMemory = emptyMemory;
            //selectedRam = TEXT("");
            lstrcpy(selectedRam, TEXT(""));
        }
        else if (LOWORD(wParam) == IDC_RADIO2)
        {
            LAPTOP laps[50], lap;
            int noSwap;
            ListView_DeleteAllItems(hWndLV);
            for (int i = n - 1; i >= 0; i--)
            {
                noSwap = 1;
                for (int j = 0; j < i; j++)
                {
                    if (sortedLaptops[j].price > sortedLaptops[j + 1].price)
                    {
                        lap = sortedLaptops[j];
                        sortedLaptops[j] = sortedLaptops[j + 1];
                        sortedLaptops[j + 1] = lap;
                        noSwap = 0;
                    }

                }
                if (noSwap == 1)
                    break;

            }
            int count = 0;
            for (i = 0; i < n; i++)
            {
                if (wcscmp(selectedRam, empty) != 0 && wcscmp(selectedRam, sortedLaptops[i].ram) != 0 ||
                    selectedMemory != emptyMemory && selectedMemory != sortedLaptops[i].memory) {
                    continue;
                }

                InsertListViewItems(hWndLV, count , sortedLaptops[i]);
                ListView_SetItemText(hWndLV, count, 1, sortedLaptops[i].ram);
                _itow_s(sortedLaptops[i].memory, buf, 10);//преобразование целого числа в строку
                ListView_SetItemText(hWndLV, count, 2, buf);
                _itow_s(sortedLaptops[i].price, buf, 10);//преобразование целого числа в строк
                ListView_SetItemText(hWndLV, count, 3, buf);
                count++;
            }

        }

    }
    return (INT_PTR)FALSE;
}
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
            IDS_COLUMN1 + iCol,
            szText,
            sizeof(szText) / sizeof(szText[0]));

        // Insert the columns into the list view.
        if (ListView_InsertColumn(hWndListView, iCol, &lvc) == -1)
            return FALSE;
    }

    return TRUE;
}
//обработка добавления компа
INT_PTR CALLBACK AddLaptop(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_CLOSE:
        EndDialog(hDlg, LOWORD(wParam));
        return (INT_PTR)TRUE;
    case WM_INITDIALOG:
        fseek(openFile, 0, SEEK_END);//перемещение указателя чтения и записи в конец файла
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_ADD_BUTTON)
        {
            GetDlgItemText(hDlg, IDC_EDIT1, laptop.name, sizeof(laptop.name));
            GetDlgItemText(hDlg, IDC_EDIT2, laptop.ram, sizeof(laptop.ram));
            //получить строки из Edit
            laptop.memory = GetDlgItemInt(hDlg, IDC_EDIT3, NULL, FALSE);
            //получаем переменную целого типа из Edit
            laptop.price = GetDlgItemInt(hDlg, IDC_EDIT4, NULL, FALSE);
            fwrite(&laptop, sizeof(laptop), 1, openFile);
            EndDialog(hDlg, LOWORD(wParam));
        }
        break;
    }
    return (INT_PTR)FALSE;
}

