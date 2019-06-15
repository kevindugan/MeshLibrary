#include "Mesh.h"
#include "gmock/gmock.h"
#include <sstream>

TEST(MeshLib_Mesh, constructor){
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

TEST(MeshLib_Mesh, partition){
    
}