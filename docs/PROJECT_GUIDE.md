# 现代C++图书馆管理系统

一个完整的面向对象设计、采用现代C++17标准实现的图书馆管理系统。

## 项目架构

### 模型层 (Models)
- **User.h/User.cpp** - 用户信息类
  - 属性: 用户名、密码、余额、VIP状态
  - 方法: 余额充值、扣款、VIP申请等

- **Admin.h/Admin.cpp** - 管理员信息类
  - 属性: 管理员名、密码
  - 方法: 密码修改等

- **Book.h/Book.cpp** - 书籍信息类
  - 属性: 书籍ID、书名、作者、简介、价格、库存
  - 方法: 库存增减、信息更新等

- **ShoppingCart.h/ShoppingCart.cpp** - 购物车类
  - 功能: 添加/移除商品、计算总价、VIP折扣计算

### 管理层 (Managers)
- **UserManager.h/UserManager.cpp** - 用户管理器
  - 用户数据持久化（从user.dat读写）
  - 用户的增删查操作

- **AdminManager.h/AdminManager.cpp** - 管理员管理器
  - 管理员数据持久化（从admin.dat读写）
  - 管理员的增删查操作

- **BookManager.h/BookManager.cpp** - 书籍管理器
  - 书籍数据持久化（从book.dat读写）
  - 书籍的增删查改、排序等操作

### UI层 (UI)
- **UIManager.h/UIManager.cpp** - 通用UI管理器
  - 清屏、标题打印、菜单打印
  - 颜色提示、居中对齐、格式化等

- **MenuUI.h/MenuUI.cpp** - 菜单UI管理器
  - 表格格式化输出
  - 用户卡片、购物车、书籍列表等表格显示

### 应用层 (Application)
- **Application.h/Application.cpp** - 应用主类
  - 整个应用的控制流
  - 登录、菜单、业务逻辑处理

## 项目结构

```
.
├── CMakeLists.txt              # CMake构建配置
├── main.cpp                    # 程序入口
├── README.md                   # 项目说明
├── src/
│   ├── Application.h           # 应用主类
│   ├── Application.cpp
│   ├── models/                 # 数据模型
│   │   ├── User.h
│   │   ├── User.cpp
│   │   ├── Admin.h
│   │   ├── Admin.cpp
│   │   ├── Book.h
│   │   ├── Book.cpp
│   │   ├── ShoppingCart.h
│   │   └── ShoppingCart.cpp
│   ├── managers/               # 数据管理层
│   │   ├── UserManager.h
│   │   ├── UserManager.cpp
│   │   ├── AdminManager.h
│   │   ├── AdminManager.cpp
│   │   ├── BookManager.h
│   │   └── BookManager.cpp
│   ├── utils/                  # 工具类
│   │   ├── UIManager.h
│   │   └── UIManager.cpp
│   └── ui/                     # UI组件
│       ├── MenuUI.h
│       └── MenuUI.cpp
└── build/                      # 编译输出目录
```

## 编译方法

### 前置要求
- CMake >= 3.10
- C++17 编译器（GCC 7+ 或 Clang 5+）

### Linux 编译

```bash
# 进入项目目录
cd Library_Mangement_System

# 创建编译目录
mkdir -p build
cd build

# 生成构建文件
cmake ..

# 编译项目
make

# 运行程序
./library_system
```

### Windows 编译

```bash
# 创建编译目录
mkdir build
cd build

# 生成 Visual Studio 项目（以 VS2019 为例）
cmake .. -G "Visual Studio 16 2019"

# 编译
cmake --build . --config Release

# 运行程序
.\Release\library_system.exe
```

## 主要功能

### 用户功能
- ✅ 用户注册和登录
- ✅ 浏览书籍列表
- ✅ 购买书籍（支持VIP9折优惠）
- ✅ 申请VIP会员
- ✅ 账户充值
- ✅ 购物车管理

### 管理员功能
- ✅ 管理员登录和创建
- ✅ 书籍入库（添加新书）
- ✅ 书籍出库（删除书籍）
- ✅ 编辑书籍信息和库存
- ✅ 按价格排序显示书籍
- ✅ 计算书籍总价值
- ✅ 查询书籍简介

### 系统配置
- ✅ 显示管理员/用户列表
- ✅ 删除用户账户
- ✅ 清除书籍/用户数据

## 设计特点

### 现代C++特性使用
- ✨ **智能指针** - 使用 `std::unique_ptr` 管理资源，避免内存泄漏
- ✨ **Lambda表达式** - 在查询和排序中使用Lambda简化代码
- ✨ **STL容器** - 使用 `std::vector` 和 `std::find_if` 等算法
- ✨ **移动语义** - 支持高效的对象移动
- ✨ **const正确性** - 正确使用const保证代码安全性

### 代码组织
- 📦 **分层设计** - Models → Managers → UI → Application
- 📦 **单一职责** - 每个类只负责一个特定功能
- 📦 **数据持久化** - 使用文本文件存储用户、管理员和书籍数据
- 📦 **统一IO** - 全部使用 `std::cin` 和 `std::cout`，不混用printf/scanf

### UI美化
- 🎨 **表格化显示** - 使用std::setw对齐的表格显示数据
- 🎨 **标题框架** - 使用ASCII字符绘制统一的菜单框
- 🎨 **色彩提示** - 不同信息使用不同的前缀（✓成功，✗错误，⚠警告）
- 🎨 **用户卡片** - 显示当前用户信息卡片

## 数据存储

系统使用三个文本文件存储数据：

### user.dat - 用户数据
```
用户名 密码 余额 VIP状态(1/0)
```

### admin.dat - 管理员数据
```
管理员名 密码
```

### book.dat - 书籍数据
```
书ID 书名 作者 简介 价格 库存
```

## 使用示例

### 第一次运行
```
1. 系统检测到没有管理员，要求创建第一个管理员
2. 输入管理员名称和密码
3. 系统提示创建成功
```

### 管理员操作流程
```
登录菜单 → 选择"管理员登录"
       → 输入管理员名和密码
       → 进入管理员菜单
       → 选择"书籍操作" → 可以添加/删除/编辑书籍
       → 或选择"书籍查询" → 可以排序/统计/查询
```

### 用户操作流程
```
登录菜单 → 选择"用户登录"或"用户注册"
        → 输入用户名和密码
        → 进入用户菜单
        → 可以浏览书籍、购买、充值、申请VIP等
```

## 代码亮点

### 1. 序列化/反序列化
每个模型类都支持与文本的互转，便于数据持久化：
```cpp
std::string User::serialize() const;
static User User::deserialize(const std::string& line);
```

### 2. 管理器的统一接口
三个管理器都提供统一的CRUD操作接口：
```cpp
bool addXXX(const XXX& obj);
bool deleteXXX(const std::string& id);
XXX* findXXXById(const std::string& id);
```

### 3. 购物车计算
购物车支持VIP折扣计算：
```cpp
double getTotalPrice() const;
double getTotalPriceWithDiscount(double discount = 1.0) const;
```

### 4. UI一致性
UIManager提供了统一的UI输出接口，保证整个应用的UI风格一致

## 编译优化

CMakeLists.txt 根据不同平台设置优化选项：
- Linux: `-O2 -Wall` 优化等级2和警告
- Windows: `/O2 /W4` 优化等级2和完整警告

## 已知限制

1. 数据存储采用简单的文本格式，不支持复杂的查询
2. 没有实现数据加密，密码以明文存储
3. 不支持并发操作
4. 简介字段不支持包含空格

## 改进建议

如果要继续改进项目，可以考虑：
- 使用SQLite或MySQL进行数据存储
- 实现密码加密存储
- 添加交易日志记录
- 实现更复杂的查询和报表功能
- 添加多线程支持
- 实现GUI界面

## 许可证

此项目为大学课程设计作品，仅供学习参考。

---

**更新时间**: 2026年1月24日
**C++标准**: C++17
**编译器要求**: GCC 7+, Clang 5+, MSVC 2015+
