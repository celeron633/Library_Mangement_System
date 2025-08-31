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
    main.c utils.c common.h utils.h)