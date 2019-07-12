enable_testing()

add_library(Mesh Node.cpp
                 Element.cpp
                 Mesh.cpp
                 MeshWriter.cpp)

target_link_libraries(Mesh metis gmock_main)
target_include_directories(Mesh INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(tests)