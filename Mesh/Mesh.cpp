#include "Mesh.h"

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

    // Get delta for each direction
    std::vector<float> delta;
    for (unsigned int i = 0; i < this->dimension; i++)
        delta.push_back( (maxVerts[i] - minVerts[i]) / float(nCells[i]) );

    // Setup Cartesian vertices
    unsigned int nVertices = 1;
    for (const auto d : nCells)
        nVertices *= (d+1);
    std::vector<std::vector<float>> vertices(nVertices, std::vector<float>(this->dimension, 0.0));
    for (unsigned int dim = 0; dim < this->dimension; dim++){
        unsigned int repeat = 1;
        for (unsigned int d = 0; d < dim; d++)
            repeat *= (nCells[d] + 1);

        unsigned int index = 0;
        while (index < vertices.size()){
            for (unsigned int point = 0; point <= nCells[dim]; point++){
                float value = point * delta[dim];
                for (unsigned int r = 0; r < repeat; r++){
                    vertices[index][dim] = value;
                    index++;
                }
            }
        }
    }

    // Create Node List
    this->vertices.reserve(vertices.size());
    for (const auto v : vertices)
        this->vertices.push_back( std::make_shared<Node>(Node(this->dimension, v)) );

    // for (const auto &v : vertices)
    //     if (v.size() == 2)
    //         printf("(%4.1f, %4.1f)\n", v[0], v[1]);
    //     else if (v.size() == 3)
    //         printf("(%4.1f, %4.1f, %4.1f)\n", v[0], v[1], v[2]);

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

    this->elements.reserve(nElements);
    for (const auto e : this->elementConnectivity){
        std::vector<std::shared_ptr<Node>> nodeList;
        for (const auto index : e)
            nodeList.push_back(this->vertices[index]);
        this->elements.push_back( std::make_shared<Element>(Element(this->dimension, nodeList)) );
    }

    // for (const auto &v : this->elementConnectivity)
    //     if (v.size() == 4)
    //         printf("[%4d, %4d, %4d, %4d]\n", v[0], v[1], v[2], v[3]);
    //     else if (v.size() == 8)
    //         printf("[%4d, %4d, %4d, %4d, %4d, %4d, %4d, %4d]\n", v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7]);

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
    for (const auto elem : this->elements)
        out << indent << std::setw(6) << this->owningProcessor << std::endl;
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