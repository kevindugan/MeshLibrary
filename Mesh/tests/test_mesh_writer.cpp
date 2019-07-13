#include "MeshWriter.h"
#include "gmock/gmock.h"
#include <sstream>

using ::testing::ElementsAreArray;

TEST(MeshLib_Writer, Init){
    Mesh cart({0.0, 0.0}, {1.0, 1.0}, {2,3});
    MeshWriter writer(cart);

    std::stringstream output;
    writer.outputVTK(output);

    std::string expected;
    expected += "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"BigEndian\">\n";
    expected += "</VTKFile>\n";

    std::cout << output.str().c_str();
}

TEST(MeshLib_Writer, xml_sections){
    Mesh cart({0.0, 0.0}, {1.0, 1.0}, {2,3});
    MeshWriter writer(cart);

    std::stringstream output;

    writer.openXMLSection(output, "Title1");
    EXPECT_STREQ("<Title1>\n", output.str().c_str());
    writer.closeXMLSection(output);
    EXPECT_STREQ("<Title1>\n</Title1>\n", output.str().c_str());

    // Clean the output
    output.str(std::string());
    writer.openXMLSection(output, "Title1");
    writer.openXMLSection(output, "Title2", {"option1", "option2"});
    EXPECT_STREQ("<Title1>\n  <Title2 option1 option2>\n", output.str().c_str());
    writer.closeXMLSection(output);
    writer.closeXMLSection(output);
    EXPECT_STREQ("<Title1>\n  <Title2 option1 option2>\n  </Title2>\n</Title1>\n", output.str().c_str());

}

TEST(MeshLib_Writer, convert_to_bitstream){
    std::vector<unsigned int> vec_int = {8, 16, 24, 32, 40, 48, 56, 64};

    std::vector<uint8_t> expected = {0, 0, 0, 8, 0, 0, 0, 16, 0, 0, 0, 24, 0, 0, 0, 32, 0, 0, 0, 40, 0, 0, 0, 48, 0, 0, 0, 56, 0, 0, 0, 64};
    std::vector<uint8_t> bitStream = MeshWriter::getBitVector<unsigned int,int32_t>(vec_int);
    EXPECT_EQ(bitStream.size(), expected.size());
    EXPECT_THAT(bitStream, ElementsAreArray(expected));
}

TEST(MeshLib_Writer, base64_encode){
    std::vector<uint8_t> byteStream = {0, 0, 0, 8, 0, 0, 0, 16, 0, 0, 0, 24, 0, 0, 0, 32, 0, 0, 0, 40, 0, 0, 0, 48, 0, 0, 0, 56, 0, 0, 0, 64};
    std::string expected = "AAAACAAAABAAAAAYAAAAIAAAACgAAAAwAAAAOAAAAEA";
    std::string b_64_val = MeshWriter::base64_encode(byteStream);
    EXPECT_EQ(b_64_val.size(), expected.size());
    EXPECT_STREQ(b_64_val.c_str(), expected.c_str());
}