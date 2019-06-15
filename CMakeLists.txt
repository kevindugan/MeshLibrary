
cmake_minimum_required(VERSION 3.6)
enable_testing()

project(MeshLib CXX)

#####################################
# GTest
#####################################
if(NOT DEFINED CMAKE_SUPPRESS_DEVELOPER_WARNINGS)
    set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE INTERNAL "No dev warnings")
endif()
message(STATUS "Configuring GTest")
enable_testing()
add_subdirectory(contrib/gtest)

#####################################
# METIS
#####################################
message(STATUS "Configuring METIS")
set(GKLIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/contrib/metis-5.1.0/GKlib" CACHE PATH "path to GKlib")
add_subdirectory(contrib/metis-5.1.0)
include_directories(contrib/metis-5.1.0/include)

#####################################
# Project
#####################################
add_subdirectory(Mesh)
add_subdirectory(Examples)