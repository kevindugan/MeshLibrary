#include "Node.h"
#include <set>
#include <algorithm>
#include <chrono>

using namespace std::chrono;

struct testRingNodes {
    unsigned int processorId;
    std::vector<float> x, y, z;
    std::set<unsigned int> interiorNodeId_set, ringNodeId_set;
    std::vector<unsigned int> interiorNodeId, ringNodeId;
    std::vector<unsigned int> exp_ringNodeOwnership, exp_ringNodeIndex, exp_ownership;

    std::vector<Node> ringNodes;
    std::vector<std::vector<Node>> neighborRingNodes;
    std::vector<unsigned int> neighborProcessIds;

    std::vector<unsigned int> ringNodeOwnership, ringNodeIndices;
    std::vector<unsigned int> ownership;
};

int main(){

    std::vector<testRingNodes> processorInfo;

    #define TWO_DIMENSIONAL
    #define SMALL
    #ifdef TWO_DIMENSIONAL
        #ifdef LARGE
            #include "ringNodeData/processor2d_large-0.out"
            #include "ringNodeData/processor2d_large-1.out"
            #include "ringNodeData/processor2d_large-2.out"
            #include "ringNodeData/processor2d_large-3.out"
            #include "ringNodeData/processor2d_large-4.out"
            #include "ringNodeData/processor2d_large-5.out"
            #include "ringNodeData/processor2d_large-6.out"
            #include "ringNodeData/processor2d_large-7.out"
        #else
            #include "ringNodeData/processor2d_small-0.out"
            #include "ringNodeData/processor2d_small-1.out"
            #include "ringNodeData/processor2d_small-2.out"
        #endif
    #else
        #ifdef LARGE
            #include "ringNodeData/processor3d_large-0.out"
            #include "ringNodeData/processor3d_large-1.out"
            #include "ringNodeData/processor3d_large-2.out"
            #include "ringNodeData/processor3d_large-3.out"
            #include "ringNodeData/processor3d_large-4.out"
            #include "ringNodeData/processor3d_large-5.out"
            #include "ringNodeData/processor3d_large-6.out"
            #include "ringNodeData/processor3d_large-7.out"
        #else
            #include "ringNodeData/processor3d_small-0.out"
            #include "ringNodeData/processor3d_small-1.out"
            #include "ringNodeData/processor3d_small-2.out"
        #endif
    #endif

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
        printf("Size: intNodeId = %4d, ringNodeId = %4d, sum = %4d\n", int(proc.interiorNodeId_set.size()), int(proc.ringNodeId_set.size()), (int(proc.interiorNodeId_set.size()) + int(proc.ringNodeId_set.size())));
    for (const auto &proc : processorInfo)
        printf("Size: ringOwner = %4d, ringNodeInd = %4d, ownwership = %4d\n", int(proc.exp_ringNodeOwnership.size()), int(proc.exp_ringNodeIndex.size()), int(proc.exp_ownership.size()));
    for (const auto &proc :processorInfo){
        printf("Neighbors: ");
        for (const auto n : proc.neighborProcessIds)
            printf("%4d", n);
        printf("\n");
    }

    // Copy node id sets to vectors
    for (auto &proc: processorInfo){
        proc.interiorNodeId.reserve(proc.interiorNodeId_set.size());
        proc.interiorNodeId.insert(proc.interiorNodeId.begin(), proc.interiorNodeId_set.begin(), proc.interiorNodeId_set.end());
        proc.ringNodeId.reserve(proc.ringNodeId_set.size());
        proc.ringNodeId.insert(proc.ringNodeId.begin(), proc.ringNodeId_set.begin(), proc.ringNodeId_set.end());
    }

    // for (const auto &proc : processorInfo){
    //     printf("RingIndices[%2d]: ",proc.processorId);
    //     for (const auto n : proc.ringNodeId)
    //         printf("%4d",n);
    //     printf("\n");
    // }

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

    // unsigned int procIndex = 0;
    // for (const auto &proc : processorInfo){
    //     printf("Ring Nodes%d: ", procIndex);
    //     for (const auto vertex : proc.ringNodes){
    //         auto coords = vertex.getCoords();
    //         printf("(%5.2f, %5.2f) ", coords[0], coords[1]);
    //     }
    //     printf("\n");
    //     procIndex++;
    // }
    // procIndex = 0;
    // for (const auto &proc : processorInfo){
    //     unsigned int index = 0;
    //     for (const auto &neighbor : proc.neighborRingNodes){
    //         printf("Neighbor%d[%2d]: ", procIndex, index);
    //         for (const auto vertex : neighbor){
    //             auto coords = vertex.getCoords();
    //             printf("(%5.2f, %5.2f) ", coords[0], coords[1]);
    //         }
    //         printf("\n");
    //         index++;
    //     }
    //     procIndex++;
    // }

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
        auto duration = duration_cast<seconds>(high_resolution_clock::now() - start);
        printf("Rank %2d Elapsed: %ds\n", proc.processorId, int(duration.count()));

        assert (proc.exp_ringNodeOwnership.size() == proc.ringNodeOwnership.size());
        assert (proc.exp_ringNodeIndex.size() == proc.ringNodeIndices.size());
        // printf("Ownership\n");
        // for (unsigned int i = 0; i < proc.exp_ringNodeOwnership.size(); i++)
        //     printf("%4d --> %4d\n", proc.exp_ringNodeOwnership[i], proc.ringNodeOwnership[i]);
        for (unsigned int i = 0; i < proc.exp_ringNodeOwnership.size(); i++){
            assert ( proc.exp_ringNodeOwnership[i] == proc.ringNodeOwnership[i] );
        }
        // printf("Indices\n");
        // for (unsigned int i = 0; i < proc.exp_ringNodeIndex.size(); i++)
        //     printf("%4d --> %4d\n", proc.exp_ringNodeIndex[i], proc.ringNodeIndices[i]);
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
        // printf("Proc %2d: ", proc.processorId);
        // for (const auto n : proc.ownership)
        //     printf("%4d", n);
        // printf("\n");
        
        for (unsigned int i = 0; i < proc.exp_ownership.size(); i++)
            assert( proc.ownership[i] == proc.exp_ownership[i] );
    }

    printf("I--Test Passed\n");

    return 0;
}