cmake_minimum_required(VERSION 3.13)
project(WinHook)
list(APPEND CMAKE_MODULE_PATH "$ENV{WORK}/cmake")
include(CsoiWin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-s -Wl,--gc-sections")

include_directories($ENV{WORK}/include)
add_definitions(-D_UNICODE -DUNICODE)

add_library(winhook SHARED winhook.c autoscroll.c arrows.c)
add_executable(winhook-exe WIN32 main.cpp WinHook.rc)
target_compile_definitions(winhook PRIVATE -DBUILD_DLL)
target_link_options(winhook PRIVATE -nostdlib)
target_link_libraries(winhook shell32)
target_link_libraries(winhook-exe winhook)
set_target_properties(winhook-exe PROPERTIES OUTPUT_NAME WinHook)