#include "Element.h"

Element::Element(const unsigned int dim,
                 const std::vector<Node> &vertices){

    // Forcing quad elements
    assert (vertices.size() == std::pow(2, dim));
    // Testing that vertices are coorect dimension
    for (const auto &node : vertices)
        assert (node.getDimension() == dim);

    // Testing that vertices are unique
    std::vector<Node> testUnique = vertices;
    std::sort(testUnique.begin(), testUnique.end());
    auto it = std::unique(testUnique.begin(), testUnique.end());
    assert (it == testUnique.end());


    this->vertices = vertices;
}