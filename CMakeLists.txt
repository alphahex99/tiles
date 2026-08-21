cmake_minimum_required(VERSION 3.23)
project(tiles)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # # libraries # # #

set(LIB_COMPILE_OPTIONS
    -O3
)

add_library(flat_hash_map INTERFACE)
target_include_directories(flat_hash_map INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/flat_hash_map/
)

add_library(raylib STATIC IMPORTED)
set_target_properties(raylib PROPERTIES
    IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/raylib-6.0_win64_mingw-w64/lib/libraylib.a
    LINKER_LANGUAGE C
)
target_include_directories(raylib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/raylib-6.0_win64_mingw-w64/include
)

add_library(simdjson STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/simdjson-4.6.7/simdjson.cpp
)
target_compile_options(simdjson PRIVATE
    ${LIB_COMPILE_OPTIONS}
)
target_include_directories(simdjson INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/simdjson-4.6.7
)

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/stb/
)

# # # executable # # #

add_executable(tiles)
target_compile_options(tiles PRIVATE
    -g -Og
)
target_include_directories(tiles PRIVATE
    ${LIB_INCLUDES}
    ${CMAKE_CURRENT_SOURCE_DIR}/
)
target_sources(tiles PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_link_libraries(tiles PRIVATE
    raylib
    simdjson
    winmm
)
