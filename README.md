# Computer-Graphics

计算机图形学实验项目。原项目基于 Visual Studio 2013 + EasyX，现在已迁移为 Windows + CMake + 原生 Win32/GDI 兼容层。

项目只保留 CMake 作为构建入口，不再保留旧 Visual Studio 2013 工程文件。

## 环境要求

| 项 | 要求 |
|---|---|
| 系统 | Windows x64 |
| 编译器 | Visual Studio 2026 Build Tools / MSVC 14.50+ |
| CMake | 已安装 CMake，或随 VS 组件安装 |
| Windows SDK | 必须包含 `rc.exe` 和 `mt.exe` |

## 目录说明

| 路径 | 内容 |
|---|---|
| `CMakeLists.txt` | 现代构建入口，生成 9 个实验 exe |
| `include/graphics.h` | EasyX 风格兼容层，内部使用 Win32/GDI |
| `include/conio.h` | `_getch()` 兼容头 |
| `scripts/check-toolchain.ps1` | 检查 VS2026、MSVC、CMake、Windows SDK |
| `scripts/build-vs2026.ps1` | 使用 VS2026 x64 环境配置并构建 |
| `scripts/run.ps1` | 启动指定实验 |
| `E1_1` 到 `E7` | 各实验源码 |
| `build-vs2026/` | VS2026 构建产物，不提交 |

## 实验内容

| 目标 | 源文件 | 内容 |
|---|---|---|
| `E1_1` | `E1_1/E1_1.cpp` | 一笔画 |
| `E1_2` | `E1_2/E1_2.cpp` | 奇数 n 阶完全图一笔画 |
| `E1_3` | `E1_3/E1_3.cpp` | 绘制三角形并填充 |
| `E2_1` | `E2_1/E2_1.cpp` | 鼠标点击画直线 |
| `E3_1` | `E3_1/E3_1.cpp` | 线段裁剪 |
| `E4` | `E4/E4.cpp` | 二维图形变换 |
| `E5` | `E5/E5.cpp` | 正轴测投影、三视图、正轴测投影旋转 |
| `E6` | `E6/E6.cpp` | 立方体的正轴测消隐 |
| `E7` | `E7/E7.cpp` | B 样条曲线 |

## 构建

先检查工具链：

```powershell
powershell -ExecutionPolicy Bypass -File scripts\check-toolchain.ps1
```

干净构建全部实验：

```powershell
powershell -ExecutionPolicy Bypass -File scripts\build-vs2026.ps1 -Clean
```

正常增量构建：

```powershell
powershell -ExecutionPolicy Bypass -File scripts\build-vs2026.ps1
```

构建成功后，`build-vs2026/` 下会生成：

```text
E1_1.exe
E1_2.exe
E1_3.exe
E2_1.exe
E3_1.exe
E4.exe
E5.exe
E6.exe
E7.exe
```

## 运行

使用脚本启动：

```powershell
powershell -ExecutionPolicy Bypass -File scripts\run.ps1 -Target E1_1
powershell -ExecutionPolicy Bypass -File scripts\run.ps1 -Target E2_1
powershell -ExecutionPolicy Bypass -File scripts\run.ps1 -Target E7
```

允许的目标：

```text
E1_1 E1_2 E1_3 E2_1 E3_1 E4 E5 E6 E7
```

也可以直接运行 `build-vs2026/` 下的 exe。

## 交互说明

| 目标 | 交互方式 |
|---|---|
| `E1_1` | 启动后自动绘制一笔画图形，按任意键退出 |
| `E1_2` | 控制台输入奇数 `n`，窗口绘制 n 阶完全图一笔画，按任意键退出 |
| `E1_3` | 启动后自动绘制并填充三角形图案，按任意键退出 |
| `E2_1` | 鼠标左键点击两点，绘制两点之间的直线；可重复点击继续画线 |
| `E3_1` | 程序先绘制蓝色 20 边图形；鼠标左键点击两点确定裁剪矩形，矩形内线段平移后显示为绿色 |
| `E4` | 启动后自动演示北极星图案的二维变换：放大、缩小、旋转、平移、对称、错切 |
| `E5` | 启动后显示三视图和正轴测投影；按任意键后清屏，正轴测投影开始旋转 |
| `E6` | 启动后显示立方体正轴测消隐图；按任意键后开始旋转 |
| `E7` | 鼠标左键添加控制点，右键绘制 B 样条曲线，左键双击清空 |

关闭窗口后进程应退出。

## 兼容说明

- 项目不再依赖 EasyX。
- `E1_2/E1_2.cpp` 使用 ASCII 文件名，避免新 MSVC/CMake 在中文源文件路径上出问题。
- CMake 会拒绝低于 MSVC 14.50 的编译器，避免误用旧 VS2019/VS2022 环境。
- 源码文件已统一为 UTF-8 编码。
