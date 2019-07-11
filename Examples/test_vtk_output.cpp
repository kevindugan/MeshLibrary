#include "Mesh.h"
#include <fstream>


int main (){

    Mesh testMesh({0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, {4, 5, 6});
    testMesh.partitionMesh(3);

    std::ofstream output_vtk("output_vtk_ascii.vtu", std::ofstream::out);
    testMesh.print(output_vtk);
    output_vtk.close();

    return 0;
}