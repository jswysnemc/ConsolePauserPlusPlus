#include <Windows.h>
#include <Psapi.h>
#include <stdio.h>
#include <conio.h>
// 改变颜色的宏
#define SET_COLOR(x)            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (x)); //0~15
#define SET_COLOR_DEFAULT       SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); // 15
#define SET_COLOR_GREEN         SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
#define SET_COLOR_RED           SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
int main(int argc, char *argv[])
{
    if (argc < 2 || argc > 4) 
        return printf("使用方法:\n\n <程序名称> <输入文件> <输出文件>\n"), -1;
    
    // 定义相关的变量
    CHAR CommandLine[MAX_PATH];
    STARTUPINFO StartupInfo;
    PROCESS_INFORMATION ProcessInfo;
    PROCESS_MEMORY_COUNTERS ProcessMemoryCounters;
    LARGE_INTEGER StartingTime, EndingTime, Frequency;
    DWORD ReturnValue;

    // 根据相关参数设置不同的重定向逻辑
    if(argc == 4){
        freopen(argv[2], "r", stdin);
        freopen(argv[3], "w", stdout);
        sprintf(CommandLine, "\"%s\" < \"%s\" > \"%s\"", argv[1], argv[2], argv[3]);
    }
    if(argc == 3){
        freopen(argv[2], "r", stdin);
        sprintf(CommandLine, "\"%s\" < \"%s\"", argv[1], argv[2]);
    }
    if(argc == 2){ sprintf(CommandLine, "%s", argv[1]); }

    // 设置标题
    SetConsoleTitle(argv[1]);

    // 设置变量初值
    memset(&ProcessInfo, 0, sizeof(ProcessInfo));
    memset(&StartupInfo, 0, sizeof(StartupInfo));
    StartupInfo.cb = sizeof(StartupInfo);

    // 创建进程
    if (!CreateProcess(NULL, CommandLine, NULL, NULL, TRUE, 0, NULL, NULL, &StartupInfo, &ProcessInfo))
        return printf("\n无法创建进程: %s\n请按任意键继续 . . . ", CommandLine), getch(), 0;
    
    // 查询进程相关信息
    QueryPerformanceFrequency(&Frequency);
    QueryPerformanceCounter(&StartingTime);
    WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
    QueryPerformanceCounter(&EndingTime);
    GetProcessMemoryInfo(ProcessInfo.hProcess, &ProcessMemoryCounters, sizeof(ProcessMemoryCounters));
    GetExitCodeProcess(ProcessInfo.hProcess, &ReturnValue);
    LONGLONG ExecutionTime = (EndingTime.QuadPart - StartingTime.QuadPart) * 1000000 / Frequency.QuadPart;

    // 重定向回控制台
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    fflush(stdin); fflush(stdout);

    // 打印相关信息
    if(ReturnValue){SET_COLOR_RED}else{SET_COLOR_GREEN}
    printf("\n------------------------------------------------------------");
    if(ReturnValue)printf("\n程序可能出现错误!!!\n");
    if(argc == 3)printf("\n重定向到文件输入流: %s!",argv[2]);
    if(argc == 4)printf("\n重定向到文件输入流: %s! \n重定向到文件输出流: %s!\n",argv[2],argv[3]);
    printf("\n程序返回状态\t: %ld (0x%lX)", ReturnValue, ReturnValue);
    printf("\n程序耗时\t: %lld.%lld ms", ExecutionTime / 1000, ExecutionTime % 1000);
    // unsigned mem = ProcessMemoryCounters.PeakWorkingSetSize;
    unsigned mem = ProcessMemoryCounters. PeakWorkingSetSize;
    unsigned vmem = ProcessMemoryCounters.PeakPagefileUsage ;


    printf("\n程序使用内存\t: %u MB  / %u KB ", ((mem + vmem) >> 20), ((mem + vmem) >> 10));
    printf("\n------------------------------------------------------------");
    SET_COLOR_DEFAULT
    printf("\n按任意键继续 . . . ");
    return getch(), 0;
}