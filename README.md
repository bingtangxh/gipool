# gipool
Query Genshin Impact wish pool info

## 生成的可执行文件
如你所见，这个仓库里有两个 workflow，一个负责编译 Windows 的可执行文件，一个负责 Linux 和 MacOS 的。

由于没有测试环境，所以 Linux 和 MacOS 的未经测试。同时这家伙也不确定 Linux 和 MacOS 上那种没有扩展名的单文件 CLI 可执行文件分不分处理器和操作系统架构，应该分吧。

Windows 平台提供 x86 x64 ARM ARM64 架构的可执行文件。后续随着 GitHub Actions Runner 更新，ARM32 可能将只能作者手动编译上传，详细原因参见`/notes/build_arm32.md`。

## 关于 .vscode 目录
你会看到项目仓库里有一个叫 .vscode 的目录。那个目录里有一些给 Visual Studio Code 的 C/C++ 插件使用的 json 配置文件，你可以直接用我的这个，但是用这个就得保证你的开发环境和我的几乎一样，所以你也可以自行修改，或者最好不要用我这个只有我自己用的最顺的。前置条件详见`/notes/about_vscode_dir.md`。
