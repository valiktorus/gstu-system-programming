// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

struct Calculator {
    int array[50];
    int A;
    int B;
    int mult;
    int negativeNumberCount;
    int arraySize;
};
BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

VOID WINAPI Calculate(Calculator& calculator) {
    int mult = 1;
    int negativeNumberCount = 0;
    int multCount = 0;

    for (int i = 0; i < calculator.arraySize; i++)
    {
        if (calculator.array[i] > calculator.A && calculator.array[i] <= calculator.B)
        {
            mult *= calculator.array[i];
            multCount++;
        }
    }

    for (int i = 2; i < calculator.arraySize; i +=3)
    {
        if (calculator.array[i] < 0) {
            negativeNumberCount++;
        }
    }
    mult = multCount != 0 ? mult : 0;
    calculator.mult = mult;
    calculator.negativeNumberCount = negativeNumberCount;
}