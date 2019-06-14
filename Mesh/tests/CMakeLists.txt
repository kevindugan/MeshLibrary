enable_testing()

add_executable(testNode test_node.cpp)
target_link_libraries(testNode Mesh)
add_test(TestNode testNode)

add_executable(testElement test_element.cpp)
target_link_libraries(testElement Mesh)
add_test(TestElement testElement)

add_executable(testMesh test_mesh.cpp)
target_link_libraries(testMesh Mesh)
add_test(TestMesh testMesh)