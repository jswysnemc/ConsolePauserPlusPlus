#include <Windows.h>
#include <Psapi.h>
#include <stdio.h>
int main(int argc, char *argv[])
{
    if (argc < 2) return printf("Usage:\n\n%s <filename> <parameters...>\n", argv[0]), -1;
    CHAR Command[MAX_PATH];
    STARTUPINFO StartupInfo;
    PROCESS_INFORMATION ProcessInfo;
    PROCESS_MEMORY_COUNTERS ProcessMemoryCounters;
    LARGE_INTEGER StartingTime, EndingTime, Frequency;
    DWORD ReturnValue;
    strcpy(Command, argv[1]);
    for (int i = 2; i < argc; i++)
        strcat(Command, " "), strcat(Command, argv[i]);
    SetConsoleTitle(Command);
    memset(&ProcessInfo, 0, sizeof(ProcessInfo));
    memset(&StartupInfo, 0, sizeof(StartupInfo));
    StartupInfo.cb = sizeof(StartupInfo);
    if (!CreateProcess(NULL, Command, NULL, NULL, FALSE, 0, NULL, NULL, &StartupInfo, &ProcessInfo))
        return printf("\nCouldn't create process: %s\nPress any key to continue . . . ", Command), getch(), -1;
    QueryPerformanceFrequency(&Frequency);
    QueryPerformanceCounter(&StartingTime);
    WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
    QueryPerformanceCounter(&EndingTime);
    GetProcessMemoryInfo(ProcessInfo.hProcess, &ProcessMemoryCounters, sizeof(ProcessMemoryCounters));
    GetExitCodeProcess(ProcessInfo.hProcess, &ReturnValue);
    LONGLONG ExecutionTime = (EndingTime.QuadPart - StartingTime.QuadPart) * 1000000 / Frequency.QuadPart;
    printf("\n-----------------------------------------------");
    printf("\nProcess returned: %u (0x%X)", ReturnValue, ReturnValue);
    printf("\nExecution time: %lld.%lld ms", ExecutionTime / 1000, ExecutionTime % 1000);
    printf("\nMaximum memory use: %u KB", (unsigned)(ProcessMemoryCounters.PeakWorkingSetSize >> 10));
    printf("\n-----------------------------------------------");
    printf("\nPress any key to continue . . . ");
    return getch(), 0;
}