cmake_minimum_required(VERSION 3.14)

project(LibraryManagementSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(LINUX)
    message("-- Building for Linux")
    set(CMAKE_CXX_FLAGS "-O2 -Wall")
elseif(WIN32)
    message("-- Building for Windows")
    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:/utf-8>)
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

# Build the official SQLite amalgamation so the project does not depend on a
# platform-specific SQLite development package.
set(SQLITE_FETCH_TIMESTAMP_ARGS)
if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.24")
    set(SQLITE_FETCH_TIMESTAMP_ARGS DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
endif()
FetchContent_Declare(sqlite_amalgamation
    URL      https://sqlite.org/2026/sqlite-amalgamation-3530300.zip
    URL_HASH SHA3_256=d45c688a8cb23f68611a894a756a12d7eb6ab6e9e2468ca70adbeab3808b5ab9
    ${SQLITE_FETCH_TIMESTAMP_ARGS}
)
FetchContent_MakeAvailable(sqlite_amalgamation)

add_library(sqlite3 STATIC ${sqlite_amalgamation_SOURCE_DIR}/sqlite3.c)
target_include_directories(sqlite3 PUBLIC ${sqlite_amalgamation_SOURCE_DIR})
target_compile_definitions(sqlite3 PRIVATE SQLITE_OMIT_LOAD_EXTENSION)

set(SOURCES
    main.cpp
    src/Application.cpp
    src/database/Database.cpp
    src/database/LegacyDataMigrator.cpp
    src/models/User.cpp
    src/models/Admin.cpp
    src/models/Book.cpp
    src/models/ShoppingCart.cpp
    src/managers/UserManager.cpp
    src/managers/AdminManager.cpp
    src/managers/BookManager.cpp
    src/managers/PurchaseManager.cpp
    src/tui/common_ui.cpp
    src/tui/login_ui.cpp
    src/tui/admin_ui.cpp
    src/tui/user_ui.cpp
    src/tui/system_ui.cpp
)

add_executable(library_system ${SOURCES})

if(MSVC)
    target_compile_options(library_system PRIVATE /W4)
endif()

target_include_directories(library_system PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(library_system
    PRIVATE
    ftxui::component
    ftxui::dom
    ftxui::screen
    sqlite3
)

include(CTest)
if(BUILD_TESTING)
    add_executable(database_integration_test
        tests/database_integration_test.cpp
        src/database/Database.cpp
        src/database/LegacyDataMigrator.cpp
        src/models/User.cpp
        src/models/Admin.cpp
        src/models/Book.cpp
        src/managers/UserManager.cpp
        src/managers/AdminManager.cpp
        src/managers/BookManager.cpp
        src/managers/PurchaseManager.cpp
    )
    target_include_directories(database_integration_test PRIVATE ${CMAKE_SOURCE_DIR})
    target_link_libraries(database_integration_test PRIVATE sqlite3)
    if(MSVC)
        target_compile_options(database_integration_test PRIVATE /W4)
    endif()
    add_test(NAME database_integration COMMAND database_integration_test)
endif()
