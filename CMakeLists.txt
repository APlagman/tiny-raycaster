cmake_minimum_required (VERSION 2.8)
project (raytrace)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

include(CheckCXXCompilerFlag)
function(enable_cxx_compiler_flag_if_supported flag)
    string(FIND "${CMAKE_CXX_FLAGS}" "${flag}" flag_already_set)
    if(flag_already_set EQUAL -1)
        check_cxx_compiler_flag("${flag}" flag_supported)
        if(flag_supported)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}" PARENT_SCOPE)
        endif()
        unset(flag_supported CACHE)
    endif()
endfunction()

enable_cxx_compiler_flag_if_supported("-pedantic")
enable_cxx_compiler_flag_if_supported("-std=c++17")
enable_cxx_compiler_flag_if_supported("-O3")
enable_cxx_compiler_flag_if_supported("-Wall")
enable_cxx_compiler_flag_if_supported("-Wextra")
enable_cxx_compiler_flag_if_supported("-Wfloat-equal")
enable_cxx_compiler_flag_if_supported("-Wshadow")
enable_cxx_compiler_flag_if_supported("-Wcast-qual")
enable_cxx_compiler_flag_if_supported("-Wcast-align")
enable_cxx_compiler_flag_if_supported("-Wlogical-op")
enable_cxx_compiler_flag_if_supported("-Wmissing-noreturn")
enable_cxx_compiler_flag_if_supported("-Wredundant-decls")
enable_cxx_compiler_flag_if_supported("-Winline")
enable_cxx_compiler_flag_if_supported("-Winvalid-pch")
enable_cxx_compiler_flag_if_supported("-Wmissing-include-dirs")
enable_cxx_compiler_flag_if_supported("-Winit-self")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories("third_party")

file(GLOB SOURCES
    "${SRC_DIR}/*.h"
    "${SRC_DIR}/*.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE "${SRC_DIR}")
