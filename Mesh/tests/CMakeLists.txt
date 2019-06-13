enable_testing()

add_executable(testNode test_node.cpp)
target_link_libraries(testNode Mesh)
add_test(TestNode testNode)