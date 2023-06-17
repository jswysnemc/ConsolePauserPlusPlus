#include <Windows.h>
#include <Psapi.h>
#include <stdio.h>
#include <conio.h>
// 改变颜色的宏
#define SET_COLOR(x)            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (x)); //0~15
#define SET_COLOR_DEFAULT       SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); // 15
#define SET_COLOR_GREEN         SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
#define SET_COLOR_RED           SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);

void showArgs(int argc,char *argv[]){
    for(int i = 0; i < argc; i++){
        printf("argv[%d]: %s\n",i,argv[i]);
    }
}

int exec_command(int argc,char *argv[]){
    if (argc < 2 || argc > 4) 
        return printf("使用方法:\n\n Pauser <运行指令> <输入文件> <输出文件>\n获得参数如下:\n"), showArgs(argc, argv), getch(), -1;

    
    // 定义相关的变量
    CHAR CommandLine[MAX_PATH];
    STARTUPINFO StartupInfo;
    PROCESS_INFORMATION ProcessInfo;
    PROCESS_MEMORY_COUNTERS ProcessMemoryCounters;
    LARGE_INTEGER StartingTime, EndingTime, Frequency;
    DWORD ReturnValue;

    STARTUPINFO StartupInfo2;
    PROCESS_INFORMATION ProcessInfo2;
    PROCESS_MEMORY_COUNTERS ProcessMemoryCounters2;
    LARGE_INTEGER StartingTime2, EndingTime2, Frequency2;
    DWORD ReturnValue2;


    // 根据相关参数设置不同的重定向逻辑

    // 只传递一个参数
    if(argc == 2){
        sprintf(CommandLine, "%s",argv[1]);
    }

    // 传递两个参数
    if(argc == 3){
        if(strstr(argv[2],"input.txt")){
            sprintf(CommandLine, "%s   <  %s ",argv[1],argv[2]);
            freopen(argv[2], "r", stdin);
            SET_COLOR(11)
        }else if(strstr(argv[2],"out.txt")){
            sprintf(CommandLine, "%s >  %s",argv[1],argv[2]);
            freopen(argv[2], "w", stdout);
        }else{
            printf("使用方法:\n\n Pauser <运行指令> <输入文件> <输出文件>\n");
            printf("传递参数如下:\n");
            return showArgs(argc, argv) , getch(), -1;
        }
    }

    // 传递三个参数
    if(argc == 4){
        sprintf(CommandLine, "%s  <  %s  >  %s ",argv[1],argv[2],argv[3]);
        freopen(argv[argc-2], "r", stdin);
        freopen(argv[argc-1], "w", stdout);
    }

    SetConsoleTitle(CommandLine);

    // 设置变量初值
    memset(&ProcessInfo, 0, sizeof(ProcessInfo));
    memset(&StartupInfo, 0, sizeof(StartupInfo));
    StartupInfo.cb = sizeof(StartupInfo);



    memset(&ProcessInfo2, 0, sizeof(ProcessInfo2));
    memset(&StartupInfo2, 0, sizeof(StartupInfo2));
    StartupInfo2.cb = sizeof(StartupInfo2);

    // 创建进程
    if (!CreateProcess(NULL, CommandLine, NULL, 
    NULL, TRUE, 0, NULL, NULL,
     &StartupInfo, &ProcessInfo))
        return printf("\n无法创建进程: %s\n请按任意键继续 . . . ", CommandLine), getch(), 0;


    long long pid = ProcessInfo.dwProcessId;
    char cmd[330];
    sprintf(cmd, "jmap -dump:format=b,file=dump.hprof %lld",pid);
    printf("\ncmd:%s\n",cmd);
    CreateProcess(NULL,cmd,NULL,
    NULL,TRUE,0,NULL,NULL,
    &StartupInfo2,&ProcessInfo2);

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
    if(argc == 3){
        if(strstr(argv[2],"input.txt") != NULL){
            printf("\n重定向到文件输入流: %s!",argv[2]);  
        }else if(strstr(argv[2],"out.txt") != NULL){
            printf("\n重定向到文件输出流: %s!",argv[2]);  
        }
    }
    if(argc == 4)printf("\n重定向到文件输入流: %s! \n重定向到文件输出流: %s!",argv[argc-2],argv[argc-1]);
    printf("\n\n程序返回状态:  %ld (0x%lX)", ReturnValue, ReturnValue);
    printf("\n程序运行耗时:  %lld.%lld ms", ExecutionTime / 1000, ExecutionTime % 1000);
    // unsigned mem = ProcessMemoryCounters.PeakWorkingSetSize;
    unsigned mem = ProcessMemoryCounters. PeakWorkingSetSize;
    unsigned vmem = ProcessMemoryCounters.PeakPagefileUsage ;
    printf("\n程序使用内存:  %.4lf MB  / %.4lf KB ", ((mem + vmem) /( 1024.0 * 1024.0)), ((mem + vmem) /(1024.0)));
    printf("\n------------------------------------------------------------");
    SET_COLOR_DEFAULT
    if(argc== 4 || argc == 3 && strstr(argv[2],"out.txt")!=NULL){
    printf("\n以下是程序输出的前20行内容:");
    printf("\n------------------------------------------------------------\n");

    FILE *fp;
    char buf[500];
    int num_lines = 10;
    int count = 0;


    fp = fopen(argv[argc-1], "r");
    if (!fp) {
        printf("Cannot open file: %s\n", argv[argc-1]);
        return 1;
    }
    num_lines = 20;
    SET_COLOR(11)
    while (fgets(buf, 500, fp) && count < num_lines) {
        printf("%s", buf);
        count++;
    }
    SET_COLOR_DEFAULT

    fclose(fp);

    printf("\n------------------------------------------------------------");
    }
    printf("\n按任意键继续 . . . ");
    return getch(), 0;
}

// #define freopen freopen_s
int main(int argc, char *argv[]){
    return exec_command(argc, argv);
}