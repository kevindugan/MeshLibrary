
cmake_minimum_required(VERSION 3.6)
enable_testing()

project(MeshLib CXX)

#####################################
# GTest
#####################################
enable_testing()
add_subdirectory(contrib/gtest)

#####################################
# Project
#####################################
add_subdirectory(Mesh)
add_subdirectory(Examples)