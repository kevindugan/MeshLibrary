
add_executable(partition test_partition.cpp)
target_link_libraries(partition Mesh)

add_executable(ring_nodes test_ring_nodes.cpp)
target_link_libraries(ring_nodes Mesh)

add_executable(vtk_output test_vtk_output.cpp)
target_link_libraries(vtk_output Mesh)