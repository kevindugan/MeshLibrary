#include "Mesh.h"
#include "gmock/gmock.h"

TEST(MeshLib_Mesh, constructor){
    Mesh cart({1.0, 2.0}, {2, 2});

    Mesh cart2({1.0, 2.0, 3.0}, {2, 2, 3});
}