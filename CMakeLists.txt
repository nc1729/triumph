# CMakeLists.txt - run with "cmake [PROJECT_DIR]" in build directory
# then use 'make ...' to build required target

# set minimum CMake version
cmake_minimum_required(VERSION 3.7)

# set project name
project(triumph VERSION 1.0 DESCRIPTION "TRIUMPH ternary computer")

# require c++17
set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# set variable TAS_SOURCES to be a list of .cpp files in tas/src
file(GLOB_RECURSE TAS_SOURCES CONFIGURE_DEPENDS "tas/src/*.cpp")

# set variable TRIUMPH_SOURCES to be a list of .cpp files in triumph/src
file(GLOB_RECURSE TRIUMPH_SOURCES CONFIGURE_DEPENDS "triumph/src/*.cpp")

add_executable(tas ${TAS_SOURCES})
target_include_directories(tas PUBLIC "tas/include")

add_executable(triumph ${TRIUMPH_SOURCES})
target_include_directories(triumph PUBLIC "triumph/include")

set(MSVC_DEBUG_OPTIONS "/WX")
set(MSVC_RELEASE_OPTIONS "/WX" "/O2")
set(DEBUG_OPTIONS "-Wall" "-Wextra" "-Werror" "-O0" "-g")
set(RELEASE_OPTIONS "-Wall" "-Wextra" "-Werror" "-O2")

# compiler options
if (MSVC)
    # Visual Studio compiler flags
    target_compile_options(tas PUBLIC "$<$<CONFIG:DEBUG>:${MSVC_DEBUG_OPTIONS}>")
    target_compile_options(tas PUBLIC "$<$<CONFIG:RELEASE>:${MSVC_RELEASE_OPTIONS}>")
    target_compile_options(triumph PUBLIC "$<$<CONFIG:DEBUG>:${MSVC_DEBUG_OPTIONS}>")
    target_compile_options(triumph PUBLIC "$<$<CONFIG:RELEASE>:${MSVC_RELEASE_OPTIONS}>")
else()
    # GCC & clang compiler flags
    target_compile_options(tas PUBLIC "$<$<CONFIG:DEBUG>:${DEBUG_OPTIONS}>")
    target_compile_options(tas PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_OPTIONS}>")
    target_compile_options(triumph PUBLIC "$<$<CONFIG:DEBUG>:${DEBUG_OPTIONS}>")
    target_compile_options(triumph PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_OPTIONS}>")
endif()
