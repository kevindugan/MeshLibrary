#ifndef MESH_WRITER_H_C9A0
#define MESH_WRITER_H_C9A0

#include "Mesh.h"
#include <stack>

class MeshWriter {
    public:
        enum vtkFormat {ascii, binary, compressed};

        MeshWriter(const Mesh&);

        void outputVTK(std::ostream &out = std::cout, vtkFormat=MeshWriter::binary);
        void openXMLSection(std::ostream &out,
                            const std::string &title,
                            const std::vector<std::string> &headerInfo ={});
        void closeXMLSection(std::ostream &out);
        
        template<typename T, typename length>
        static std::string vtkDataToBinary(const std::vector<T> &vals);
        template<typename T, typename length>
        static std::string vtkDataToCompressedBinary(const std::vector<T> &vals);
        template<typename T, typename length>
        static std::string getBitStream(const std::vector<T> &vals);
        static std::string base64_encode(const std::string &bit_string);

    private:
        const Mesh& referenceMesh;

        std::string indent;
        std::stack<std::string> xml_headers;

};

#include "MeshWriter.hpp"

#endif // MESH_WRITER_H_C9A0