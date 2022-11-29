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
target_include_directories(triumph PUBLIC
  "${CMAKE_SOURCE_DIR}"
  "${CMAKE_SOURCE_DIR}/triumph")
target_sources(triumph PUBLIC
                "common/FileIO.cpp"
                "common/Tryte.cpp"
                "common/util.cpp"
                "triumph/Computer/main.cpp"
                "triumph/Computer/Computer.cpp"
                "triumph/CPU/CPU.cpp"
                "triumph/CPU/CPUInstructions.cpp"
                "triumph/CPU/CPUjump.cpp"
                "triumph/CPU/CPUdebugger.cpp"
                "triumph/IO/Console.cpp"
                "triumph/IO/Disk.cpp"
                "triumph/IO/Screen.cpp"
                "triumph/Memory/Memory.cpp"
                "triumph/Memory/Bank.cpp"
                "triumph/Memory/BankManager.cpp"
                )
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(triumph ${SDL2_LIBRARIES})

# tas - ternary assembler
add_executable(tas)
target_include_directories(tas PUBLIC
  "${CMAKE_SOURCE_DIR}"
  "tas/include")
target_sources(tas PUBLIC
                "tas/src/Block.cpp"
                "tas/src/error.cpp"
                "tas/src/assemble.cpp"
                "tas/src/handle_instr.cpp"
                "tas/src/handle_jump.cpp"
                "tas/src/link.cpp"
                "tas/src/main.cpp"
                "tas/src/parse.cpp"
                "tas/src/Statement.cpp"
                "tas/src/tas.cpp"
                "tas/src/Token.cpp"
                "tas/src/tokenize.cpp"
                "common/FileIO.cpp"
                "common/Tryte.cpp"
                "common/util.cpp"
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
    target_link_options(tas PUBLIC "/SUBSYSTEM:CONSOLE")
    target_link_options(triumph PUBLIC "/SUBSYSTEM:CONSOLE")
else()
    # GCC & clang compiler flags
    target_compile_options(tas PUBLIC "$<$<CONFIG:DEBUG>:${DEBUG_OPTIONS}>")
    target_compile_options(tas PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_OPTIONS}>")
    target_compile_options(triumph PUBLIC "$<$<CONFIG:DEBUG>:${DEBUG_OPTIONS}>")
    target_compile_options(triumph PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_OPTIONS}>")
endif()
