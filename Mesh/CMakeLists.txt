enable_testing()

add_library(Mesh Node.cpp
                 Element.cpp)

add_subdirectory(tests)