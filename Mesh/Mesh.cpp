#include "Mesh.h"

Mesh::Mesh(const std::vector<float> &maxVerts,
           const std::vector<unsigned int> &nCells){
    
    assert (maxVerts.size() == nCells.size());
    std::vector<float> delta;
    for (unsigned int i = 0; i < nCells.size(); i++)
        delta.push_back( maxVerts[i] / float(nCells[i]) );

    // Setup Cartesian vertices
    unsigned int nVertices = 1;
    for (const auto d : nCells)
        nVertices *= (d+1);
    std::vector<std::vector<float>> vertices(nVertices, std::vector<float>(nCells.size(), 0.0));
    for (unsigned int dim = 0; dim < nCells.size(); dim++){
        unsigned int repeat = 1;
        for (unsigned int d = 0; d < dim; d++)
            repeat *= (nCells[d] + 1);

        unsigned int index = 0;
        while (index < vertices.size()){
            for (unsigned int point = 0; point <= nCells[dim]; point++){
                float value = point * delta[dim];
                for (unsigned int r = 0; r < repeat; r++){
                    vertices[index][dim] = value;
                    index++;
                }
            }
        }
    }

    // Create Node List
    this->vertices.reserve(vertices.size());
    for (const auto v : vertices)
        this->vertices.push_back( std::make_shared<Node>(Node(nCells.size(), v)) );

    // for (const auto &v : vertices)
    //     if (v.size() == 2)
    //         printf("(%4.1f, %4.1f)\n", v[0], v[1]);
    //     else if (v.size() == 3)
    //         printf("(%4.1f, %4.1f, %4.1f)\n", v[0], v[1], v[2]);

}