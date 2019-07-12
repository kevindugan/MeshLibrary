#include "MeshWriter.h"

MeshWriter::MeshWriter(const Mesh& inMesh) : referenceMesh(inMesh){}

void MeshWriter::outputVTK(std::ostream &out, vtkFormat outputFormat){
    const auto& vertices = referenceMesh.getVertices();
    const auto& elements = referenceMesh.getElements();
    const auto& connectivity = referenceMesh.getConnectivity();
    const auto& partition = referenceMesh.getPartition();

    // Header
    std::vector<std::string> headerOptions = {"type=\"UnstructuredGrid\"", "version=\"0.1\""};
    if (outputFormat == MeshWriter::binary)
        headerOptions.push_back("byte_order=\"BigEndian\"");
    this->openXMLSection(out, "VTKFile", headerOptions);
    this->openXMLSection(out, "UnstructuredGrid");
    std::string nPoints = "NumberOfPoints=\"" + std::to_string(vertices.size()) + "\"";
    std::string nCells = "NumberOfCells=\"" + std::to_string(elements.size()) + "\"";
    this->openXMLSection(out, "Piece", {nPoints, nCells});

    // Point Data
    this->openXMLSection(out, "PointData");
    this->closeXMLSection(out);

    // Cell Data
    this->openXMLSection(out, "CellData");
    std::vector<std::string> cellDataOptions = {"Name=\"Partition\"", "type=\"Int32\"", "NumberOfComponents=\"1\""};
    if (outputFormat == MeshWriter::ascii)
        cellDataOptions.push_back("format=\"ascii\"");
    else if (outputFormat == MeshWriter::binary)
        cellDataOptions.push_back("format=\"binary\"");
    this->openXMLSection(out, "DataArray", cellDataOptions);
    {
        if (outputFormat == MeshWriter::ascii){
            for (const auto elem : partition)
                out << this->indent << std::setw(6) << elem << std::endl;
        } else if (outputFormat == MeshWriter::binary){
            out << this->indent
                << base64_encode<unsigned int, int32_t>({(unsigned int)partition.size()*4})
                << base64_encode<unsigned int, int32_t>(partition)
                << std::endl;
        }
    }
    this->closeXMLSection(out);
    this->closeXMLSection(out);

    // List of Spatial Coordinates
    this->openXMLSection(out, "Points");
    std::vector<std::string> pointsOptions = {"type=\"Float32\"", "NumberOfComponents=\"3\""};
    if (outputFormat == MeshWriter::ascii)
        pointsOptions.push_back("format=\"ascii\"");
    else if (outputFormat == MeshWriter::binary)
        pointsOptions.push_back("format=\"binary\"");
    this->openXMLSection(out, "DataArray", pointsOptions);
    {
        std::vector<float> coords;
        coords.reserve(vertices.size() * 3);
        for (const auto v : vertices){
            for (const auto p : v->getCoords())
                coords.push_back(p);
            for (unsigned int i = referenceMesh.getDimension(); i < 3; i++)
                coords.push_back(0.0);
        }
        if (outputFormat == MeshWriter::ascii){
            for (unsigned int i = 0; i < coords.size(); i+=3)
                out << this->indent
                    << std::setw(14) << std::scientific << std::setprecision(6) << coords[i+0] 
                    << std::setw(14) << std::scientific << std::setprecision(6) << coords[i+1] 
                    << std::setw(14) << std::scientific << std::setprecision(6) << coords[i+2]
                    << std::endl;
        } else if (outputFormat == MeshWriter::binary){
            out << this->indent
                << base64_encode<unsigned int, int32_t>({(unsigned int)coords.size()*4})
                << base64_encode<float, int32_t>(coords)
                << std::endl;
        }
    }
    this->closeXMLSection(out);
    this->closeXMLSection(out);

    // List of Cells
    this->openXMLSection(out, "Cells");
    std::vector<std::string> cellsOptions = {"Name=\"connectivity\"", "type=\"Int32\""};
    if (outputFormat == MeshWriter::ascii)
        cellsOptions.push_back("format=\"ascii\"");
    else if (outputFormat == MeshWriter::binary)
        cellsOptions.push_back("format=\"binary\"");
    this->openXMLSection(out, "DataArray", cellsOptions);
    {
        std::vector<unsigned int> connect;
        unsigned int nNeighbors = pow(2, referenceMesh.getDimension());
        connect.reserve( connectivity.size() * nNeighbors );
        for (const auto &e : connectivity)
            for (const auto &p : e)
                connect.push_back(p);
        if (outputFormat == MeshWriter::ascii){
            for (unsigned int elem = 0; elem < connect.size(); elem+=nNeighbors){
                out << this->indent;
                for (unsigned int n = 0; n < nNeighbors; n++)
                    out << std::setw(6) << connect[elem+n];
                out << std::endl;
            }
        } else if (outputFormat == MeshWriter::binary){
            out << this->indent
                << base64_encode<unsigned int, int32_t>({(unsigned int)connect.size()*4})
                << base64_encode<unsigned int, int32_t>(connect)
                << std::endl;
        }
    }
    this->closeXMLSection(out);
    std::vector<std::string> offsetOptions = {"Name=\"offsets\"", "type=\"Int32\""};
    if (outputFormat == MeshWriter::ascii)
        offsetOptions.push_back("format=\"ascii\"");
    else if (outputFormat == MeshWriter::binary)
        offsetOptions.push_back("format=\"binary\"");
    this->openXMLSection(out, "DataArray", offsetOptions);
    {
        unsigned int offset_size = pow(2, referenceMesh.getDimension());
        unsigned int index = 0;
        std::vector<unsigned int> offset;
        offset.reserve(connectivity.size());
        for (const auto &e : connectivity)
            offset.push_back( index += offset_size );
        if (outputFormat == MeshWriter::ascii){
            for (const auto elem : offset)
                out << this->indent
                    << std::setw(6) << elem
                    << std::endl;
        } else if (outputFormat == MeshWriter::binary){
            out << this->indent
                << base64_encode<unsigned int, int32_t>({(unsigned int)offset.size()*4})
                << base64_encode<unsigned int, int32_t>(offset)
                << std::endl;
        }
    }
    this->closeXMLSection(out);
    std::vector<std::string> typesOptions = {"Name=\"types\"", "type=\"UInt8\""};
    if (outputFormat == MeshWriter::ascii)
        typesOptions.push_back("format=\"ascii\"");
    else if (outputFormat == MeshWriter::binary)
        typesOptions.push_back("format=\"binary\"");
    this->openXMLSection(out, "DataArray", typesOptions);
    {
        std::vector<unsigned int> vtkElementTypes = {3, 9, 12};
        unsigned int elementType = vtkElementTypes[referenceMesh.getDimension() - 1];

        std::vector<unsigned int> types;
        types.reserve(connectivity.size());
        for (const auto &e : connectivity)
            types.push_back( elementType );

        if (outputFormat == MeshWriter::ascii){
            for (const auto elem : types)
                out << this->indent
                    << std::setw(6) << elem
                    << std::endl;
        } else if (outputFormat == MeshWriter::binary){
            out << this->indent
                << base64_encode<unsigned int, int32_t>({(unsigned int)types.size()})
                << base64_encode<unsigned int, uint8_t>(types)
                << std::endl;
        }
    }
    this->closeXMLSection(out);
    this->closeXMLSection(out);



    // If XML Sections remain open, close
    // them here
    while (!this->xml_headers.empty())
        this->closeXMLSection(out);
}

void MeshWriter::openXMLSection(std::ostream &out,
                                const std::string &title,
                                const std::vector<std::string> &headerInfo){
    this->xml_headers.push(title);
    out << indent << "<" << title;

    for (const auto item : headerInfo)
        out << " " << item;

    out << ">" << std::endl;

    this->indent += "  ";
}

void MeshWriter::closeXMLSection(std::ostream &out){
    std::string title = this->xml_headers.top();
    this->xml_headers.pop();

    // Unindent two spaces
    this->indent.pop_back();
    this->indent.pop_back();

    out << indent << "</" << title << ">" << std::endl;
}