#ifndef NODE_H_0V9A
#define NODE_H_0V9A

#include <vector>
#include <assert.h>

class Node {

    public:
        Node(const unsigned int dim, 
             const std::vector<float> &coords);

    private:
        unsigned int dimension;
        std::vector<float> coordinates;
};

#endif // NODE_H_0V9A