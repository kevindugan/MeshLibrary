#include "Node.h"

Node::Node(const unsigned int dim,
           const std::vector<float> &coords) : dimension(dim) {
               assert (coords.size() == dim);
               
           }