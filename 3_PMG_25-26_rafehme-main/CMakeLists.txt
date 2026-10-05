
cmake_minimum_required(VERSION 3.20)

project(MyEngine
    VERSION 0.1.0
    LANGUAGES C CXX
)

# ============================================================
# Configuración
# ============================================================

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================================
# GLFW
# ============================================================

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(
    ${CMAKE_SOURCE_DIR}/deps/GLFW/glfw-3.5.1
)

# ============================================================
# GLAD
# ============================================================

add_library(glad STATIC
    ${CMAKE_SOURCE_DIR}/deps/glad/src/gl.c
)

target_include_directories(glad PUBLIC
    ${CMAKE_SOURCE_DIR}/deps/glad/include
)


# ============================================================
# Lua
# ============================================================

add_library(lua STATIC
    ${CMAKE_SOURCE_DIR}/deps/Lua/src/lapi.c
    ${CMAKE_SOURCE_DIR}/deps/Lua/src/lauxlib.c
    ${CMAKE_SOURCE_DIR}/deps/Lua/src/lbaselib.c
    ${CMAKE_SOURCE_DIR}/deps/Lua/src/lcode.c
    ${CMAKE_SOURCE_DIR}/deps/Lua/src/lcorolib.c
    ${CMAKE_SOURCE_DIR}/deps/Lua/src/lctype.c
    ${CMAKE_SOURCE_DIR}/deps/Lua/src/ldblib.c
    ${CMAKE_SOURCE_DIR}/deps/Lua/src/ldebug.c
    ${CMAKE_SOURCE_DIR}/deps/Lua/src/ldo.c
    ${CMAKE_SOURCE_DIR}/deps/Lua/src/ldump.c
    ${CMAKE_SOURCE_DIR}/deps/Lua/src/lfunc.c
    ${CMAKE_SOURCE_DIR}/deps/Lua/src/lgc.c
    ${CMAKE_SOURCE_DIR}/deps/Lua/src/linit.c
    ${CMAKE_SOURCE_DIR}/deps/Lua/src/liolib.c
    ${CMAKE_SOURCE_DIR}/deps/Lua/src/llex.c
    ${CMAKE_SOURCE_DIR}/deps/Lua/src/lmathlib.c
    ${CMAKE_SOURCE_DIR}/deps/Lua/src/lmem.c
    ${CMAKE_SOURCE_DIR}/deps/Lua/src/loadlib.c
    ${CMAKE_SOURCE_DIR}/deps/Lua/src/lobject.c
    ${CMAKE_SOURCE_DIR}/deps/Lua/src/lopcodes.c
    ${CMAKE_SOURCE_DIR}/deps/Lua/src/loslib.c
    ${CMAKE_SOURCE_DIR}/deps/Lua/src/lparser.c
    ${CMAKE_SOURCE_DIR}/deps/Lua/src/lstate.c
    ${CMAKE_SOURCE_DIR}/deps/Lua/src/lstring.c
    ${CMAKE_SOURCE_DIR}/deps/Lua/src/lstrlib.c
    ${CMAKE_SOURCE_DIR}/deps/Lua/src/ltable.c
    ${CMAKE_SOURCE_DIR}/deps/Lua/src/ltablib.c
    ${CMAKE_SOURCE_DIR}/deps/Lua/src/ltm.c
    ${CMAKE_SOURCE_DIR}/deps/Lua/src/lundump.c
    ${CMAKE_SOURCE_DIR}/deps/Lua/src/lutf8lib.c
    ${CMAKE_SOURCE_DIR}/deps/Lua/src/lvm.c
    ${CMAKE_SOURCE_DIR}/deps/Lua/src/lzio.c
)

target_include_directories(lua PUBLIC
    ${CMAKE_SOURCE_DIR}/deps/Lua/src
)

# ============================================================
# Imgui
# ============================================================



add_library(imgui STATIC
    ${CMAKE_SOURCE_DIR}/deps/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/deps/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/deps/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/deps/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/deps/imgui/imgui_demo.cpp

    ${CMAKE_SOURCE_DIR}/deps/imgui/backends/imgui_impl_glfw.cpp
    ${CMAKE_SOURCE_DIR}/deps/imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
    ${CMAKE_SOURCE_DIR}/deps/imgui
    ${CMAKE_SOURCE_DIR}/deps/imgui/backends
)

target_link_libraries(imgui PUBLIC
    glfw
    glad
)

# ============================================================
# Assimp
# ============================================================

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(
    ${CMAKE_SOURCE_DIR}/deps/assimp/assimp
)

# ============================================================
# Código fuente
# ============================================================

file(GLOB_RECURSE ENGINE_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

file(GLOB_RECURSE ENGINE_HEADERS
    CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/include/**
)

# ============================================================
# Ejecutable
# ============================================================

add_executable(${PROJECT_NAME}
    ${ENGINE_SOURCES}
    ${ENGINE_HEADERS}
)

# ============================================================
# Include directories
# ============================================================

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/deps/GLAD/include
    ${CMAKE_SOURCE_DIR}/deps/sol2/include
    ${CMAKE_SOURCE_DIR}/deps/json
)

# ============================================================
# Librerías
# ============================================================

target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    glad
    assimp
    lua
    imgui
)

# ============================================================
# Jerarquía de carpetas en Visual Studio
# ============================================================

source_group(
    TREE ${CMAKE_SOURCE_DIR}/src
    PREFIX "Source Files"
    FILES ${ENGINE_SOURCES}
)

source_group(
    TREE ${CMAKE_SOURCE_DIR}/include
    PREFIX "Header Files"
    FILES ${ENGINE_HEADERS}
)

# ============================================================
# Assets
# ============================================================

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/data
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/data
)

# ============================================================
# Información
# ============================================================

message(STATUS "===================================")
message(STATUS "        ${PROJECT_NAME}")
message(STATUS "===================================")
message(STATUS "C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type:   ${CMAKE_BUILD_TYPE}")
message(STATUS "===================================")



