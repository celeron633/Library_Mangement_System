# 图书馆管理系统

一个使用 C++17 和 [FTXUI](https://github.com/ArthurSonzogni/FTXUI) 编写的终端图书馆管理系统。项目提供管理员、普通用户和系统配置三套操作流程，界面支持键盘导航、表单输入、消息弹窗和表格展示，业务数据会自动保存到本地文件。

## 功能概览

### 管理员

- 管理员账号登录与创建
- 查看全部书籍及库存
- 添加、删除和编辑书籍信息
- 单独调整书籍库存
- 按价格排序书籍
- 按编号查询书籍简介
- 统计当前全部库存的总价值

### 普通用户

- 用户注册与登录
- 浏览当前可购买的书籍
- 账户余额充值（需验证密码）
- 购买书籍并自动扣减余额和库存
- 申请 VIP（需验证密码）
- VIP 购书享受九折优惠
- 查看本次登录期间的购物清单与合计金额

### 系统配置

- 管理员身份验证
- 查看管理员列表
- 创建新管理员
- 查看和删除用户
- 清空书籍数据或用户数据

## 界面与交互

系统使用 FTXUI 构建全屏终端界面：

- 使用方向键或 `j` / `k` 切换菜单项
- 按 `Enter` 确认或提交
- 使用 `Tab` / `Shift+Tab` 在表单控件之间切换
- 密码输入框会隐藏输入内容
- 关键操作通过弹窗反馈成功、错误或确认信息

建议在支持 UTF-8 和 ANSI 控制序列的现代终端中运行，例如 Windows Terminal、PowerShell 7、常见 Linux 终端或 macOS Terminal。

## 快速开始

### 环境要求

- CMake 3.14 或更高版本
- 支持 C++17 的编译器
  - Windows：Visual Studio 2019/2022
  - Linux：GCC 7+ 或 Clang 5+
  - macOS：Apple Clang
- 首次配置时可访问网络，CMake 会自动下载 FTXUI v6.1.9

### 配置与编译

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

### 运行

Windows（Visual Studio 等多配置生成器）：

```powershell
.\build\Release\library_system.exe
```

Windows（Ninja 等单配置生成器）：

```powershell
.\build\library_system.exe
```

Linux / macOS：

```bash
./build/library_system
```

首次运行时还没有管理员数据，系统会引导创建第一个管理员账号。

## 典型使用流程

1. 首次启动，创建管理员账号。
2. 管理员登录并录入书籍、价格和库存。
3. 在主菜单注册普通用户。
4. 用户登录后充值余额，可选择申请 VIP。
5. 用户浏览并购买书籍，系统同步更新余额和库存。

## 数据持久化

程序在**运行时的当前工作目录**读写以下文件：

| 文件 | 保存内容 |
| --- | --- |
| `admin.dat` | 管理员名称和密码 |
| `user.dat` | 用户名称、密码、余额和 VIP 状态 |
| `book.dat` | 书籍编号、名称、作者、简介、价格和库存 |

这些文件已被 `.gitignore` 忽略，不会提交到仓库。若从其他目录启动可执行文件，程序会在那个目录查找或创建数据文件；希望固定数据位置时，请始终从同一个工作目录运行。

当前数据格式以空白字符分隔字段，因此账号、书名、作者和简介等输入不应包含空格。账号密码以明文保存在本地，此项目适合课程学习和本地演示，不应直接用于生产环境。

## 项目结构

```text
.
├── CMakeLists.txt              # 构建配置与 FTXUI 依赖
├── main.cpp                    # 程序入口
├── src/
│   ├── Application.*           # 业务流程与各界面之间的协调
│   ├── models/                 # Admin、User、Book、ShoppingCart
│   ├── managers/               # 账号、用户和书籍的数据管理与持久化
│   └── tui/                    # 当前使用的 FTXUI 界面
└── docs/                       # 项目说明、重构记录和课程资料
```

`main_old.cpp`、`CMakeLists_old.txt`、`src/ui/`、`src/utils/` 以及根目录的 `utils.*` 是重构前或过渡期代码，不在当前 CMake 目标中编译。当前界面实现以 `src/tui/` 为准。

## 代码分层

- `models`：保存领域数据，并负责对象的序列化与反序列化。
- `managers`：管理对象集合，提供查询、增删、排序和文件读写能力。
- `tui`：只负责菜单、表单、表格和弹窗渲染。
- `Application`：连接界面与业务逻辑，处理登录、购买、充值、库存变更等完整流程。

以购书为例：用户提交书籍编号和数量后，`Application` 会检查书籍、库存和余额，应用 VIP 折扣，随后扣减库存与余额、记录购物项，并将书籍和用户数据写回磁盘。

## 开发说明

重新编译已有构建目录：

```bash
cmake --build build --config Release
```

如需重新生成构建配置，可删除本地 `build` 目录后再次执行“配置与编译”中的命令。更详细的历史说明可查看 [`docs/PROJECT_GUIDE.md`](docs/PROJECT_GUIDE.md) 和 [`docs/REFACTOR_SUMMARY.md`](docs/REFACTOR_SUMMARY.md)。
