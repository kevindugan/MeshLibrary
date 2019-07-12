#ifndef MESH_H_C0A0
#define MESH_H_C0A0

#include "Element.h"
#include <iomanip>
#include <chrono>
#include "metis.h"
#include <bitset>

using Pair_UI_UI = std::pair<std::vector<unsigned int>, std::vector<unsigned int>>;

class Mesh {

    public:
        Mesh() = delete;
        Mesh(const std::vector<float> &,
             const std::vector<float> &,
             const std::vector<unsigned int> &,
             unsigned int processor=0);

        void print(std::ostream &out=std::cout) const;
        void outputVTK(std::ostream &out=std::cout) const;
        
        Pair_UI_UI getElementAdjacency() const;
        void partitionMesh(const unsigned int nPartitions);

        std::vector<unsigned int> getPartition() const {return partition;}
        void setPartition(const std::vector<unsigned int> &other) {partition = other;}
        std::vector<std::shared_ptr<Node>> getVertices() const {return vertices;}

        void findRingNodes();
        std::vector<unsigned int> getRingNodes(const unsigned int i) const {return ringNodeIndices[i];}

        template<typename T, typename length>
        static std::string base64_encode(const std::vector<T> &vals){
            std::string b64_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

            union {
                T input;
                length output;
            } data;

            // std::cout << "Size of: " << sizeof(length)*8 << std::endl;

            std::string bit_string = "";
            bit_string.reserve(sizeof(length)*8 * vals.size());
            for (const auto item : vals){
                data.input = item;
                // std::cout << item << " -> " << std::bitset<sizeof(length)*8>(data.output) << std::endl;
                bit_string += std::bitset<sizeof(length)*8>(data.output).to_string();
            }

            std::string result = "";
            for (unsigned int bit_index = 0; bit_index < bit_string.size(); bit_index += 24){
                std::string block_bit_value = bit_string.substr(bit_index, 24);
                // block_bit_value.append(6 - block_bit_value.size(), '0');
                // std::cout << block_bit_value.c_str() << std::endl;

                for (unsigned int i = 0; i < 24; i+=6){
                    if (block_bit_value.size() < i)
                        break;
                    std::string chunk_bit_value = block_bit_value.substr(i, 6);
                    chunk_bit_value.append(6-chunk_bit_value.size(), '0');
                    // std::cout << chunk_bit_value.c_str() << std::endl;

                    unsigned int table_index = std::bitset<6>(chunk_bit_value).to_ulong();
                    // std::cout << table_index << std::endl;

                    result += b64_table[table_index];
                }
            }

            result.append( (result.size()-1) % 3, '=');
            return result;
        }

    private:
        unsigned int dimension, owningProcessor;
        std::vector<unsigned int> partition;
        std::vector<std::shared_ptr<Node>> vertices;
        std::vector<std::shared_ptr<Element>> elements;
        std::vector<std::vector<unsigned int>> elementConnectivity;
        std::vector<std::vector<unsigned int>> ringNodeIndices;

        void generateVertices(const std::vector<float> &minVerts,
                              const std::vector<float> &maxVerts,
                              const std::vector<unsigned int> &nCells);
        void generateElements(const std::vector<unsigned int> &nCells);
};

#endif // MESH_H_C0A0