cmake_minimum_required(VERSION 3.10)

project(Library_Mangement_System)

set(OUTPUT_BIN_NAME "Library_Mangement_System")

if(LINUX)
    message("-- building under Linux env")
set(CMAKE_C_FLAGS "-O0 -g -Wall -std=c99")
elseif(WIN32)
    message("-- building under Windows env")
    SET(CMAKE_CXX_FLAGS_RELEASE "/Od /Zi")
endif()

add_executable(${OUTPUT_BIN_NAME} 
    Library_Mangement_System.c)