# gipool
Query Genshin Impact wish pool info



## 关于 .vscode 目录
你会看到项目仓库里有一个叫 .vscode 的目录。那个目录里有一些给 Visual Studio Code 的 C/C++ 插件使用的 json 配置文件。
使用它的前置条件有：
- 你的开发机运行的是 x64 的 Windows 操作系统
- 你已安装 Visual Studio 的四个架构的生成工具 （x86 x64 ARM ARM64）
- 你已安装 Windows 11 SDK 10.0.22621.0 （若要使用其他版本的 SDK ，可手动修改 tasks.json）
- 你已将 vcvarsall.bat 所在的目录路径放入加入 Windows 的 PATH 环境变量
- 你的系统上有 MinGW，并且已将 bin 目录加入 Windows 的 PATH 环境变量，如 "C:\mingw64\bin"
- （注意 PATH 环境变量不应该有双引号，哪怕路径里有空格或特殊字符也不要加双引号）
- 你的系统上已经创建了 C:\cproglexefile 这个目录