cmake_minimum_required(VERSION 3.10)

project(Library_Mangement_System)

set(OUTPUT_BIN_NAME "Library_Mangement_System")
if(LINUX)
    message("-- building for Linux")
    set(CMAKE_C_FLAGS "-O0 -g -Wall -std=c99")
elseif(WIN32)
    message("-- building for Windows")
    set(CMAKE_CXX_FLAGS_RELEASE "/Od /Zi")
endif()

add_executable(${OUTPUT_BIN_NAME} 
    main_new.c 
    utils.c 
    user_management.c
    book_management.c
    admin_management.c
    system_ui.c
    data_storage.c
    common.h 
    utils.h
    user_management.h
    book_management.h
    admin_management.h
    system_ui.h
    data_storage.h)