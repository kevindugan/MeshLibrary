#include "Node.h"
#include <set>
#include <algorithm>
#include <chrono>

using namespace std::chrono;

struct testRingNodes {
    unsigned int processorId;
    std::vector<float> x, y, z;
    std::set<unsigned int> interiorNodeId, ringNodeId;
    std::vector<unsigned int> exp_ringNodeOwnership, exp_ringNodeIndex, exp_ownership;

    std::vector<Node> ringNodes;
    std::vector<std::vector<Node>> neighborRingNodes;
    std::vector<unsigned int> neighborProcessIds;

    std::vector<unsigned int> ringNodeOwnership, ringNodeIndices;
    std::vector<unsigned int> ownership;
};

int main(){

    std::vector<testRingNodes> processorInfo;

    // input
    processorInfo.push_back(testRingNodes());
    processorInfo[0].processorId = 0;
    processorInfo[0].x = { 0.17,  0.17,  0.17,  0.33,  0.33,  0.33,   0.5,   0.5,  0.17,  0.17,  0.21,  0.29,  0.33,  0.33,  0.29,  0.21,  0.38,  0.45,   0.5,   0.5,  0.45,  0.38,  0.17,  0.17,  0.21,  0.29,  0.33,  0.33,  0.21,  0.21,  0.29,  0.29,  0.38,  0.38,  0.45,  0.45,  0.21,  0.21,  0.29,  0.29, };
    processorInfo[0].y = {-0.33, -0.17,     0, -0.33, -0.17,     0, -0.17,     0, -0.046, -0.12, -0.17, -0.17, -0.12, -0.046,     0,     0, -0.17, -0.17, -0.12, -0.046,     0,     0, -0.21, -0.29, -0.33, -0.33, -0.29, -0.21, -0.046, -0.12, -0.046, -0.12, -0.046, -0.12, -0.046, -0.12, -0.21, -0.29, -0.21, -0.29, };
    processorInfo[0].z = {    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0, };
    processorInfo[0].interiorNodeId = {    5,     7,    10,    11,    12,    13,    14,    15,    18,    19,    20,    21,    28,    29,    30,    31,    32,    33,    34,    35,    36,    37,    38,    39, };
    processorInfo[0].ringNodeId =     {    0,     1,     2,     3,     4,     6,     8,     9,    16,    17,    22,    23,    24,    25,    26,    27, };

    // output
    processorInfo[0].exp_ringNodeOwnership = {    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0, };
    processorInfo[0].exp_ringNodeIndex =     {    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0, };
    processorInfo[0].exp_ownership =         {    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0, };

    // input
    processorInfo.push_back(testRingNodes());
    processorInfo[1].processorId = 1;
    processorInfo[1].x = {    0,     0,     0,     0,  0.17,  0.17,  0.17,  0.17,     0,     0, 0.046,  0.12,  0.17,  0.17,  0.12, 0.046,     0,     0, 0.046,  0.12,  0.17,  0.17,     0,     0, 0.046,  0.12,  0.17,  0.17, 0.046, 0.046,  0.12,  0.12, 0.046, 0.046,  0.12,  0.12, 0.046, 0.046,  0.12,  0.12, };
    processorInfo[1].y = { -0.5, -0.33, -0.17,     0,  -0.5, -0.33, -0.17,     0, -0.046, -0.12, -0.17, -0.17, -0.12, -0.046,     0,     0, -0.21, -0.29, -0.33, -0.33, -0.29, -0.21, -0.38, -0.45,  -0.5,  -0.5, -0.45, -0.38, -0.046, -0.12, -0.046, -0.12, -0.21, -0.29, -0.21, -0.29, -0.38, -0.45, -0.38, -0.45, };
    processorInfo[1].z = {    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0, };
    processorInfo[1].interiorNodeId = {    0,     1,     2,     3,     8,     9,    10,    11,    14,    15,    16,    17,    18,    19,    22,    23,    24,    25,    28,    29,    30,    31,    32,    33,    34,    35,    36,    37,    38,    39, };
    processorInfo[1].ringNodeId =     {    4,     5,     6,     7,    12,    13,    20,    21,    26,    27, };

    // output
    processorInfo[1].exp_ringNodeOwnership = {    1,     0,     0,     0,     0,     0,     0,     0,     1,     1, };
    processorInfo[1].exp_ringNodeIndex =     {    0,     0,     1,     2,     7,     6,    11,    10,     0,     0, };
    processorInfo[1].exp_ownership =         {    1,     1,     1,     1,     1,     0,     0,     0,     1,     1,     1,     1,     0,     0,     1,     1,     1,     1,     1,     1,     0,     0,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1, };

    // input
    processorInfo.push_back(testRingNodes());
    processorInfo[2].processorId = 2;
    processorInfo[2].x = { 0.17,  0.17,  0.33,  0.33,  0.33,   0.5,   0.5,   0.5,  0.33,  0.33,  0.38,  0.45,   0.5,   0.5,  0.45,  0.38,  0.17,  0.17,  0.21,  0.29,  0.33,  0.33,  0.29,  0.21,  0.38,  0.45,   0.5,   0.5,  0.38,  0.38,  0.45,  0.45,  0.21,  0.21,  0.29,  0.29,  0.38,  0.38,  0.45,  0.45, };
    processorInfo[2].y = { -0.5, -0.33,  -0.5, -0.33, -0.17,  -0.5, -0.33, -0.17, -0.21, -0.29, -0.33, -0.33, -0.29, -0.21, -0.17, -0.17, -0.38, -0.45,  -0.5,  -0.5, -0.45, -0.38, -0.33, -0.33,  -0.5,  -0.5, -0.45, -0.38, -0.21, -0.29, -0.21, -0.29, -0.38, -0.45, -0.38, -0.45, -0.38, -0.45, -0.38, -0.45, };
    processorInfo[2].z = {    0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0, };
    processorInfo[2].interiorNodeId = {    2,     5,     6,    10,    11,    12,    13,    18,    19,    20,    21,    24,    25,    26,    27,    28,    29,    30,    31,    32,    33,    34,    35,    36,    37,    38,    39, };
    processorInfo[2].ringNodeId =     {    0,     1,     3,     4,     7,     8,     9,    14,    15,    16,    17,    22,    23, };

    // output
    processorInfo[2].exp_ringNodeOwnership = {    1,     0,     0,     0,     0,     0,     0,     0,     0,     1,     1,     0,     0, };
    processorInfo[2].exp_ringNodeIndex =     {    0,     0,     3,     4,     5,    15,    14,     9,     8,     9,     8,    13,    12, };
    processorInfo[2].exp_ownership =         {    1,     0,     2,     0,     0,     2,     2,     0,     0,     0,     2,     2,     2,     2,     0,     0,     1,     1,     2,     2,     2,     2,     0,     0,     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,     2, };

    // Neighbor process ids
    std::vector<unsigned int> neighborIds;
    for (const auto &proc : processorInfo)
        neighborIds.push_back(proc.processorId);

    for (auto &proc : processorInfo){
        proc.neighborProcessIds.reserve(neighborIds.size()-1);
        for (const auto &id : neighborIds){
            if (id == proc.processorId)
                continue;
            proc.neighborProcessIds.push_back(id);
        }
    }

    for (const auto &proc : processorInfo)
        printf("Size: x = %4d, y = %4d, z = %4d\n", int(proc.x.size()), int(proc.y.size()), int(proc.z.size()));
    for (const auto &proc : processorInfo)
        printf("Size: intNodeId = %4d, ringNodeId = %4d, sum = %4d\n", int(proc.interiorNodeId.size()), int(proc.ringNodeId.size()), (int(proc.interiorNodeId.size()) + int(proc.ringNodeId.size())));
    for (const auto &proc : processorInfo)
        printf("Size: ringOwner = %4d, ringNodeInd = %4d, ownwership = %4d\n", int(proc.exp_ringNodeOwnership.size()), int(proc.exp_ringNodeIndex.size()), int(proc.exp_ownership.size()));
    for (const auto &proc :processorInfo){
        printf("Neighbors: ");
        for (const auto n : proc.neighborProcessIds)
            printf("%4d", n);
        printf("\n");
    }


    // Setup Ring nodes
    for (auto &proc : processorInfo){
        proc.ringNodes.reserve(proc.ringNodeId.size());
        for (auto it = proc.ringNodeId.begin(); it != proc.ringNodeId.end(); it++){
            proc.ringNodes.push_back(Node(3, {proc.x[*it], proc.y[*it], proc.z[*it]}));
        }
    }

    // Setup neighbor ring nodes
    for (auto &proc : processorInfo)
        proc.neighborRingNodes.resize(processorInfo.size());
    for (unsigned int i = 0; i < processorInfo.size(); i++){
        for (const auto node : processorInfo[i].ringNodes){
            for (unsigned int j = 0; j < processorInfo.size(); j++){
                if (i == j)
                    continue;
                processorInfo[j].neighborRingNodes[i].push_back(node);
            }
        }
    }

    unsigned int procIndex = 0;
    for (const auto &proc : processorInfo){
        printf("Ring Nodes%d: ", procIndex);
        for (const auto vertex : proc.ringNodes){
            auto coords = vertex.getCoords();
            printf("(%5.2f, %5.2f) ", coords[0], coords[1]);
        }
        printf("\n");
        procIndex++;
    }
    procIndex = 0;
    for (const auto &proc : processorInfo){
        unsigned int index = 0;
        for (const auto &neighbor : proc.neighborRingNodes){
            printf("Neighbor%d[%2d]: ", procIndex, index);
            for (const auto vertex : neighbor){
                auto coords = vertex.getCoords();
                printf("(%5.2f, %5.2f) ", coords[0], coords[1]);
            }
            printf("\n");
            index++;
        }
        procIndex++;
    }

    //===================================================================
    // Original Algo
    //===================================================================
    for (auto &proc : processorInfo){
        auto start = high_resolution_clock::now();
        proc.ringNodeOwnership.resize(proc.ringNodes.size(), proc.processorId);
        proc.ringNodeIndices.resize(proc.ringNodes.size());
        for (unsigned int n = 0; n < proc.ringNodes.size(); ++n){
            bool found = false;
            Node &myNode = proc.ringNodes[n];
            for (unsigned int p = 0; p < proc.neighborProcessIds.size(); ++p){
                const unsigned int pid = proc.neighborProcessIds[p];
                if (pid > proc.processorId)
                    continue;
                const unsigned int count = proc.neighborRingNodes[pid].size();
                for (unsigned int nn = 0; nn < count; ++nn){
                    Node &node = proc.neighborRingNodes[pid][nn];
                    if (myNode == node){
                        proc.ringNodeOwnership[n] = pid;
                        proc.ringNodeIndices[n] = nn;
                        found = true;
                        break;
                    }
                }
                if (found)
                    break;
            }
        }
        auto duration = duration_cast<milliseconds>(high_resolution_clock::now() - start);
        printf("Rank %2d Elapsed: %dms\n", proc.processorId, int(duration.count()));

        assert (proc.exp_ringNodeOwnership.size() == proc.ringNodeOwnership.size());
        assert (proc.exp_ringNodeIndex.size() == proc.ringNodeIndices.size());
        printf("Ownership\n");
        for (unsigned int i = 0; i < proc.exp_ringNodeOwnership.size(); i++)
            printf("%4d --> %4d\n", proc.exp_ringNodeOwnership[i], proc.ringNodeOwnership[i]);
        for (unsigned int i = 0; i < proc.exp_ringNodeOwnership.size(); i++){
            assert ( proc.exp_ringNodeOwnership[i] == proc.ringNodeOwnership[i] );
        }
        printf("Indices\n");
        for (unsigned int i = 0; i < proc.exp_ringNodeIndex.size(); i++)
            printf("%4d --> %4d\n", proc.exp_ringNodeIndex[i], proc.ringNodeIndices[i]);
        for (unsigned int i = 0; i < proc.exp_ringNodeIndex.size(); i++){
            assert ( proc.exp_ringNodeIndex[i] == proc.ringNodeIndices[i] );
        }
    }

    // Get Final Ownership
    for (auto & proc : processorInfo){
        proc.ownership.assign(proc.x.size(), proc.processorId);
        unsigned int n = 0;
        for (auto it = proc.ringNodeId.begin(); it != proc.ringNodeId.end(); it++ ){
            proc.ownership[*it] = proc.ringNodeOwnership[n];
            n++;
        }
        
        assert(proc.ownership.size() == proc.exp_ownership.size());
        printf("Proc %2d: ", proc.processorId);
        for (const auto n : proc.ownership)
            printf("%4d", n);
        printf("\n");
        
        for (unsigned int i = 0; i < proc.exp_ownership.size(); i++)
            assert( proc.ownership[i] == proc.exp_ownership[i] );
    }

    return 0;
}