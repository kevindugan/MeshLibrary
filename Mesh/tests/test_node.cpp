#include "Node.h"
#include "gmock/gmock.h"


TEST(MeshLib_Node, constructor){
    Node node1(2, {1.2, 3.2});
    
    EXPECT_DEATH(Node node2(3, {1.1});, "");
    EXPECT_DEATH(Node node2(2, {1.1, 2.1, 3.4});, "");
    EXPECT_DEATH(Node node2(4, {1.2, 3.2, 4.1, 2.4});, "");
}

TEST(MeshLib_Node, get_coords){
    Node node1(2, {1.2, 2.3});

    EXPECT_FLOAT_EQ(node1.getCoords()[0], 1.2);
    EXPECT_FLOAT_EQ(node1.getCoords()[1], 2.3);
}

TEST(MeshLib_Node, get_dim){
    Node node1(2, {1.2, 4.3});
    Node node2(3, {4.3, 2.1, 3.4});
    Node node3(1, {4.3});

    EXPECT_EQ(node1.getDimension(), 2);
    EXPECT_EQ(node2.getDimension(), 3);
    EXPECT_EQ(node3.getDimension(), 1);
}

TEST(MeshLib_Node, equality_check){
    Node node1(3, {1.3, 3.2, -4.1});
    Node node2(3, {1.3, 3.2, -4.1});

    Node node3(3, {1.3, 3.2, 4.1});
    Node node4(2, {3.2, 2.1});

    EXPECT_TRUE(node1 == node2);
    EXPECT_FALSE(node2 == node3);
    EXPECT_DEATH(node1 == node4;, "");
}

TEST(MeshLib_Node, lt_comparison){
    Node node1(1, {2.3});
    Node node2(1, {2.4});
    EXPECT_TRUE( node1 < node2 );
    EXPECT_FALSE( node2 < node1 );

    Node node3(2, {3.4, 2.1});
    Node node4(2, {3.5, 2.2});
    Node ndoe5(2, {3.2, 2.4});
    EXPECT_TRUE( node3 < node4 );
    EXPECT_FALSE( node4 < node3 );
}

TEST(MeshLib_Node, is_at){
    Node node1(3, {1.3, 3.2, -4.1});
    Node node2(3, {1.3, 3.2, -4.1});

    Node node4(2, {3.2, 2.1});

    EXPECT_TRUE(node1.isAt( {1.3, 3.2, -4.1} ));
    EXPECT_TRUE(node2.isAt( {1.3, 3.2, -4.1} ));
    EXPECT_FALSE(node1.isAt( {1.3, 3.2, 4.1} ));
    EXPECT_DEATH(node1.isAt( {3.2, 2.1} ), "");
}