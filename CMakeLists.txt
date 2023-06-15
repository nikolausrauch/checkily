#########################################
#                checkily               #
#---------------------------------------#
# @author Nikolaus Rauch                #
# @date 16.04.2022                      #
#---------------------------------------#
#########################################
cmake_minimum_required( VERSION 3.9 )
project( checkily VERSION 0.0.0 LANGUAGES CXX )

message("\n * checkily is a bitboard based chess move generator, chess engine, and uci compatible gui ")

message(STATUS "${PROJECT_NAME} build environment --")
message(STATUS "Build realm:     ${CMAKE_SYSTEM_NAME}-${CMAKE_HOST_SYSTEM_PROCESSOR}")


#########################################
#                Options                #
#########################################
option(BUILD_TESTS "Build Google Test Programs" OFF)


#########################################
#              CMake-Stuff              #
#########################################
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)


#########################################
#        Global Compiler Options        #
#########################################
set(GCC_COMPILE_OPTIONS "-Wall")
set(GCC_COMPILE_DEBUG_OPTIONS "${GCC_COMPILE_OPTIONS};-ggdb;-O0")
set(GCC_COMPILE_RELEASE_OPTIONS "${GCC_COMPILE_OPTIONS};-O3")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(MSVC_COMPILE_OPTIONS "/Wall")
set(MSVC_COMPILE_DEBUG_OPTIONS "${MSVC_COMPILE_OPTIONS};/Zi")
set(MSVC_COMPILE_RELEASE_OPTIONS "${MSVC_COMPILE_OPTIONS};/O2")
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_compile_options("$<$<AND:$<CXX_COMPILER_ID:GNU>,$<CONFIG:DEBUG>>:${GCC_COMPILE_DEBUG_OPTIONS}>")
add_compile_options("$<$<AND:$<CXX_COMPILER_ID:GNU>,$<CONFIG:RELEASE>>:${GCC_COMPILE_RELEASE_OPTIONS}>")

add_compile_options("$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:DEBUG>>:${MSVC_COMPILE_DEBUG_OPTIONS}>")
add_compile_options("$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:RELEASE>>:${MSVC_COMPILE_RELEASE_OPTIONS}>")

#########################################
#       Build Chess Move Generator      #
#########################################
add_subdirectory(chess)

#########################################
#           Build Google Tests          #
#########################################
if(BUILD_TESTS)
    add_subdirectory(test)
endif(BUILD_TESTS)

