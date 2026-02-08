# gipool
Query Genshin Impact wish pool info

## 生成的可执行文件
如你所见，这个仓库里有两个 workflow，一个负责编译 Windows 的可执行文件，一个负责 Linux 和 MacOS 的。
由于没有测试环境，所以 Linux 和 MacOS 的未经测试。同时这家伙也不确定 Linux 和 MacOS 上那种没有扩展名的单文件 CLI 可执行文件分不分处理器和操作系统架构。

Windows 的提供 x86 x64 ARM64 架构。

没办法，Windows ARM32 的没法用 GitHub Actions 编译，具体原因就是那句
`Compiling Desktop applications for the ARM platform is not supported.`
在给人使绊子。
本地手动编译上传的话，倒是可以直接用管理员身份开一个文本编辑器，将这个文件：
`C:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\V140\Platforms\ARM\PlatformToolsets\v140\Toolset.targets`
中的
`<VCMessage Code="MSB8022" Type="Error" Condition="'$(DesignTimeBuild)' != 'true' and '$(WindowsSDKDesktopARMSupport)' != 'true' and '$(WindowsAppContainer)' != 'true'"/>`
和这两个文件
`corecrt.h`
`crtdefs.h`
中的那句
`#error Compiling Desktop applications for the ARM platform is not supported.`
都注释掉就可以编译了，但是 GitHub Actions Runner 上面的虚拟机里面微软给的 SDK 头文件之类的咱可不好改，此外还考虑到很多人的 Windows ARM32 设备开启的是“测试模式”而不是“UMCI 审核模式”，也就是桌面应用需要进行测试签名才能运行，而测试签名这一步要么我本地签好再上传，要么最终用户下下来没签名的东西，签名之后才能用，那我就一次性签好再发布呗，所以 ARM32 还是手动编译上传吧。

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