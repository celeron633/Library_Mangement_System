cmake_minimum_required(VERSION 3.14)

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

# Fetch FTXUI
include(FetchContent)
FetchContent_Declare(ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
    GIT_TAG        v6.1.9
    GIT_SHALLOW    TRUE
)
set(FTXUI_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(FTXUI_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(FTXUI_BUILD_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(ftxui)

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
    src/tui/common_ui.cpp
    src/tui/login_ui.cpp
    src/tui/admin_ui.cpp
    src/tui/user_ui.cpp
    src/tui/system_ui.cpp
)

add_executable(library_system ${SOURCES})

target_include_directories(library_system PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(library_system
    PRIVATE
    ftxui::component
    ftxui::dom
    ftxui::screen
)
