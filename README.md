# 图书馆管理系统

一个使用 C++17、[FTXUI](https://github.com/ArthurSonzogni/FTXUI) 和 [SQLite](https://sqlite.org/) 编写的终端图书馆管理系统。项目提供管理员、普通用户和系统配置三套操作流程，界面支持键盘导航、表单输入、消息弹窗和表格展示，业务数据统一保存在 SQLite 数据库中。

## 功能概览

### 管理员

- 管理员账号登录与创建
- 查看全部书籍及库存
- 添加、删除和编辑书籍信息
- 单独调整书籍库存
- 按编号、书名或作者搜索书籍
- 查看库存不高于 5 本的低库存书籍
- 按价格排序书籍
- 按编号查询书籍简介
- 统计当前全部库存的总价值
- 查看全部用户的购买记录
- 修改管理员登录密码

### 普通用户

- 用户注册与登录
- 浏览当前可购买的书籍
- 按编号、书名或作者搜索书籍
- 账户余额充值（需验证密码）
- 购买书籍并自动扣减余额和库存
- 申请 VIP（需验证密码）
- VIP 购书享受九折优惠
- 查看本次登录期间的购物清单与合计金额
- 查看跨登录保留的个人购买记录
- 验证旧密码后修改登录密码

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
- 首次配置时可访问网络，CMake 会自动下载 FTXUI v6.1.9 和 SQLite 3.53.3

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

程序在**运行时的当前工作目录**创建 `library.db`，其中包含以下数据表：

| 数据表 | 保存内容 |
| --- | --- |
| `admins` | 管理员名称和密码 |
| `users` | 用户名称、密码、余额和 VIP 状态 |
| `books` | 书籍编号、名称、作者、简介、价格和库存 |
| `purchases` | 用户、书籍、成交单价、数量、折扣、合计和购买时间 |
| `app_metadata` | 数据迁移状态等应用元数据 |

数据库及其 WAL 临时文件已被 `.gitignore` 忽略。若从其他目录启动可执行文件，程序会在那个目录查找或创建数据库；希望固定数据位置时，请始终从同一个工作目录运行。

从旧版本升级时，程序会在首次启动中检查同目录的 `admin.dat`、`user.dat` 和 `book.dat`。当对应数据表为空时，旧数据会自动导入 SQLite，并通过元数据标记确保只迁移一次。原 `.dat` 文件会保留，方便人工核对或备份。

SQLite 存储不再以空格拆分新数据，因此书名、作者和简介可以正常包含空格。账号密码目前仍以明文保存在本地，此项目适合课程学习和本地演示，不应直接用于生产环境。

## 项目结构

```text
.
├── CMakeLists.txt              # 构建配置与 FTXUI 依赖
├── main.cpp                    # 程序入口
├── src/
│   ├── Application.*           # 业务流程与各界面之间的协调
│   ├── database/               # SQLite 连接、建表、事务与元数据
│   ├── models/                 # Admin、User、Book、Purchase、ShoppingCart
│   ├── managers/               # 账号、用户、书籍和购买记录管理
│   └── tui/                    # 当前使用的 FTXUI 界面
├── tests/                      # SQLite 数据层集成测试
└── docs/                       # 项目说明、重构记录和课程资料
```

## 代码分层

- `database`：封装 SQLite 连接、数据库结构、事务和错误信息。
- `models`：保存管理员、用户、书籍、购物项和购买记录等领域数据。
- `managers`：管理对象集合，提供查询、增删、排序和数据库读写能力。
- `tui`：只负责菜单、表单、表格和弹窗渲染。
- `Application`：连接界面与业务逻辑，处理登录、购买、充值、库存变更等完整流程。

以购书为例：用户提交书籍编号和数量后，`Application` 会检查书籍、库存和余额并应用 VIP 折扣。余额更新、库存扣减和购买记录写入会在同一个 SQLite 事务中提交，任何一步失败都会整体回滚。

## 开发说明

重新编译已有构建目录：

```bash
cmake --build build --config Release
```

运行数据层集成测试：

```bash
ctest --test-dir build -C Release --output-on-failure
```

如需重新生成构建配置，可删除本地 `build` 目录后再次执行“配置与编译”中的命令。更详细的历史说明可查看 [`docs/PROJECT_GUIDE.md`](docs/PROJECT_GUIDE.md) 和 [`docs/REFACTOR_SUMMARY.md`](docs/REFACTOR_SUMMARY.md)。
