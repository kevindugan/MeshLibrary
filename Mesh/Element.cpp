#include "Element.h"

Element::Element(const unsigned int dim,
                 const std::vector<std::shared_ptr<Node>> &vertices) : dimension(dim) {

    // Forcing quad elements
    assert (vertices.size() == std::pow(2, dim));
    // Testing that vertices are coorect dimension
    for (const auto &node : vertices)
        assert (node->getDimension() == dim);

    // Testing that vertices are unique
    std::vector<std::shared_ptr<Node>> testUnique = vertices;
    std::sort(testUnique.begin(), testUnique.end(), ptr_lt<Node>);
    auto it = std::unique(testUnique.begin(), testUnique.end(), ptr_eq<Node>);
    assert (it == testUnique.end());


    this->vertices = vertices;
}

bool Element::operator==(const Element &other) const {
    auto otherVert = other.getVertices();
    auto myVert = this->vertices;
    assert (otherVert.size() == myVert.size());

    std::sort(otherVert.begin(), otherVert.end(), ptr_lt<Node>);
    std::sort(myVert.begin(), myVert.end(), ptr_lt<Node>);
    
    bool result = true;
    for (unsigned int i = 0; i < myVert.size(); i++)
        result &= (*myVert[i] == *otherVert[i]);

    return result;
}

bool Element::operator<(const Element &other) const {
    auto otherVert = other.getVertices();
    auto myVert = this->vertices;
    assert (otherVert.size() == myVert.size());

    std::sort(otherVert.begin(), otherVert.end(), ptr_lt<Node>);
    std::sort(myVert.begin(), myVert.end(), ptr_lt<Node>);

    std::vector<float> left, right;
    left.reserve(myVert.size() * this->dimension);
    right.reserve(myVert.size() * this->dimension);
    for (unsigned int i = 0; i < myVert.size(); i++){
        for (const auto v : myVert[i]->getCoords())
            left.push_back(v);
        for (const auto v : otherVert[i]->getCoords())
            right.push_back(v);
    }
    return left < right;
}

unsigned int Element::numBoundaryNodes() const {
    unsigned int result = 0;
    for (const auto &v : vertices)
        if (v->isOnBoundary())
            result++;

    return result;
}

std::vector<std::shared_ptr<Node>> Element::getBoundaryNodes() const {
    std::vector<std::shared_ptr<Node>> result;
    for (const auto v : vertices)
        if (v->isOnBoundary())
            result.push_back(v);

    return result;
}

unsigned int Element::numFaceVertices() const {
    return pow(2, this->dimension-1);
}

bool Element::isNeighborElement(std::shared_ptr<Element> other){
    auto otherVert = other->getVertices();
    auto myVert = this->vertices;
    assert (otherVert.size() == myVert.size());

    // Sort vertices
    std::sort(otherVert.begin(), otherVert.end(), ptr_lt<Node>);
    std::sort(myVert.begin(), myVert.end(), ptr_lt<Node>);

    // Intersection
    std::vector<std::shared_ptr<Node>> intersection(2*myVert.size());
    auto it = std::set_intersection(myVert.begin(), myVert.end(),
                                    otherVert.begin(), otherVert.end(),
                                    intersection.begin(), ptr_lt<Node>);

    intersection.resize(it - intersection.begin());

    return intersection.size() == this->numFaceVertices();
}

void Element::addNeighborElement(std::shared_ptr<Element> other){
    bool found = false;
    for (const auto elem : neighbors)
        if (*elem == *other)
            found = true;
    
    assert (!found);
    this->neighbors.push_back(other);
}

void Element::removeNeighborElement(std::shared_ptr<Element> other){
    auto it = this->neighbors.begin();
    for (; it != this->neighbors.end(); it++)
        if (**it == *other)
            break;
    assert(it != this->neighbors.end());
    this->neighbors.erase(it);
}

unsigned int Element::numNeighbors() const {
    return this->neighbors.size();
}