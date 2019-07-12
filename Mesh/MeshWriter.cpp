#include "MeshWriter.h"

MeshWriter::MeshWriter(const Mesh& inMesh) : referenceMesh(inMesh){}

void MeshWriter::outputVTK(std::ostream &out){
    const auto& vertices = referenceMesh.getVertices();
    const auto& elements = referenceMesh.getElements();
    const auto& connectivity = referenceMesh.getConnectivity();
    const auto& partition = referenceMesh.getPartition();

    // Header
    this->openXMLSection(out, "VTKFile", {"type=\"UnstructuredGrid\"",
                                          "version=\"0.1\"",
                                          "byte_order=\"BigEndian\""});
    this->openXMLSection(out, "UnstructuredGrid");
    std::string nPoints = "NumberOfPoints=\"" + std::to_string(vertices.size()) + "\"";
    std::string nCells = "NumberOfCells=\"" + std::to_string(elements.size()) + "\"";
    this->openXMLSection(out, "Piece", {nPoints, nCells});

    // Point Data
    this->openXMLSection(out, "PointData");
    this->closeXMLSection(out);

    // Cell Data
    this->openXMLSection(out, "CellData");
    this->openXMLSection(out, "DataArray", {"Name=\"Partition\"",
                                            "type=\"Int32\"",
                                            "NumberOfComponents=\"1\"",
                                            "format=\"binary\""});
    {
        out << this->indent
            << base64_encode<unsigned int, int32_t>({(unsigned int)partition.size()*4})
            << base64_encode<unsigned int, int32_t>(partition)
            << std::endl;
    }
    this->closeXMLSection(out);
    this->closeXMLSection(out);

    // List of Spatial Coordinates
    this->openXMLSection(out, "Points");
    this->openXMLSection(out, "DataArray", {"type=\"Float32\"",
                                            "NumberOfComponents=\"3\"",
                                            "format=\"binary\""});
    {
        std::vector<float> coords;
        coords.reserve(vertices.size() * 3);
        for (const auto v : vertices){
            for (const auto p : v->getCoords())
                coords.push_back(p);
            for (unsigned int i = referenceMesh.getDimension(); i < 3; i++)
                coords.push_back(0.0);
        }
        out << this->indent
            << base64_encode<unsigned int, int32_t>({(unsigned int)coords.size()*4})
            << base64_encode<float, int32_t>(coords)
            << std::endl;
    }
    this->closeXMLSection(out);
    this->closeXMLSection(out);

    // List of Cells
    this->openXMLSection(out, "Cells");
    this->openXMLSection(out, "DataArray", {"Name=\"connectivity\"",
                                            "type=\"Int32\"",
                                            "format=\"binary\""});
    {
        std::vector<unsigned int> connect;
        connect.reserve( connectivity.size() * 2 * referenceMesh.getDimension() );
        for (const auto &e : connectivity)
            for (const auto &p : e)
                connect.push_back(p);
        out << this->indent
            << base64_encode<unsigned int, int32_t>({(unsigned int)connect.size()*4})
            << base64_encode<unsigned int, int32_t>(connect)
            << std::endl;
    }
    this->closeXMLSection(out);
    this->openXMLSection(out, "DataArray", {"Name=\"offsets\"",
                                            "type=\"Int32\"",
                                            "format=\"binary\""});
    {
        unsigned int offset_size = pow(2, referenceMesh.getDimension());
        unsigned int index = 0;
        std::vector<unsigned int> offset;
        offset.reserve(connectivity.size());
        for (const auto &e : connectivity)
            offset.push_back( index += offset_size );
        out << this->indent
            << base64_encode<unsigned int, int32_t>({(unsigned int)offset.size()*4})
            << base64_encode<unsigned int, int32_t>(offset)
            << std::endl;
    }
    this->closeXMLSection(out);
    this->openXMLSection(out, "DataArray", {"Name=\"types\"",
                                            "type=\"UInt8\"",
                                            "format=\"binary\""});
    {
        std::vector<unsigned int> vtkElementTypes = {3, 9, 12};
        unsigned int elementType = vtkElementTypes[referenceMesh.getDimension() - 1];

        std::vector<unsigned int> types;
        types.reserve(connectivity.size());
        for (const auto &e : connectivity)
            types.push_back( elementType );

        out << this->indent
            << base64_encode<unsigned int, int32_t>({(unsigned int)types.size()})
            << base64_encode<unsigned int, uint8_t>(types)
            << std::endl;
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