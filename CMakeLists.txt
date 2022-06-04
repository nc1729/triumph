# CMakeLists.txt - run with "cmake [PROJECT_DIR]" in build directory
# then use 'make ...' to build required target

# set minimum CMake version
cmake_minimum_required(VERSION 3.13)

# set project name
project(triumph VERSION 1.0 DESCRIPTION "TRIUMPH ternary computer")

# require c++14
set(CMAKE_CXX_STANDARD 14)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# triumph - ternary computer
add_executable(triumph)
target_include_directories(triumph PUBLIC "triumph/include")
target_include_directories(triumph PUBLIC "common/include")
target_sources(triumph PUBLIC
                triumph/src/main.cpp
                triumph/src/Bank.cpp
                triumph/src/BankManager.cpp
                triumph/src/Computer.cpp
                triumph/src/Console.cpp
                triumph/src/CPU.cpp
                triumph/src/CPUInstructions.cpp
                triumph/src/CPUjump.cpp
                triumph/src/Disk.cpp
                triumph/src/Memory.cpp
                triumph/src/Screen.cpp
                common/src/Tryte.cpp
                )
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(triumph ${SDL2_LIBRARIES})

# tas - ternary assembler
add_executable(tas)
target_include_directories(tas PUBLIC "tas/include")
target_include_directories(tas PUBLIC "common/include")
target_sources(tas PUBLIC
                tas/src/Block.cpp
                tas/src/error.cpp
                tas/src/assemble.cpp
                tas/src/handle_instr.cpp
                tas/src/handle_jump.cpp
                tas/src/link.cpp
                tas/src/main.cpp
                tas/src/parse.cpp
                tas/src/Statement.cpp
                tas/src/tas.cpp
                tas/src/Token.cpp
                tas/src/tokenize.cpp
                tas/src/util.cpp
                common/src/FileIO.cpp
                common/src/Tryte.cpp
                )

# set up multithreading (for Linux, clang on MacOS doesn't seem to need this)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
target_link_libraries(triumph Threads::Threads)

# compiler flags - Visual Studio requires different flags altogether
set(MSVC_DEBUG_OPTIONS "/WX")
set(MSVC_RELEASE_OPTIONS "/WX" "/O2")
set(DEBUG_OPTIONS "-Wall" "-Wextra" "-Werror" "-O0" "-g")
set(RELEASE_OPTIONS "-Wall" "-Wextra" "-Werror" "-O2")
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
