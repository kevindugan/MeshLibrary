#ifndef MESH_H_C0A0
#define MESH_H_C0A0

#include "Element.h"
#include <iomanip>
#include "metis.h"

class Mesh {

    public:
        Mesh() = delete;
        Mesh(const std::vector<float> &,
             const std::vector<float> &,
             const std::vector<unsigned int> &,
             unsigned int processor=0);

        void print(std::ostream &out=std::cout) const;
        void partitionMesh(const unsigned int nPartitions);

    private:
        unsigned int dimension, owningProcessor;
        std::vector<std::shared_ptr<Node>> vertices;
        std::vector<std::shared_ptr<Element>> elements;
        std::vector<std::vector<unsigned int>> elementConnectivity;

        void generateVertices(const std::vector<float> &minVerts,
                              const std::vector<float> &maxVerts,
                              const std::vector<unsigned int> &nCells);
        void generateElements(const std::vector<unsigned int> &nCells);
};

#endif // MESH_H_C0A0