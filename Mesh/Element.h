#ifndef ELEMENT_H_3KLA
#define ELEMENT_H_3KLA

#include "Node.h"
#include <memory>
#include <algorithm>

class Element {

    public:
        Element() = delete;
        Element(const unsigned int dim,
                const std::vector<Node> &vertices);

    private:
        std::vector<Node> vertices;
};

#endif // ELEMENT_H_3KLA