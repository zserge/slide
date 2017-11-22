list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
find_package(Cairo)

add_library(slide-core INTERFACE)
target_include_directories(slide-core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} ${Cairo_INCLUDE_DIRS})
target_link_libraries(slide-core INTERFACE ${Cairo_LIBRARIES})

add_executable(slide main.cpp)
target_link_libraries(slide PRIVATE slide-core)
