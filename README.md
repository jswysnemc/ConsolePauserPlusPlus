## ConsolePauser Plus Plus版本控制台暂停程序

### 一个开源的检测程序运行算法问题的耗时和内存占用的小工具

本开源项目基于[xkk1/ConsolePauser: Dev-C++ 中 ConsolePauser.exe 中文可替代版 (github.com)](https://github.com/xkk1/ConsolePauser)该源码改进

此工具仅在windows平台上配合使用,可以配合vscode及其code-runner插件获得良好体验,欢迎提交linux版本代码

最新版本的源码在verson6文件夹(请宽恕我拼写错误)中,您也可以看看前几个版本的代码,看到更迭的步骤

代码原理: 通过CreateProcess 创建一个子进程,

通过以下代码查询进程相关信息,以获得程序运行时间及其内存占用

  // 查询进程相关信息

  QueryPerformanceFrequency(&Frequency);

  QueryPerformanceCounter(&StartingTime);

  WaitForSingleObject(ProcessInfo.hProcess, INFINITE);

  QueryPerformanceCounter(&EndingTime);



使用截图![image-20230617185246854](https://s1.vika.cn/space/2023/06/17/64caf22a5d4a4519a1faa3b902ce964a)

使用方法: 编译该源码得到Pauser.exe,添加该程序到环境变量(建议),初次运行该程序得到以下输出:

![image-20230617185519717](https://s1.vika.cn/space/2023/06/17/5a28c57be60a4687af8582926983d25c)

该程序需要三个参数,第一个参数是运行程序的指令  

如果是c++代码,则输入编译得到的程序名称即可,

如果是java代码, 则可以输入 java 类名,

如果是python代码, 则可以输入 python 源文件名,

如果是js代码, 则可以输入 node 源文件名



第二个,第三个参数分别是程序的输入文件和输出文件,被硬编码为input.txt和out.txt,可以缺少,

如果都没有,默认使用控制台输入,默认使用控制台输出,

如果只有输入文件,则在控制台输出,如果有输出文件,则在控制输出前20行,同时在文件输出.

值得注意的是,由于输入参数是严格的三个,如果有空格隔开的一个参数,需要使用引号包含使其视为一个参数.

如果需要配合code-runner插件使用,您需要在.vscode文件夹中的settings.json文件中配置如下的设置

```json
{

    "code-runner.executorMap": {
        // FileStream
        "cpp" : "if (!(Test-Path -Path \"$workspaceRoot\\bin\")){mkdir \"$workspaceRoot\\bin\" | Out-Null} && g++ -Wall -std=c++11 -fexec-charset=GBK -lshow -o $workspaceRoot\\bin\\$fileNameWithoutExt.exe $fullFileName && start Pauser '\"$workspaceRoot\\bin\\$fileNameWithoutExt.exe\" input.txt  out.txt ' ",
        "java": "chcp 65001 | Out-Null && javac  -d $workspaceRoot\\bin -encoding UTF-8 $fullFileName && start Pauser '\"java -classpath $workspaceRoot\\bin $fileNameWithoutExt \" input.txt out.txt  '",
        "python" : "start Pauser '\"python -u $fullFileName\" input.txt out.txt'",
        "javascript":"start Pauser '\"node $fullFileName\" input.txt out.txt' "
    },

    "code-runner.runInTerminal": true

}
```

在根文件夹中已经编译好了一个gbk编码版本的程序,您可以直接使用

注意,因为是基于进程信息的检测内存耗时的,所以这个程序仅在c++程序使用时可以比较准确的检测的内存占用,其他语言似乎无法准确检测其内存占用
