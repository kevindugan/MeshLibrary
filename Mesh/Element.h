#ifndef ELEMENT_H_3KLA
#define ELEMENT_H_3KLA

#include "Node.h"
#include <memory>
#include <algorithm>

class Element {

    public:
        Element() = delete;
        Element(const unsigned int dim,
                const std::vector<std::shared_ptr<Node>> &vertices);

        bool operator==(const Element &other) const;
        bool operator<(const Element &other) const;
        std::vector<std::shared_ptr<Node>> getVertices() const {return this->vertices;}

        unsigned int numBoundaryNodes() const;
        std::vector<std::shared_ptr<Node>> getBoundaryNodes() const;

        bool isNeighborElement(std::shared_ptr<Element>);
        void addNeighborElement(std::shared_ptr<Element>);
        void removeNeighborElement(std::shared_ptr<Element>);
        unsigned int numNeighbors() const;

        unsigned int numFaceVertices() const;

    private:
        std::vector<std::shared_ptr<Node>> vertices;
        std::vector<std::shared_ptr<Element>> neighbors;
        const unsigned int dimension;
        
        static bool ptr_lt(std::shared_ptr<Node> left, std::shared_ptr<Node> right) {return *left < *right;}
        static bool ptr_eq(std::shared_ptr<Node> left, std::shared_ptr<Node> right) {return *left == *right;}
};

#endif // ELEMENT_H_3KLA