#include "Mesh.h"
#include <sstream>

Mesh::Mesh(const std::vector<float> &minVerts,
           const std::vector<float> &maxVerts,
           const std::vector<unsigned int> &nCells,
           unsigned int processor){
    
    assert (minVerts.size() == maxVerts.size());
    for (unsigned int i = 0; i < minVerts.size(); i++)
        assert (minVerts[i] < maxVerts[i]);
    assert (maxVerts.size() == nCells.size());
    this->dimension = nCells.size();
    assert (this->dimension >= 1 and this->dimension <= 3);
    this->owningProcessor = processor;

    this->generateVertices(minVerts, maxVerts, nCells);
    this->generateElements(nCells);

    this->partition.resize(this->elements.size(), this->owningProcessor);
}

void Mesh::generateVertices(const std::vector<float> &minVerts,
                            const std::vector<float> &maxVerts,
                            const std::vector<unsigned int> &nCells){

    // Get delta for each direction
    std::vector<float> delta;
    for (unsigned int i = 0; i < this->dimension; i++)
        delta.push_back( (maxVerts[i] - minVerts[i]) / float(nCells[i]) );

    // Setup Cartesian vertices
    unsigned int nVertices = 1;
    for (const auto d : nCells)
        nVertices *= (d+1);
    std::vector<std::vector<float>> vertexCoordinates(nVertices, std::vector<float>(this->dimension, 0.0));
    for (unsigned int dim = 0; dim < this->dimension; dim++){
        unsigned int repeat = 1;
        for (unsigned int d = 0; d < dim; d++)
            repeat *= (nCells[d] + 1);

        unsigned int index = 0;
        while (index < vertexCoordinates.size()){
            for (unsigned int point = 0; point <= nCells[dim]; point++){
                float value = point * delta[dim];
                for (unsigned int r = 0; r < repeat; r++){
                    vertexCoordinates[index][dim] = value;
                    index++;
                }
            }
        }
    }

    // Create Node List
    this->vertices.reserve(vertexCoordinates.size());
    for (const auto v : vertexCoordinates)
        this->vertices.push_back( std::make_shared<Node>(Node(this->dimension, v)) );

    // for (const auto &v : vertices)
    //     if (v.size() == 2)
    //         printf("(%4.1f, %4.1f)\n", v[0], v[1]);
    //     else if (v.size() == 3)
    //         printf("(%4.1f, %4.1f, %4.1f)\n", v[0], v[1], v[2]);
}

void Mesh::generateElements(const std::vector<unsigned int> &nCells){
    assert (this->vertices.size() > 0);

    // Setup Element Connectivity
    unsigned int nElements = 1;
    for (const auto d : nCells)
        nElements *= d;
    unsigned int nVertexPerElement = pow(2, this->dimension);
    this->elementConnectivity.resize(nElements, std::vector<unsigned int>(nVertexPerElement, 0));
    for (unsigned int elem = 0; elem < this->elementConnectivity.size(); elem++){
        
        std::vector<unsigned int> vertexList;
        if (this->dimension == 1){
            unsigned int col = elem;

            vertexList.push_back( col );
            vertexList.push_back( col + 1 );
        }
        if (this->dimension == 2){
            unsigned int col = elem % nCells[0];
            unsigned int row = floor( elem / nCells[0] );
            unsigned int rowOffset = (nCells[0] + 1);

            vertexList.push_back( col + row*rowOffset );
            vertexList.push_back( col + row*rowOffset + 1 );
            vertexList.push_back( col + (row+1)*rowOffset + 1 );
            vertexList.push_back( col + (row+1)*rowOffset );
        } else if (this->dimension == 3){
            unsigned int col = elem % nCells[0];
            unsigned int row = int(floor( elem / nCells[0] )) % nCells[1];
            unsigned int rowOffset = (nCells[0] + 1);
            unsigned int layer = floor( elem / (nCells[0]*nCells[1]) );
            unsigned int layerOffset = (nCells[0] + 1) * (nCells[1] + 1);

            vertexList.push_back( col + row*rowOffset + layer*layerOffset);
            vertexList.push_back( col + row*rowOffset + layer*layerOffset + 1 );
            vertexList.push_back( col + (row+1)*rowOffset + layer*layerOffset + 1 );
            vertexList.push_back( col + (row+1)*rowOffset + layer*layerOffset );
            vertexList.push_back( col + row*rowOffset + (layer+1)*layerOffset);
            vertexList.push_back( col + row*rowOffset + (layer+1)*layerOffset + 1 );
            vertexList.push_back( col + (row+1)*rowOffset + (layer+1)*layerOffset + 1 );
            vertexList.push_back( col + (row+1)*rowOffset + (layer+1)*layerOffset );
        }

        assert (this->elementConnectivity[elem].size() == vertexList.size());
        this->elementConnectivity[elem] = vertexList;
    }

    // Create Element pointer list
    this->elements.reserve(nElements);
    unsigned int elementIndex = 0;
    for (const auto e : this->elementConnectivity){
        std::vector<std::shared_ptr<Node>> nodeList;
        for (const auto index : e)
            nodeList.push_back(this->vertices[index]);
        
        
        auto newElement = std::make_shared<Element>(Element(this->dimension, nodeList));
        // Find Neighbors
        std::vector<int> potentialNeighborIndices;
        potentialNeighborIndices.reserve(pow(2, this->dimension-1));
        unsigned int offset = 1;
        for (unsigned int dim = 0; dim < nCells.size(); dim++){
            potentialNeighborIndices.push_back(elementIndex - offset);
            potentialNeighborIndices.push_back(elementIndex + offset);
            offset *= nCells[dim];
        }
        std::sort(potentialNeighborIndices.begin(), potentialNeighborIndices.end());
        auto outsideRange = [=](const int test){return (test < 0);};
        auto last = std::remove_if(potentialNeighborIndices.begin(), potentialNeighborIndices.end(), outsideRange);
        potentialNeighborIndices.resize(last - potentialNeighborIndices.begin());

        // printf("Potential Neighbors: ");
        // for (const auto n : potentialNeighborIndices)
        //     printf("%6d", n);
        // printf("\n");
        
        for (const auto otherIndex : potentialNeighborIndices){
            if (otherIndex > elementIndex)
                break;
            auto other = this->elements[otherIndex];
            if (newElement->isNeighborElement(other)){
                other->addNeighborElement(newElement);
                newElement->addNeighborElement(other);
            }
        }

        this->elements.push_back( newElement );
        elementIndex++;
    }

    // for (const auto &v : this->elementConnectivity)
    //     if (v.size() == 4)
    //         printf("[%4d, %4d, %4d, %4d]\n", v[0], v[1], v[2], v[3]);
    //     else if (v.size() == 8)
    //         printf("[%4d, %4d, %4d, %4d, %4d, %4d, %4d, %4d]\n", v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7]);
}

void Mesh::outputVTK(std::ostream &out) const {
    auto increaseIndent = [](const std::string &input){ return input + "  ";};
    auto decreaseIndent = [](const std::string &input){ unsigned int length = input.size() - 2;
                                                        return std::string(input.begin(), input.begin()+length);};
    ////////////////////////////////////////////////////////
    // Header
    ////////////////////////////////////////////////////////
    std::string indent = "";
    out << indent << "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"BigEndian\">" << std::endl;
    indent = increaseIndent(indent);
    out << indent << "<UnstructuredGrid>" << std::endl;
    indent = increaseIndent(indent);
    unsigned int nVertexPerElement = pow(2, this->dimension);
    out << indent << "<Piece NumberOfPoints=\"" << this->vertices.size() << "\" "
                    << "NumberOfCells=\"" << this->elements.size() << "\">" << std::endl;
    indent = increaseIndent(indent);

    ////////////////////////////////////////////////////////
    // Point Data
    ////////////////////////////////////////////////////////
    out << indent << "<PointData>" << std::endl;
    indent = increaseIndent(indent);

    indent = decreaseIndent(indent);
    out << indent << "</PointData>" << std::endl;

    ////////////////////////////////////////////////////////
    // Cell Data
    ////////////////////////////////////////////////////////
    out << indent << "<CellData>" << std::endl;
    indent = increaseIndent(indent);
    out << indent << "<DataArray Name=\"Partition\" type=\"Int32\" NumberOfComponents=\"1\" format=\"binary\">" << std::endl;
    indent = increaseIndent(indent);
    out << indent << this->base64_encode<unsigned int,int32_t>({(unsigned int)this->partition.size()*4}) << this->base64_encode<unsigned int,int32_t>(this->partition) << std::endl;
    indent = decreaseIndent(indent);
    out << indent << "</DataArray>" << std::endl;
    indent = decreaseIndent(indent);
    out << indent << "</CellData>" << std::endl;

    ////////////////////////////////////////////////////////
    // List of Points
    ////////////////////////////////////////////////////////
    out << indent << "<Points>" << std::endl;
    indent = increaseIndent(indent);
    out << indent << "<DataArray type=\"Float32\" "
                             << "NumberOfComponents=\"3\" "
                             << "format=\"binary\">" << std::endl;
    indent = increaseIndent(indent);
    std::vector<float> coords;
    coords.reserve(this->vertices.size() * 3);
    for (const auto v : this->vertices){
        for (const auto p : v->getCoords())
            coords.push_back(p);
        for (unsigned int i = this->dimension; i < 3; i++)
            coords.push_back(0.0);
    }
    out << indent << this->base64_encode<unsigned int,int32_t>({(unsigned int)coords.size()*4}) << this->base64_encode<float,int32_t>(coords) << std::endl;
    indent = decreaseIndent(indent);
    out << indent << "</DataArray>" << std::endl;
    indent = decreaseIndent(indent);
    out << indent << "</Points>" << std::endl;

    ////////////////////////////////////////////////////////
    // List of Cells
    ////////////////////////////////////////////////////////
    out << indent << "<Cells>" << std::endl;
    indent = increaseIndent(indent);
    out << indent << "<DataArray type=\"Int32\" "
                             << "Name=\"connectivity\" "
                             << "format=\"binary\">" << std::endl;
    indent = increaseIndent(indent);
    std::vector<unsigned int> connect;
    connect.reserve(this->elementConnectivity.size() * 2 * this->dimension);
    for (const auto &e : this->elementConnectivity){
        for (const auto &p : e)
            connect.push_back(p);
    }
    out << indent << this->base64_encode<unsigned int,int32_t>({(unsigned int)connect.size()*4}) << this->base64_encode<unsigned int,int32_t>(connect) << std::endl;
    indent = decreaseIndent(indent);
    out << indent << "</DataArray>" << std::endl;

    out << indent << "<DataArray type=\"Int32\" "
                             << "Name=\"offsets\" "
                             << "format=\"binary\">" << std::endl;
    indent = increaseIndent(indent);
    unsigned int offset = pow(2, this->dimension);
    unsigned int index = 0;
    std::vector<unsigned int> offset_vec;
    offset_vec.reserve(this->elementConnectivity.size());
    for (const auto &e : this->elementConnectivity)
        offset_vec.push_back( index += offset );
    out << indent << this->base64_encode<unsigned int,int32_t>({(unsigned int)offset_vec.size()*4}) << this->base64_encode<unsigned int,int32_t>(offset_vec) << std::endl;
    indent = decreaseIndent(indent);
    out << indent << "</DataArray>" << std::endl;
    
    out << indent << "<DataArray type=\"UInt8\" "
                             << "Name=\"types\" "
                             << "format=\"binary\">" << std::endl;
    indent = increaseIndent(indent);
    unsigned int elementType;
    if (this->dimension == 1)
        elementType = 3;
    else if (this->dimension == 2)
        elementType = 9;
    else if (this->dimension == 3)
        elementType = 12;
    
    std::vector<unsigned int> type_vec;
    type_vec.reserve(this->elementConnectivity.size());
    for (const auto &e : this->elementConnectivity){
        type_vec.push_back( elementType );
    }
    out << indent << this->base64_encode<unsigned int,int32_t>({(unsigned int)type_vec.size()}) << this->base64_encode<unsigned int,uint8_t>(type_vec) << std::endl;
    indent = decreaseIndent(indent);
    out << indent << "</DataArray>" << std::endl;
    indent = decreaseIndent(indent);
    out << indent << "</Cells>" << std::endl;

    ////////////////////////////////////////////////////////
    // Footer
    ////////////////////////////////////////////////////////
    indent = decreaseIndent(indent);
    out << indent << "</Piece>" << std::endl;
    indent = decreaseIndent(indent);
    out << indent << "</UnstructuredGrid>" << std::endl;
    indent = decreaseIndent(indent);
    out << indent << "</VTKFile>" << std::endl;
}

void Mesh::print(std::ostream &out) const {
    auto increaseIndent = [](const std::string &input){ return input + "  ";};
    auto decreaseIndent = [](const std::string &input){ unsigned int length = input.size() - 2;
                                                        return std::string(input.begin(), input.begin()+length);};
    ////////////////////////////////////////////////////////
    // Header
    ////////////////////////////////////////////////////////
    std::string indent = "";
    out << indent << "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\">" << std::endl;
    indent = increaseIndent(indent);
    out << indent << "<UnstructuredGrid>" << std::endl;
    indent = increaseIndent(indent);
    unsigned int nVertexPerElement = pow(2, this->dimension);
    out << indent << "<Piece NumberOfPoints=\"" << this->vertices.size() << "\" "
                    << "NumberOfCells=\"" << this->elements.size() << "\">" << std::endl;
    indent = increaseIndent(indent);

    ////////////////////////////////////////////////////////
    // Point Data
    ////////////////////////////////////////////////////////
    out << indent << "<PointData>" << std::endl;
    indent = increaseIndent(indent);

    indent = decreaseIndent(indent);
    out << indent << "</PointData>" << std::endl;

    ////////////////////////////////////////////////////////
    // Cell Data
    ////////////////////////////////////////////////////////
    out << indent << "<CellData>" << std::endl;
    indent = increaseIndent(indent);
    out << indent << "<DataArray Name=\"Partition\" type=\"Int32\" NumberOfComponents=\"1\" format=\"ascii\">" << std::endl;
    indent = increaseIndent(indent);
    for (const auto elem : this->partition)
        out << indent << std::setw(6) << elem << std::endl;
    indent = decreaseIndent(indent);
    out << indent << "</DataArray>" << std::endl;
    indent = decreaseIndent(indent);
    out << indent << "</CellData>" << std::endl;

    ////////////////////////////////////////////////////////
    // List of Points
    ////////////////////////////////////////////////////////
    out << indent << "<Points>" << std::endl;
    indent = increaseIndent(indent);
    out << indent << "<DataArray type=\"Float32\" "
                             << "NumberOfComponents=\"3\" "
                             << "format=\"ascii\">" << std::endl;
    indent = increaseIndent(indent);
    for (const auto v : this->vertices){
        out << indent;
        for (const auto p : v->getCoords())
            out << std::setw(14) << std::scientific << std::setprecision(6) << p;
        // Points must contain 3 components
        for (unsigned int i = this->dimension; i < 3; i++)
            out << std::setw(14) << std::scientific << std::setprecision(6) << 0.0;
        out << std::endl;
    }
    indent = decreaseIndent(indent);
    out << indent << "</DataArray>" << std::endl;
    indent = decreaseIndent(indent);
    out << indent << "</Points>" << std::endl;

    ////////////////////////////////////////////////////////
    // List of Cells
    ////////////////////////////////////////////////////////
    out << indent << "<Cells>" << std::endl;
    indent = increaseIndent(indent);
    out << indent << "<DataArray type=\"Int32\" "
                             << "Name=\"connectivity\">" << std::endl;
    indent = increaseIndent(indent);
    for (const auto &e : this->elementConnectivity){
        out << indent;
        for (const auto &p : e)
            out << std::setw(6) << p;
        out << std::endl;
    }
    indent = decreaseIndent(indent);
    out << indent << "</DataArray>" << std::endl;

    out << indent << "<DataArray type=\"Int32\" "
                             << "Name=\"offsets\">" << std::endl;
    indent = increaseIndent(indent);
    unsigned int offset = pow(2, this->dimension);
    unsigned int index = 0;
    for (const auto &e : this->elementConnectivity){
        out << indent << std::setw(6) << (index += offset) << std::endl;
    }
    indent = decreaseIndent(indent);
    out << indent << "</DataArray>" << std::endl;
    
    out << indent << "<DataArray type=\"UInt8\" "
                             << "Name=\"types\">" << std::endl;
    indent = increaseIndent(indent);
    unsigned int elementType;
    if (this->dimension == 1)
        elementType = 3;
    else if (this->dimension == 2)
        elementType = 9;
    else if (this->dimension == 3)
        elementType = 12;
    for (const auto &e : this->elementConnectivity){
        out << indent << std::setw(6) << elementType << std::endl;
    }
    indent = decreaseIndent(indent);
    out << indent << "</DataArray>" << std::endl;
    indent = decreaseIndent(indent);
    out << indent << "</Cells>" << std::endl;

    ////////////////////////////////////////////////////////
    // Footer
    ////////////////////////////////////////////////////////
    indent = decreaseIndent(indent);
    out << indent << "</Piece>" << std::endl;
    indent = decreaseIndent(indent);
    out << indent << "</UnstructuredGrid>" << std::endl;
    indent = decreaseIndent(indent);
    out << indent << "</VTKFile>" << std::endl;

}


Pair_UI_UI Mesh::getElementAdjacency() const{    
    // First of pair contains indices, Second contains compressed values
    Pair_UI_UI result;
    result.first.reserve(this->elements.size()+1);
    result.second.reserve(this->elements.size() * pow(2, this->dimension-1)); // Conservative reservation

    result.first.push_back(0);
    for (const auto e : this->elements){
        for (const auto n : e->getNeighbors()){
            auto it = std::find(this->elements.begin(), this->elements.end(), n);
            result.second.push_back( int(it - this->elements.begin()) );
        }
        unsigned int nNeighbors = e->numNeighbors();
        result.first.push_back( result.first.back() + nNeighbors );
    }

    // printf("========================\n");
    // printf("Neighbors\n");
    // printf("========================\n");
    // for (unsigned int j = 0; j < result.first.size()-1; j++){
    //     printf("Neighbors[%2d]: ", j );
    //     for (unsigned int i = result.first[j]; i < result.first[j+1]; i++)
    //         printf("%4d", result.second[i]);
    //     printf("\n");
    // }

    return result;
}

void Mesh::partitionMesh(const unsigned int nParts_in){
    using namespace std::chrono;
    auto start = high_resolution_clock::now();
    // Create Element adjacenty (CSR format)
    Pair_UI_UI adj = this->getElementAdjacency();

    // Convert adjacency to METIS compatible
    using Vec_IDX = std::vector<idx_t>;
    using Pair_IDX_IDX = std::pair<Vec_IDX, Vec_IDX>;
    Pair_IDX_IDX adjacency = std::make_pair<Vec_IDX, Vec_IDX>(Vec_IDX(adj.first.size()), Vec_IDX(adj.second.size()));
    for (unsigned int i = 0; i < adj.first.size(); i++)
        adjacency.first[i] = static_cast<idx_t>(adj.first[i]);
    for (unsigned int i = 0; i < adj.second.size(); i++)
        adjacency.second[i] = static_cast<idx_t>(adj.second[i]);

    // METIS Partitioning
    idx_t nElements = static_cast<idx_t>( this->elements.size() );
    idx_t nConstraints = 1;
    idx_t objectiveValue;
    idx_t nParts = static_cast<idx_t>( nParts_in );
    std::vector<idx_t> METISpartition(this->elements.size());
    unsigned int err = METIS_PartGraphKway(&nElements, &nConstraints,
                                                adjacency.first.data(),
                                                adjacency.second.data(),
                                                NULL, NULL, NULL, &nParts, NULL,
                                                NULL, NULL, &objectiveValue,
                                                METISpartition.data());

    // unsigned int index = 0;
    // for (const auto e : METISpartition){
    //     printf("Element %4d,  Owned by: %4d\n", index, METISpartition[index]);
    //     index++;
    // }

    // Save partition
    this->partition.clear();
    for (const auto e : METISpartition)
        this->partition.push_back( static_cast<unsigned int>(e) );

    // Output Elapsed time
    auto duration = duration_cast<seconds>(high_resolution_clock::now() - start);
    printf("Partitioning Elapsed Time: %4ds\n", int(duration.count()));
}

void Mesh::findRingNodes() {
    auto it = std::max_element(this->partition.begin(), this->partition.end());
    unsigned int nPartitions = *it + 1;
    printf("Number of Partitions: %4d\n", nPartitions);

    // Use temporary node indices
    this->ringNodeIndices.resize(nPartitions);
    unsigned int index = 0;
    for (const auto e : this->elements){
        unsigned int thisProcessor = this->partition[index];
        for (const auto n : e->getNeighbors()){
            auto it = std::find(this->elements.begin(), this->elements.end(), n);
            unsigned int neighborIndex = int(it - this->elements.begin());
            unsigned int neighborProcessor = this->partition[neighborIndex];
            if (neighborProcessor != thisProcessor){
                // printf("ThisElement: %4d, Neighbor: %4d\n", index, neighborIndex);
                auto sharedVerts = e->getSharedVertices(*it);
                std::vector<unsigned int> sharedIndices;
                sharedIndices.reserve(sharedVerts.size());
                for (const auto v : sharedVerts){
                    auto nodeIt = std::find(this->vertices.begin(), this->vertices.end(), v);
                    sharedIndices.push_back( int( nodeIt - this->vertices.begin() ) );
                }
                auto nodeEnd = this->ringNodeIndices[thisProcessor].end();
                this->ringNodeIndices[thisProcessor].insert(nodeEnd, sharedIndices.begin(), sharedIndices.end());
            }
        }
        index++;
    }

    // Sort and get unique indices
    index = 0;
    for (auto &proc : this->ringNodeIndices){
        std::sort(proc.begin(), proc.end());
        auto it = std::unique(proc.begin(), proc.end());
        proc.resize( std::distance(proc.begin(), it) );
        index++;
    }
}