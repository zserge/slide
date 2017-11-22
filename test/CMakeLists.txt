enable_testing()

add_executable(slide_test slide_test.cpp)
target_link_libraries(slide_test PUBLIC slide-core)

add_test(NAME slide_test COMMAND slide_test catch)
