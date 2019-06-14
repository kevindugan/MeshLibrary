#include "Node.h"

Node::Node(const unsigned int dim,
           const std::vector<float> &coords) : dimension(dim) {

    assert (coords.size() == dim);
    assert ( dim >= 1 && dim <= 3);

    this->coordinates = coords;               
}

bool Node::operator==(const Node &other) const {
    assert (this->dimension == other.getDimension());

    bool result = true;
    std::vector<float> otherCoords = other.getCoords();
    for (unsigned int i = 0; i < this->dimension; i++)
        result &= (fabs( this->coordinates[i] - otherCoords[i] ) < this->eps);

    return result;
}

bool Node::operator<(const Node &other) const {
    assert (other.getDimension() == this->dimension);
    return this->coordinates < other.getCoords();
}

bool Node::isAt(const std::vector<float> &coords) const {
    assert (coords.size() == dimension);

    bool result = true;
    for (unsigned int i = 0; i < this->dimension; i++)
        result &= (fabs( this->coordinates[i] - coords[i] ) < this->eps);

    return result;
}