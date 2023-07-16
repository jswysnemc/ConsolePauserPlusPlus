# $filePath = "D:\StudingFiles\c++方向\C++算法\.vscode\settings.json"
$searchString1 = "FileStream"
$searchString2 = "STD_Stream"


$content = Get-Content $filePath
# 通过以下代码切换文件输入和 控制台输入

# 在这一行下方添加指定内容
$conf1 = @'
        // FileStream
        "cpp" : "if (!(Test-Path -Path \"$workspaceRoot\\bin\")){mkdir \"$workspaceRoot\\bin\" | Out-Null} && g++ -Wall -std=c++11 -fexec-charset=GBK -lshow -o $workspaceRoot\\bin\\$fileNameWithoutExt.exe $fullFileName && start Pauser '\"$workspaceRoot\\bin\\$fileNameWithoutExt.exe\" input.txt  out.txt ' ",
        "java": "chcp 65001 | Out-Null && javac  -d $workspaceRoot\\bin -encoding UTF-8 $fullFileName && start Pauser '\"java -classpath $workspaceRoot\\bin $fileNameWithoutExt \" input.txt out.txt  '",
'@
$conf2 = @'
        // STD_Stream
        "cpp" : "if (!(Test-Path -Path \"$workspaceRoot\\bin\")){mkdir \"$workspaceRoot\\bin\" | Out-Null} && g++ -Wall -std=c++11 -fexec-charset=GBK -lshow -o $workspaceRoot\\bin\\$fileNameWithoutExt.exe $fullFileName && start Pauser $workspaceRoot\\bin\\$fileNameWithoutExt.exe ",
        "java": "chcp 65001 | Out-Null && javac  -d $workspaceRoot\\bin -encoding UTF-8 $fullFileName && start Pauser '\"java -classpath $workspaceRoot\\bin $fileNameWithoutExt \"'",
'@

$conf1_lines = $conf1 -split "\n\n"
$conf2_lines = $conf2 -split "\n\n"

for ($i = 0; $i -lt $content.Count; $i++) {
  if ($content[$i].Contains($searchString1) -or $content[$i].Contains($searchString2) ) {
    if($content[$i].Contains($searchString2)){
        $content[$i]   = $conf1_lines[0]
        $content[$i+1] = $conf1_lines[1]
        $content[$i+2] = $conf1_lines[2]
        echo $searchString1
        break
    }else{
        $content[$i]   = $conf2_lines[0]
        $content[$i+1] = $conf2_lines[1]
        $content[$i+2] = $conf2_lines[2]
        echo $searchString2
        break
    }
  }
}

$content | Set-Content $filePath -Encoding UTF8