# 为啥 Windows ARM32 的可执行文件只能手动编译上传

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