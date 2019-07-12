#include "Mesh.h"
#include "gmock/gmock.h"
#include <sstream>
#include <fstream>

#ifdef DEBUG_MODE
#  define DebugTestName(testName) testName
#else
#  define DebugTestName(testName) DISABLED_ ## testName
#endif

using ::testing::ElementsAreArray;

TEST(DebugTestName(MeshLib_Mesh), constructor){
    Mesh cart({0.0, 0.0},
              {1.0, 2.0},
              {3, 3});

    Mesh cart2({0.0, 0.0, 0.0},
               {1.0, 2.0, 3.0},
               {5, 3, 2});

    // Test non-conforming dimensions
    EXPECT_DEATH(Mesh mesh({0.0, 0.1}, {1.1, 2.2}, {1, 2, 3}), "");
    EXPECT_DEATH(Mesh mesh({0.0, 0.1, 0.0}, {1.1, 2.2}, {1, 2, 3}), "");
    // Out of range dimension
    EXPECT_DEATH(Mesh mesh({0.0}, {0.1}, {}), "");
    EXPECT_DEATH(Mesh mesh({0.0}, {0.1}, {1, 2, 3, 4}), "");
    // min greater than max
    EXPECT_DEATH(Mesh mesh({0.0, 0.0, 0.1}, {0.1, 0.1, 0.0}, {1, 1, 2}), "");
    EXPECT_DEATH(Mesh mesh({0.0, 0.1, 0.1}, {0.1, 0.1, 0.0}, {1, 1, 2}), "");
}

TEST(MeshLib_Mesh, Encode_base_64){
    std::vector<unsigned int> vec_int = {8, 16, 24, 32, 40, 48, 56, 64};

    std::string expected = "AAAACAAAABAAAAAYAAAAIAAAACgAAAAwAAAAOAAAAEA";
    std::string b_64_val = Mesh::base64_encode<unsigned int,int32_t>(vec_int);
    EXPECT_EQ(b_64_val.size(), expected.size());
    EXPECT_STREQ(b_64_val.c_str(), expected.c_str());
}

TEST(MeshLib_Mesh, print){
    Mesh cart({0.0, 0.0},
              {1.0, 2.0},
              {3, 3});

    Mesh cart2({0.0, 0.0, 0.0},
               {1.0, 2.0, 3.0},
               {5, 3, 2});

    std::stringstream output1;
    cart.print(output1);

    std::stringstream output2;
    cart2.print(output2);

    EXPECT_GT(output1.str().size(), 0);
    EXPECT_GT(output2.str().size(), 0);
    EXPECT_GT(output2.str().size(), output1.str().size());
}

TEST(MeshLib_Mesh, adjacency){

    Mesh cart({0.0, 0.0},
              {1.0, 2.0},
              {4, 3});

    Mesh cart2({0.0, 0.0, 0.0},
               {1.0, 2.0, 3.0},
               {5, 3, 4});
    
    Pair_UI_UI adj1 = cart.getElementAdjacency();
    Pair_UI_UI adj2 = cart2.getElementAdjacency();

    std::vector<unsigned int> expected1 = {1, 4, 
                                           0, 2, 5,
                                           1, 3, 6,
                                           2, 7,
                                           0, 5, 8,
                                           1, 4, 6, 9,
                                           2, 5, 7, 10,
                                           3, 6, 11,
                                           4, 9,
                                           5, 8, 10,
                                           6, 9, 11,
                                           7, 10};
    std::vector<unsigned int> expectedIndex1 = {0, 2, 5, 8, 10, 13, 17, 21, 24, 26, 29, 32, 34};

    std::vector<unsigned int> expectedIndex2 = {  0,   3,   7,  11,  15,  18,  22,  27,  32,  37,  41,  44,  48,  52,  56,  59,
                                                      63,  68,  73,  78,  82,  87,  93,  99, 105, 110, 114, 119, 124, 129, 133,
                                                     137, 142, 147, 152, 156, 161, 167, 173, 179, 184, 188, 193, 198, 203, 207,
                                                     210, 214, 218, 222, 225, 229, 234, 239, 244, 248, 251, 255, 259, 263, 266};

    std::vector<unsigned int> expected2 = { 1,  5, 15,
                                            0,  2,  6, 16,
                                            1,  3,  7, 17,
                                            2,  4,  8, 18,
                                            3,  9, 19,
                                            0,  6, 10, 20,
                                            1,  5,  7, 11, 21,
                                            2,  6,  8, 12, 22,
                                            3,  7,  9, 13, 23,
                                            4,  8, 14, 24,
                                            5, 11, 25,
                                            6, 10, 12, 26,
                                            7, 11, 13, 27,
                                            8, 12, 14, 28,
                                            9, 13, 29,
                                           
                                            0, 16, 20, 30,
                                            1, 15, 17, 21, 31,
                                            2, 16, 18, 22, 32,
                                            3, 17, 19, 23, 33,
                                            4, 18, 24, 34,
                                            5, 15, 21, 25, 35,
                                            6, 16, 20, 22, 26, 36,
                                            7, 17, 21, 23, 27, 37,
                                            8, 18, 22, 24, 28, 38,
                                            9, 19, 23, 29, 39,
                                           10, 20, 26, 40,
                                           11, 21, 25, 27, 41,
                                           12, 22, 26, 28, 42,
                                           13, 23, 27, 29, 43,
                                           14, 24, 28, 44,
                                           
                                           15, 31, 35, 45,
                                           16, 30, 32, 36, 46,
                                           17, 31, 33, 37, 47,
                                           18, 32, 34, 38, 48,
                                           19, 33, 39, 49,
                                           20, 30, 36, 40, 50,
                                           21, 31, 35, 37, 41, 51,
                                           22, 32, 36, 38, 42, 52,
                                           23, 33, 37, 39, 43, 53,
                                           24, 34, 38, 44, 54,
                                           25, 35, 41, 55,
                                           26, 36, 40, 42, 56,
                                           27, 37, 41, 43, 57,
                                           28, 38, 42, 44, 58,
                                           29, 39, 43, 59,
                                           
                                           30, 46, 50,
                                           31, 45, 47, 51,
                                           32, 46, 48, 52,
                                           33, 47, 49, 53,
                                           34, 48, 54,
                                           35, 45, 51, 55,
                                           36, 46, 50, 52, 56,
                                           37, 47, 51, 53, 57,
                                           38, 48, 52, 54, 58,
                                           39, 49, 53, 59,
                                           40, 50, 56,
                                           41, 51, 55, 57,
                                           42, 52, 56, 58,
                                           43, 53, 57, 59,
                                           44, 54, 58
                                           };

    ASSERT_EQ(expectedIndex1.size(), adj1.first.size());
    ASSERT_EQ(expected1.size(), adj1.second.size());
    ASSERT_EQ(expectedIndex2.size(), adj2.first.size());
    ASSERT_EQ(expected2.size(), adj2.second.size());

    EXPECT_THAT(adj1.first, ElementsAreArray(expectedIndex1));
    EXPECT_THAT(adj1.second, ElementsAreArray(expected1));
    EXPECT_THAT(adj2.first, ElementsAreArray(expectedIndex2));
    EXPECT_THAT(adj2.second, ElementsAreArray(expected2));
}

TEST(MeshLib_Mesh, partition){

    using namespace std::chrono;
    auto start = high_resolution_clock::now();
    Mesh cart({0.0, 0.0},
              {1.0, 1.0},
              {6, 4});
    auto duration = duration_cast<seconds>(high_resolution_clock::now() - start);
    printf("2D Init Elapsed Time: %4ds\n", int(duration.count()));

    cart.partitionMesh(8);
    std::ofstream output2d("output2d.vtu", std::ofstream::out);
    cart.print(output2d);
    output2d.close();

    start = high_resolution_clock::now();
    Mesh cart2({0.0, 0.0, 0.0},
               {1.0, 2.0, 3.0},
               {5, 4, 3});
    duration = duration_cast<seconds>(high_resolution_clock::now() - start);
    printf("3D Init Elapsed Time: %4ds\n", int(duration.count()));

    cart2.partitionMesh(8);
    std::ofstream output3d("output3d.vtu", std::ofstream::out);
    cart2.print(output3d);
    output3d.close();
}

TEST(MeshLib_Mesh, get_ring_nodes2d){
    Mesh cart({0.0, 0.0},
              {1.0, 1.0},
              {11, 8});

    // cart.partitionMesh(5);
    // printf("{");
    // for (const auto e : cart.getPartition()){
    //     printf("%2d,", e);
    // }
    // printf("};\n");

    // std::ofstream out("outputRind.vtu", std::ofstream::out);
    // cart.print(out);
    // out.close();

    std::vector<unsigned int> partition = { 1, 1, 1, 1, 2, 2, 2, 4, 4, 4, 4,
                                              1, 1, 1, 1, 2, 2, 2, 4, 4, 4, 4,
                                              1, 1, 1, 1, 2, 2, 2, 4, 4, 4, 4,
                                              1, 1, 1, 1, 2, 2, 2, 4, 4, 4, 4,
                                              1, 1, 0, 0, 2, 2, 2, 3, 3, 4, 4,
                                              0, 0, 0, 0, 0, 2, 2, 3, 3, 3, 3,
                                              0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3,
                                              0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3};

    std::vector<std::vector<unsigned int>> ExpectedRings = 
                                           {{50, 51, 52, 60, 61, 62, 64, 65, 77, 89, 101},
                                            {4, 16, 28, 40, 50, 51, 52, 60, 61, 62},
                                            {4, 7, 16, 19, 28, 31, 40, 43, 52, 55, 64, 65, 67, 77, 78, 79},
                                            {55, 56, 57, 67, 69, 70, 71, 77, 78, 79, 89, 101},
                                            {7, 19, 31, 43, 55, 56, 57, 69, 70, 71}};

    cart.setPartition(partition);

    cart.findRingNodes();

    unsigned int index = 0;
    for (const auto partitionRing : ExpectedRings){
        EXPECT_THAT(cart.getRingNodes(index), partitionRing);
        index++;
    }
    
}

TEST(MeshLib_Mesh, get_ring_nodes3d){
    Mesh cart({0.0, 0.0, 0.0},
              {1.0, 1.0, 1.0},
              {11, 8, 3});

    // cart.partitionMesh(7);
    // printf("{");
    // for (const auto e : cart.getPartition()){
    //     printf("%2d,", e);
    // }
    // printf("};\n");

    // std::ofstream out("outputRind.vtu", std::ofstream::out);
    // cart.print(out);
    // out.close();

    std::vector<unsigned int> partition = { 4, 4, 4, 4, 3, 3, 3, 0, 0, 0, 0,
                                            4, 4, 4, 3, 3, 3, 0, 0, 0, 0, 0,
                                            4, 4, 4, 3, 3, 3, 2, 2, 0, 0, 0,
                                            4, 4, 4, 3, 3, 3, 2, 2, 2, 2, 2,
                                            6, 6, 6, 5, 5, 3, 1, 1, 2, 2, 2,
                                            6, 6, 6, 5, 5, 5, 1, 1, 1, 1, 2,
                                            6, 6, 6, 5, 5, 5, 5, 1, 1, 1, 1,
                                            6, 6, 6, 5, 5, 5, 5, 1, 1, 1, 1,

                                            4, 4, 4, 3, 3, 3, 0, 0, 0, 0, 0,
                                            4, 4, 4, 3, 3, 3, 0, 0, 0, 0, 0,
                                            4, 4, 4, 3, 3, 3, 2, 2, 0, 0, 0,
                                            4, 4, 4, 4, 3, 3, 2, 2, 2, 2, 2,
                                            6, 6, 6, 5, 2, 2, 1, 1, 2, 2, 2,
                                            6, 6, 6, 5, 5, 5, 1, 1, 1, 1, 2,
                                            6, 6, 6, 5, 5, 5, 5, 1, 1, 1, 1,
                                            6, 6, 6, 5, 5, 5, 5, 1, 1, 1, 1,

                                            4, 4, 4, 3, 3, 3, 0, 0, 0, 0, 0,
                                            4, 4, 4, 3, 3, 3, 0, 0, 0, 0, 0,
                                            4, 4, 4, 3, 3, 3, 2, 2, 0, 0, 0,
                                            4, 4, 4, 3, 3, 3, 2, 2, 2, 2, 2,
                                            6, 6, 6, 5, 3, 3, 2, 1, 2, 2, 2,
                                            6, 6, 6, 5, 5, 5, 5, 1, 2, 2, 2,
                                            6, 6, 6, 5, 5, 5, 5, 1, 1, 1, 1,
                                            6, 6, 6, 5, 5, 5, 5, 1, 1, 1, 1};

    std::vector<std::vector<unsigned int>> ExpectedRings = 
                                           {{7, 18, 19, 30, 31, 32, 44, 45, 46, 47,
                                             114, 115, 126, 127, 138, 139, 140, 152, 153, 154, 155,
                                             222, 234, 246, 247, 248, 260, 261, 262, 263,
                                             330, 342, 354, 355, 356, 368, 369, 370, 371},
                                            {54, 55, 56, 66, 68, 69, 70, 78, 79, 82, 83, 91, 103,
                                             162, 163, 164, 174, 176, 177, 178, 186, 187, 190, 191, 199, 211,
                                             270, 271, 272, 282, 283, 284, 285, 286, 294, 295, 296, 297, 298, 299, 307, 319,
                                             379, 380, 391, 392, 403, 404, 405, 406, 407, 415, 427},
                                            {30, 31, 32, 42, 44, 45, 46, 47, 54, 55, 56, 68, 69, 70, 82, 83,
                                             138, 139, 140, 150, 152, 153, 154, 155, 160, 161, 162, 163, 164, 172, 173, 174, 176, 177, 178, 190, 191,
                                             246, 247, 248, 258, 260, 261, 262, 263, 268, 269, 270, 271, 272, 280, 281, 282, 283, 284, 285, 286, 296, 297, 298, 299,
                                             354, 355, 356, 366, 368, 369, 370, 371, 378, 379, 380, 390, 391, 392, 404, 405, 406, 407},
                                            {4, 7, 15, 16, 18, 19, 27, 30, 39, 42, 51, 52, 53, 54, 65, 66,
                                             111, 112, 114, 115, 123, 124, 126, 127, 135, 138, 147, 148, 150, 159, 160, 161, 162, 173, 174,
                                             219, 222, 231, 234, 243, 246, 255, 256, 258, 267, 268, 269, 270, 280, 281, 282,
                                             327, 330, 339, 342, 351, 354, 363, 366, 375, 376, 378, 388, 389, 390},
                                            {4, 15, 16, 27, 39, 48, 49, 50, 51,
                                             111, 112, 123, 124, 135, 147, 148, 156, 157, 158, 159, 160,
                                             219, 231, 243, 255, 256, 264, 265, 266, 267, 268,
                                             327, 339, 351, 363, 372, 373, 374, 375},
                                            {51, 52, 53, 63, 65, 66, 75, 78, 79, 87, 91, 99, 103,
                                             159, 160, 161, 171, 172, 173, 174, 183, 186, 187, 195, 199, 207, 211,
                                             267, 268, 279, 280, 281, 282, 283, 291, 294, 295, 303, 307, 315, 319,
                                             375, 376, 387, 388, 389, 390, 391, 399, 403, 411, 415, 423, 427},
                                            {48, 49, 50, 51, 63, 75, 87, 99,
                                             156, 157, 158, 159, 171, 183, 195, 207,
                                             264, 265, 266, 267, 279, 291, 303, 315,
                                             372, 373, 374, 375, 387, 399, 411, 423}};

    cart.setPartition(partition);

    cart.findRingNodes();

    unsigned int index = 0;
    for (const auto partitionRing : ExpectedRings){
        for (unsigned int j = 0; j < partitionRing.size(); j++)
            if (cart.getRingNodes(index)[j] != partitionRing[j])
                printf("Proc %4d: Expected: %4d, Result: %4d\n", index, partitionRing[j], cart.getRingNodes(index)[j]);
        EXPECT_THAT(cart.getRingNodes(index), partitionRing);
        index++;
    }
    
}