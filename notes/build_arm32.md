# 为啥 Windows ARM32 的可执行文件只能手动编译上传

Windows ARM32 的目前还可以 GitHub Actions 编译，之前因为会报错

`Compiling Desktop applications for the ARM platform is not supported.`

无法编译，现在有了仓库里的两个 `*.props` 文件，就可以编译了。

但是 Visual Studio 2026 直接不支持 ARM32 了， windows-latest 已经带的是 VS2026 了，随着时间的推移，最后一个能用 VS2022 ARM32 的镜像 windows-2022 迟早下架，那时候就编译不出 ARM32 的程序了。

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

仓库根目录下有 `build_clang_win.cmd` 和 `build_msvc.cmd` ，可以在 VSCode 中打开该仓库，并单击“终端(T)”→“运行任务...”进行调用。但是这样生成的 ARM32 程序没有签名，不能在未开启 UMCI Audit 模式的 Windows RT 或 Windows RT 8.1 系统上运行，且没有图标。