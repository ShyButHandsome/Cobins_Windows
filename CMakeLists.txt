cmake_minimum_required(VERSION 2.8)

project(cobins_project)

add_compile_options(-std=c++11)

add_subdirectory(${PROJECT_SOURCE_DIR}/external/llfsm ${PROJECT_SOURCE_DIR}/external/llfsm/build)
add_subdirectory(${PROJECT_SOURCE_DIR}/cobins ${PROJECT_SOURCE_DIR}/cobins/build)
add_subdirectory(${PROJECT_SOURCE_DIR}/probes ${PROJECT_SOURCE_DIR}/probes/build)
add_subdirectory(${PROJECT_SOURCE_DIR}/robots ${PROJECT_SOURCE_DIR}/robots/build)
add_subdirectory(${PROJECT_SOURCE_DIR}/spy_dll ${PROJECT_SOURCE_DIR}/spy_dll/build)

