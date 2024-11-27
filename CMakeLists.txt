cmake_minimum_required(VERSION 3.31)

project(PP_CellularAutomata)

set(SOURCE_EXE Tests.cpp)
set(SOURCE_LIB PP_CellularAutomata.cpp)
add_library(PP_CellularAutomata STATIC ${SOURCE_LIB})


add_executable(Tests ${SOURCE_EXE})

target_link_libraries(Tests PP_CellularAutomata)