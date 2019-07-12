#include "Mesh.h"
#include "MeshWriter.h"
#include <fstream>


int main (){

    Mesh testMesh({0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, {4, 5, 6});
    // Mesh testMesh({0.0, 0.0}, {1.0, 1.0}, {2, 3});
    testMesh.partitionMesh(3);
    MeshWriter writer(testMesh);

    std::ofstream output_vtk_ascii("output_vtk_ascii.vtu", std::ofstream::out);
    writer.outputVTK(output_vtk_ascii, MeshWriter::ascii);
    output_vtk_ascii.close();

    std::ofstream output_vtk_bin("output_vtk_binary.vtu", std::ofstream::out | std::ofstream::binary);
    writer.outputVTK(output_vtk_bin);
    output_vtk_bin.close();

    return 0;
}