project(csc473)

cmake_minimum_required(VERSION 3.0)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(LABS_ROOT "${ROOT_DIR}/labs")
set(ATLAS_ROOT "${ROOT_DIR}/lib/atlas")
set(DATA_ROOT "${ROOT_DIR}/data")

include("${ATLAS_ROOT}/config/Compiler.cmake")
if(ATLAS_COMPIER_MSCV)
    add_definitions(
        -DNOMINMAX
        )
endif()

add_subdirectory(${ATLAS_ROOT})
include_directories(${ATLAS_INCLUDE_DIRS})
add_subdirectory(${LABS_ROOT})
