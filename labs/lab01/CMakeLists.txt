get_filename_component(LAB_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)

set(LAB_ROOT ${CMAKE_CURRENT_LIST_DIR})
set(LAB_INCLUDE_ROOT "${LAB_ROOT}/include")
set(LAB_SOURCE_ROOT "${LAB_ROOT}/source")
set(LAB_SHADER_ROOT "${LAB_ROOT}/shaders")

add_subdirectory(${LAB_INCLUDE_ROOT})
add_subdirectory(${LAB_SOURCE_ROOT})
add_subdirectory(${LAB_SHADER_ROOT})

source_group("source" FILES ${LAB_SOURCE_LIST})
source_group("include" FILES ${LAB_INCLUDE_LIST})
source_group("shaders" FILES ${LAB_SHADER_LIST})

include_directories(${LAB_INCLUDE_ROOT})
include_directories(${LAB_SHADER_ROOT})

add_executable(${LAB_NAME} ${LAB_SOURCE_LIST} ${LAB_INCLUDE_LIST}
    ${LAB_SHADER_LIST})
target_link_libraries(${LAB_NAME} ${ATLAS_LIBRARIES})
set_target_properties(${LAB_NAME} PROPERTIES FOLDER "labs")
