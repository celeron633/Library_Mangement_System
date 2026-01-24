# 快速开始指南

## 📦 重构内容总结

这个项目已从纯C的旧版本完全重构为现代C++17应用。

### 重构前后对比

| 方面 | 旧版本 | 新版本 |
|------|--------|--------|
| C++标准 | C with Class | C++17 |
| 内存管理 | 手动指针 | `std::unique_ptr` 智能指针 |
| 代码组织 | 单一文件 | 分层模块化设计 |
| UI输出 | printf/scanf混用 | 统一使用cin/cout |
| 数据容器 | 原始数组 | `std::vector`容器 |
| 算法 | 手写循环 | STL算法（find_if, sort等） |
| 界面 | 简单文本 | 美化的表格和卡片 |
| 代码行数 | ~1600行 | ~2500行（模块化后） |

## 🚀 快速编译和运行

### 编译
```bash
cd /home/dengxh/Library_Mangement_System
mkdir -p build && cd build
cmake ..
make
```

### 运行
```bash
./library_system
```

## 📂 项目结构一览

```
src/
├── Application.h/cpp          # 应用主控制类
├── models/                    # 数据模型
│   ├── User.h/cpp             # 用户数据模型
│   ├── Admin.h/cpp            # 管理员数据模型  
│   ├── Book.h/cpp             # 书籍数据模型
│   └── ShoppingCart.h/cpp     # 购物车模型
├── managers/                  # 数据管理层
│   ├── UserManager.h/cpp      # 用户管理器
│   ├── AdminManager.h/cpp     # 管理员管理器
│   └── BookManager.h/cpp      # 书籍管理器
├── utils/                     # 工具类
│   └── UIManager.h/cpp        # UI管理器
└── ui/                        # UI组件
    └── MenuUI.h/cpp           # 菜单格式化
```

## 🎯 核心类概览

### Models（数据模型）
- **User** - 用户信息（名称、密码、余额、VIP）
- **Admin** - 管理员信息（名称、密码）
- **Book** - 书籍信息（ID、书名、作者、简介、价格、库存）
- **ShoppingCart** - 购物车（管理购物项、计算总价）

### Managers（数据管理）
所有Manager提供统一接口：
- `loadFromFile()` - 从文件加载数据
- `saveToFile()` - 保存数据到文件
- `add(obj)` - 添加对象
- `delete(id)` - 删除对象
- `find(id)` - 查找对象
- `getAll()` - 获取所有对象

### UIManager（UI管理）
- `printTitle(text)` - 打印标题框
- `printMenuItem(index, text)` - 打印菜单项
- `printSuccess/Error/Warning(msg)` - 打印提示信息
- `pause()` - 暂停
- `clearScreen()` - 清屏

### MenuUI（菜单格式化）
- `printBookTableHeader()` - 书籍表头
- `printUserTableHeader()` - 用户表头
- `printCartTableHeader()` - 购物车表头
- `printXxxRow()` - 打印行数据

### Application（应用主类）
主要方法：
- `run()` - 启动应用
- `showLoginMenu()` - 显示登录菜单
- `adminLogin/Menu()` - 管理员流程
- `userLogin/Register/Menu()` - 用户流程
- `systemConfig()` - 系统配置

## ✨ 现代C++特性使用

### 1. 智能指针
```cpp
std::unique_ptr<UserManager> m_userManager;
// 自动内存管理，避免内存泄漏
```

### 2. Lambda表达式
```cpp
auto it = std::find_if(m_users.begin(), m_users.end(),
    [&name](const User& u) { return u.getName() == name; });
```

### 3. STL容器和算法
```cpp
std::vector<User> m_users;
std::sort(m_books.begin(), m_books.end(),
    [](const Book& a, const Book& b) { return a.getPrice() < b.getPrice(); });
```

### 4. const正确性
```cpp
const std::string& User::getName() const { return m_name; }
const User* findUserByNameConst(const std::string& name) const;
```

### 5. 字符串处理
```cpp
// 使用std::string和std::stringstream，避免C风格字符串
std::string User::serialize() const {
    std::ostringstream oss;
    oss << m_name << " " << m_password << " " << m_balance;
    return oss.str();
}
```

## 📊 数据流示例

### 用户购买书籍流程
```
用户输入 
    ↓
Application::userBuyBook()
    ↓
BookManager::findBookById() 获取书籍
    ↓
验证库存和余额
    ↓
Book::deductStock() - 减少库存
User::deductBalance() - 扣除余额
ShoppingCart::addItem() - 加入购物车
    ↓
BookManager::saveToFile() - 保存数据
UserManager::saveToFile() - 保存数据
    ↓
MenuUI::printSuccess() - 显示成功信息
```

## 🎨 界面美化示例

### 菜单标题
```
============================================================
||                      图书馆管理系统 - 登录              ||
============================================================
```

### 表格显示
```
编号        书名           作者           简介           价格         库存
-------------------------------------------------------------------
B001        C++程序设计    Bjarne Stroustrup  现代C++     89.99        5
B002        设计模式       Gang of Four    经典著作        59.99        3
```

### 用户卡片
```
==================================================
  用户: john_doe
  余额: ¥1000.50
  等级: VIP会员
==================================================
```

## 🔄 编译和链接

CMakeLists.txt自动处理：
- 包含路径设置
- 源文件编译
- 链接优化（Linux: -O2，Windows: /O2）
- 警告标志启用（-Wall, /W4）

## 💾 数据文件格式

### user.dat
```
username password balance vip
john_doe secret123 1000.50 1
```

### admin.dat
```
admin admin123
```

### book.dat
```
id name author intro price stock
B001 C++17参考 Bjarne C++标准库 89.99 5
```

## 🔧 常见操作

### 查看所有源文件
```bash
find src -name "*.h" -o -name "*.cpp" | sort
```

### 清理构建
```bash
rm -rf build
```

### 重新编译
```bash
mkdir build && cd build && cmake .. && make
```

### 查看编译结果
```bash
file build/library_system
ls -lh build/library_system
```

## 📖 进阶阅读

- 查看 [README.md](README.md) 了解项目整体情况
- 查看 [PROJECT_GUIDE.md](PROJECT_GUIDE.md) 了解详细的技术文档
- 查看源代码注释了解具体实现

## ⚙️ 系统要求

| 项目 | 要求 |
|------|------|
| 操作系统 | Linux / Windows / macOS |
| C++标准 | C++17 |
| CMake | 版本 3.10+ |
| 编译器 | GCC 7+, Clang 5+, MSVC 2015+ |
| 磁盘空间 | ~5MB（包括编译输出） |

## 🐛 调试技巧

### 启用详细编译输出
```bash
make VERBOSE=1
```

### 编译单个源文件
```bash
cmake --build . --target CMakeFiles/library_system.dir/src/Application.cpp.o
```

### 查看CMake变量
```bash
cmake -L ..
```

---

**提示**: 如有问题，请查阅项目文档或检查源代码注释！

