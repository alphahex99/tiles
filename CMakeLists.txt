cmake_minimum_required(VERSION 3.23)
project(bestagon)

add_library(raylib STATIC IMPORTED)
set_target_properties(raylib PROPERTIES
    IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/raylib-6.0_win64_mingw-w64/lib/libraylib.a
    LINKER_LANGUAGE C
)

add_executable(bestagon)
target_compile_options(bestagon PRIVATE
    -g -Og
)
target_include_directories(bestagon PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/raylib-6.0_win64_mingw-w64/include
    ${CMAKE_CURRENT_SOURCE_DIR}/
)
target_sources(bestagon PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_link_libraries(bestagon PRIVATE
    raylib
    winmm
)