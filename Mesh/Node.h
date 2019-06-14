#ifndef NODE_H_0V9A
#define NODE_H_0V9A

#include <vector>
#include <assert.h>
#include <cmath>
#include <iostream>

class Node {

    public:
        Node() = delete;
        Node(const unsigned int dim, 
             const std::vector<float> &coords);

        std::vector<float> getCoords() const {return this->coordinates;}
        unsigned int getDimension() const {return this->dimension;}

        bool operator==(const Node &) const;
        bool operator<(const Node &) const;
        bool isAt(const std::vector<float> &coords) const;

    private:
        unsigned int dimension;
        std::vector<float> coordinates;
        constexpr static const float eps = 1.0e-6;
};

#endif // NODE_H_0V9A