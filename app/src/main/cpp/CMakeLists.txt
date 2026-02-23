cmake_minimum_required(VERSION 3.22.1)

project("UnityResolve")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

macro(SET_OPTION option value)
    set(${option} ${value} CACHE INTERNAL "" FORCE)
endmacro()
SET_OPTION(DOBBY_DEBUG OFF)
SET_OPTION(DOBBY_GENERATE_SHARED OFF)
add_subdirectory(extern/Dobby dobby)

add_compile_definitions(kNO_KEYSTONE)
file(GLOB_RECURSE KITTY_MEMORY_SOURCES extern/KittyMemory/KittyMemory/*.cpp)

add_library(${CMAKE_PROJECT_NAME} SHARED
        native-lib.cpp
        ${KITTY_MEMORY_SOURCES}
)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
    include
    extern/KittyMemory
    extern/fmt/include
)

target_link_libraries(${CMAKE_PROJECT_NAME}
        android
        log
	dobby
        fmt::fmt
        ${KEYSTONE_LIB}
