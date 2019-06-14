#ifndef MESH_H_C0A0
#define MESH_H_C0A0

#include "Element.h"

class Mesh {

    public:
        Mesh() = delete;
        Mesh(const std::vector<float> &,
             const std::vector<unsigned int> &);

    private:
        std::vector<std::shared_ptr<Node>> vertices;
        std::vector<std::shared_ptr<Element>> elements;
};

#endif // MESH_H_C0A0