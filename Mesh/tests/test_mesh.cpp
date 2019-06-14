#include "Mesh.h"
#include "gmock/gmock.h"
#include <fstream>

TEST(MeshLib_Mesh, constructor){
    Mesh cart({1.0, 2.0}, {3, 3});
    
    std::ofstream output2d("output-2d.vtu", std::ofstream::out);
    cart.print(output2d);
    output2d.close();

    Mesh cart2({1.0, 2.0, 3.0}, {4, 3, 2});
}