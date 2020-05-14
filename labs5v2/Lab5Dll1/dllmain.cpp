// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "resource.h"

__declspec(dllexport) HICON hIcon;
__declspec(dllexport) HCURSOR hCursor;
__declspec(dllexport) HBITMAP hBitmap;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        hIcon = LoadIcon(hModule, MAKEINTRESOURCE(IDI_ICON1));
        hCursor = LoadCursor(hModule, MAKEINTRESOURCE(IDC_CURSOR1));
        hBitmap = LoadBitmap(hModule, MAKEINTRESOURCE(IDB_BITMAP1));
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        DestroyIcon(hIcon);
        DestroyCursor(hCursor);
        break;
    }
    return TRUE;
}

