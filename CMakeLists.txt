cmake_minimum_required(VERSION 3.10)

project(LibraryManagementSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(LINUX)
    message("-- Building for Linux")
    set(CMAKE_CXX_FLAGS "-O2 -Wall")
elseif(WIN32)
    message("-- Building for Windows")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /W4")
endif()

set(SOURCES
    main.cpp
    src/Application.cpp
    src/models/User.cpp
    src/models/Admin.cpp
    src/models/Book.cpp
    src/models/ShoppingCart.cpp
    src/managers/UserManager.cpp
    src/managers/AdminManager.cpp
    src/managers/BookManager.cpp
    src/utils/UIManager.cpp
    src/ui/MenuUI.cpp
)

add_executable(library_system ${SOURCES})

# 指定包含目录
target_include_directories(library_system PRIVATE ${CMAKE_SOURCE_DIR})
