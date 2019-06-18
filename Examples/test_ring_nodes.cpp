#include "Node.h"
#include <set>
#include <algorithm>
#include <chrono>

using namespace std::chrono;


int main(){

    // input
    std::vector<float> x0 = { 0.17,  0.17,  0.17,  0.33,  0.33,  0.33,   0.5,   0.5,  0.17,  0.17,  0.21,  0.29,  0.33,  0.33,  0.29,  0.21,  0.38,  0.45,   0.5,   0.5,  0.45,  0.38,  0.17,  0.17,  0.21,  0.29,  0.33,  0.33,  0.21,  0.21,  0.29,  0.29,  0.38,  0.38,  0.45,  0.45,  0.21,  0.21,  0.29,  0.29, };
    std::vector<float> y0 = {-0.33, -0.17,     0, -0.33, -0.17,     0, -0.17,     0, -0.046, -0.12, -0.17, -0.17, -0.12, -0.046,     0,     0, -0.17, -0.17, -0.12, -0.046,     0,     0, -0.21, -0.29, -0.33, -0.33, -0.29, -0.21, -0.046, -0.12, -0.046, -0.12, -0.046, -0.12, -0.046, -0.12, -0.21, -0.29, -0.21, -0.29, };
    std::vector<float> z0 = {    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0, };
    std::set<unsigned int> intNodeId0 =  {    5,     7,    10,    11,    12,    13,    14,    15,    18,    19,    20,    21,    28,    29,    30,    31,    32,    33,    34,    35,    36,    37,    38,    39, };
    std::set<unsigned int> ringNodeId0 = {    0,     1,     2,     3,     4,     6,     8,     9,    16,    17,    22,    23,    24,    25,    26,    27, };

    // output
    std::vector<unsigned int> exp_ringNodeOwnership0 = {    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0, };
    std::vector<unsigned int> exp_ringNodeIndex0 =     {    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0, };
    std::vector<unsigned int> exp_ownership0 =         {    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0, };

    // input
    std::vector<float> x1 = {    0,     0,     0,     0,  0.17,  0.17,  0.17,  0.17,     0,     0, 0.046,  0.12,  0.17,  0.17,  0.12, 0.046,     0,     0, 0.046,  0.12,  0.17,  0.17,     0,     0, 0.046,  0.12,  0.17,  0.17, 0.046, 0.046,  0.12,  0.12, 0.046, 0.046,  0.12,  0.12, 0.046, 0.046,  0.12,  0.12, };
    std::vector<float> y1 = { -0.5, -0.33, -0.17,     0,  -0.5, -0.33, -0.17,     0, -0.046, -0.12, -0.17, -0.17, -0.12, -0.046,     0,     0, -0.21, -0.29, -0.33, -0.33, -0.29, -0.21, -0.38, -0.45,  -0.5,  -0.5, -0.45, -0.38, -0.046, -0.12, -0.046, -0.12, -0.21, -0.29, -0.21, -0.29, -0.38, -0.45, -0.38, -0.45, };
    std::vector<float> z1 = {    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0, };
    std::set<unsigned int> intNodeId1 =  {    0,     1,     2,     3,     8,     9,    10,    11,    14,    15,    16,    17,    18,    19,    22,    23,    24,    25,    28,    29,    30,    31,    32,    33,    34,    35,    36,    37,    38,    39, };
    std::set<unsigned int> ringNodeId1 = {    4,     5,     6,     7,    12,    13,    20,    21,    26,    27, };

    // output
    std::vector<unsigned int> exp_ringNodeOwnership1 = {    1,     0,     0,     0,     0,     0,     0,     0,     1,     1, };
    std::vector<unsigned int> exp_ringNodeIndex1 =     {    0,     0,     1,     2,     7,     6,    11,    10,     0,     0, };
    std::vector<unsigned int> exp_ownership1 =         {    1,     1,     1,     1,     1,     0,     0,     0,     1,     1,     1,     1,     0,     0,     1,     1,     1,     1,     1,     1,     0,     0,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1, };

    // input
    std::vector<float> x2 = { 0.17,  0.17,  0.33,  0.33,  0.33,   0.5,   0.5,   0.5,  0.33,  0.33,  0.38,  0.45,   0.5,   0.5,  0.45,  0.38,  0.17,  0.17,  0.21,  0.29,  0.33,  0.33,  0.29,  0.21,  0.38,  0.45,   0.5,   0.5,  0.38,  0.38,  0.45,  0.45,  0.21,  0.21,  0.29,  0.29,  0.38,  0.38,  0.45,  0.45, };
    std::vector<float> y2 = { -0.5, -0.33,  -0.5, -0.33, -0.17,  -0.5, -0.33, -0.17, -0.21, -0.29, -0.33, -0.33, -0.29, -0.21, -0.17, -0.17, -0.38, -0.45,  -0.5,  -0.5, -0.45, -0.38, -0.33, -0.33,  -0.5,  -0.5, -0.45, -0.38, -0.21, -0.29, -0.21, -0.29, -0.38, -0.45, -0.38, -0.45, -0.38, -0.45, -0.38, -0.45, };
    std::vector<float> z2 = {    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0, };
    std::set<unsigned int> intNodeId2 =  {    2,     5,     6,    10,    11,    12,    13,    18,    19,    20,    21,    24,    25,    26,    27,    28,    29,    30,    31,    32,    33,    34,    35,    36,    37,    38,    39, };
    std::set<unsigned int> ringNodeId2 = {    0,     1,     3,     4,     7,     8,     9,    14,    15,    16,    17,    22,    23, };

    // output
    std::vector<unsigned int> exp_ringNodeOwnership2 = {    1,     0,     0,     0,     0,     0,     0,     0,     0,     1,     1,     0,     0, };
    std::vector<unsigned int> exp_ringNodeIndex2 =     {    0,     0,     3,     4,     5,    15,    14,     9,     8,     9,     8,    13,    12, };
    std::vector<unsigned int> exp_ownership2 =         {    1,     0,     2,     0,     0,     2,     2,     0,     0,     0,     2,     2,     2,     2,     0,     0,     1,     1,     2,     2,     2,     2,     0,     0,     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,     2, };


    printf("Size: x = %4d, y = %4d, z = %4d\n", int(x0.size()), int(y0.size()), int(z0.size()));
    printf("Size: x = %4d, y = %4d, z = %4d\n", int(x1.size()), int(y1.size()), int(z1.size()));
    printf("Size: x = %4d, y = %4d, z = %4d\n", int(x2.size()), int(y2.size()), int(z2.size()));
    printf("Size: intNodeId = %4d, ringNodeId = %4d, sum = %4d\n", int(intNodeId0.size()), int(ringNodeId0.size()), (int(intNodeId0.size()) + int(ringNodeId0.size())));
    printf("Size: intNodeId = %4d, ringNodeId = %4d, sum = %4d\n", int(intNodeId1.size()), int(ringNodeId1.size()), (int(intNodeId1.size()) + int(ringNodeId1.size())));
    printf("Size: intNodeId = %4d, ringNodeId = %4d, sum = %4d\n", int(intNodeId2.size()), int(ringNodeId2.size()), (int(intNodeId2.size()) + int(ringNodeId2.size())));
    printf("Size: ringOwner = %4d, ringNodeInd = %4d, ownwership = %4d\n", int(exp_ringNodeOwnership0.size()), int(exp_ringNodeIndex0.size()), int(exp_ownership0.size()));
    printf("Size: ringOwner = %4d, ringNodeInd = %4d, ownwership = %4d\n", int(exp_ringNodeOwnership1.size()), int(exp_ringNodeIndex1.size()), int(exp_ownership1.size()));
    printf("Size: ringOwner = %4d, ringNodeInd = %4d, ownwership = %4d\n", int(exp_ringNodeOwnership2.size()), int(exp_ringNodeIndex2.size()), int(exp_ownership2.size()));

    // Setup Ring nodes
    std::vector<Node> ringNodes0;
    ringNodes0.reserve(ringNodeId0.size());
    for (auto it = ringNodeId0.begin(); it != ringNodeId0.end(); it++){
        ringNodes0.push_back(Node(3, {x0[*it], y0[*it], z0[*it]}));
    }
    std::vector<Node> ringNodes1;
    ringNodes1.reserve(ringNodeId1.size());
    for (auto it = ringNodeId1.begin(); it != ringNodeId1.end(); it++){
        ringNodes1.push_back(Node(3, {x1[*it], y1[*it], z1[*it]}));
    }
    std::vector<Node> ringNodes2;
    ringNodes2.reserve(ringNodeId2.size());
    for (auto it = ringNodeId2.begin(); it != ringNodeId2.end(); it++){
        ringNodes2.push_back(Node(3, {x2[*it], y2[*it], z2[*it]}));
    }

    // Setup neighbor ring nodes
    std::vector<std::vector<Node>> neighborRingNodes0(3);
    std::vector<std::vector<Node>> neighborRingNodes1(3);
    std::vector<std::vector<Node>> neighborRingNodes2(3);
    for (const auto node : ringNodes0){
        neighborRingNodes1[0].push_back(node);
        neighborRingNodes2[0].push_back(node);
    }
    for (const auto node : ringNodes1){
        neighborRingNodes0[1].push_back(node);
        neighborRingNodes2[1].push_back(node);
    }
    for (const auto node : ringNodes2){
        neighborRingNodes0[2].push_back(node);
        neighborRingNodes1[2].push_back(node);
    }

    // unsigned int index = 0;
    // printf("Ring Nodes0:  ", index);
    // for (const auto vertex : ringNodes0){
    //     auto coords = vertex.getCoords();
    //     printf("(%5.2f, %5.2f) ", coords[0], coords[1]);
    // }
    // printf("\n");
    // index = 0;
    // printf("Ring Nodes1:  ", index);
    // for (const auto vertex : ringNodes1){
    //     auto coords = vertex.getCoords();
    //     printf("(%5.2f, %5.2f) ", coords[0], coords[1]);
    // }
    // printf("\n");
    // index = 0;
    // printf("Ring Nodes2:  ", index);
    // for (const auto vertex : ringNodes2){
    //     auto coords = vertex.getCoords();
    //     printf("(%5.2f, %5.2f) ", coords[0], coords[1]);
    // }
    // printf("\n");
    // index = 0;
    // for (const auto neighbor : neighborRingNodes0){
    //     printf("Neighbor0[%2d]: ", index);
    //     for (const auto vertex : neighbor){
    //         auto coords = vertex.getCoords();
    //         printf("(%5.2f, %5.2f) ", coords[0], coords[1]);
    //     }
    //     printf("\n");
    //     index++;
    // }
    // index = 0;
    // for (const auto neighbor : neighborRingNodes1){
    //     printf("Neighbor1[%2d]: ", index);
    //     for (const auto vertex : neighbor){
    //         auto coords = vertex.getCoords();
    //         printf("(%5.2f, %5.2f) ", coords[0], coords[1]);
    //     }
    //     printf("\n");
    //     index++;
    // }
    // index = 0;
    // for (const auto neighbor : neighborRingNodes2){
    //     printf("Neighbor2[%2d]: ", index);
    //     for (const auto vertex : neighbor){
    //         auto coords = vertex.getCoords();
    //         printf("(%5.2f, %5.2f) ", coords[0], coords[1]);
    //     }
    //     printf("\n");
    //     index++;
    // }


    //===================================================================
    // Original Algo
    //===================================================================
    auto start = high_resolution_clock::now();
    unsigned int myProcessId = 0;
    std::vector<unsigned int> neighborProcessIds0 = {1, 2};
    std::vector<unsigned int> ringNodeOwnership0(ringNodes0.size(), myProcessId);
    std::vector<unsigned int> ringNodeIndices0(ringNodes0.size());
    for (unsigned int n = 0; n < ringNodes0.size(); ++n){
        bool found = false;
        Node &myNode = ringNodes0[n];
        for(unsigned int p = 0; p < neighborProcessIds0.size(); ++p){
            const unsigned int pid = neighborProcessIds0[p];
            if (pid > myProcessId)
                continue;
            const unsigned int count = neighborRingNodes0[pid].size();
            for (unsigned int nn = 0; nn < count; ++nn){
                Node &node = neighborRingNodes0[pid][nn];
                if (myNode == node){
                    ringNodeOwnership0[n] = pid;
                    ringNodeIndices0[n] = nn;
                    found = true;
                    break;
                }
            }
            if (found)
                break;
        }
    }
    assert (exp_ringNodeOwnership0.size() == ringNodeOwnership0.size());
    assert (exp_ringNodeIndex0.size() == ringNodeIndices0.size());
    for (unsigned int i = 0; i < exp_ringNodeOwnership0.size(); i++){
        assert ( exp_ringNodeOwnership0[i] == ringNodeOwnership0[i] );
    }
    for (unsigned int i = 0; i < exp_ringNodeIndex0.size(); i++){
        assert ( exp_ringNodeIndex0[i] == ringNodeIndices0[i] );
    }
    auto duration = duration_cast<milliseconds>(high_resolution_clock::now() - start);
    printf("Rank 0 Elapsed: %dms\n", int(duration.count()));
    //===================================================================
    //===================================================================
    start = high_resolution_clock::now();
    myProcessId = 1;
    std::vector<unsigned int> neighborProcessIds1 = {0, 2};
    std::vector<unsigned int> ringNodeOwnership1(ringNodes1.size(), myProcessId);
    std::vector<unsigned int> ringNodeIndices1(ringNodes1.size());
    for (unsigned int n = 0; n < ringNodes1.size(); ++n){
        bool found = false;
        Node &myNode = ringNodes1[n];
        for(unsigned int p = 0; p < neighborProcessIds1.size(); ++p){
            const unsigned int pid = neighborProcessIds1[p];
            if (pid > myProcessId)
                continue;
            const unsigned int count = neighborRingNodes1[pid].size();
            for (unsigned int nn = 0; nn < count; ++nn){
                Node &node = neighborRingNodes1[pid][nn];
                if (myNode == node){
                    ringNodeOwnership1[n] = pid;
                    ringNodeIndices1[n] = nn;
                    found = true;
                    break;
                }
            }
            if (found)
                break;
        }
    }
    assert (exp_ringNodeOwnership1.size() == ringNodeOwnership1.size());
    assert (exp_ringNodeIndex1.size() == ringNodeIndices1.size());
    for (unsigned int i = 0; i < exp_ringNodeOwnership1.size(); i++){
        assert ( exp_ringNodeOwnership1[i] == ringNodeOwnership1[i] );
    }
    // for (unsigned int i = 0; i < exp_ringNodeIndex1.size(); i++)
    //     printf("%4d --> %4d\n",exp_ringNodeIndex1[i], ringNodeIndices1[i]);
    for (unsigned int i = 0; i < exp_ringNodeIndex1.size(); i++){
        assert ( exp_ringNodeIndex1[i] == ringNodeIndices1[i] );
    }
    duration = duration_cast<milliseconds>(high_resolution_clock::now() - start);
    printf("Rank 1 Elapsed: %dms\n", int(duration.count()));
    //===================================================================
    //===================================================================
    start = high_resolution_clock::now();
    myProcessId = 2;
    std::vector<unsigned int> neighborProcessIds2 = {0, 1};
    std::vector<unsigned int> ringNodeOwnership2(ringNodes2.size(), myProcessId);
    std::vector<unsigned int> ringNodeIndices2(ringNodes2.size());
    for (unsigned int n = 0; n < ringNodes2.size(); ++n){
        bool found = false;
        Node &myNode = ringNodes2[n];
        for(unsigned int p = 0; p < neighborProcessIds2.size(); ++p){
            const unsigned int pid = neighborProcessIds2[p];
            if (pid > myProcessId)
                continue;
            const unsigned int count = neighborRingNodes2[pid].size();
            for (unsigned int nn = 0; nn < count; ++nn){
                Node &node = neighborRingNodes2[pid][nn];
                if (myNode == node){
                    ringNodeOwnership2[n] = pid;
                    ringNodeIndices2[n] = nn;
                    found = true;
                    break;
                }
            }
            if (found)
                break;
        }
    }
    assert (exp_ringNodeOwnership2.size() == ringNodeOwnership2.size());
    assert (exp_ringNodeIndex2.size() == ringNodeIndices2.size());
    for (unsigned int i = 0; i < exp_ringNodeOwnership2.size(); i++){
        assert ( exp_ringNodeOwnership2[i] == ringNodeOwnership2[i] );
    }
    // for (unsigned int i = 0; i < exp_ringNodeIndex2.size(); i++)
    //     printf("%4d --> %4d\n",exp_ringNodeIndex2[i], ringNodeIndices2[i]);
    for (unsigned int i = 0; i < exp_ringNodeIndex2.size(); i++){
        assert ( exp_ringNodeIndex2[i] == ringNodeIndices2[i] );
    }
    duration = duration_cast<milliseconds>(high_resolution_clock::now() - start);
    printf("Rank 2 Elapsed: %dms\n", int(duration.count()));


    return 0;
}