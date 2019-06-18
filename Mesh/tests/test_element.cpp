#include "Element.h"
#include "gmock/gmock.h"

#ifdef DEBUG_MODE
#  define DebugTestName(testName) testName
#else
#  define DebugTestName(testName) DISABLED_ ## testName
#endif

using ::testing::ElementsAreArray;

TEST(DebugTestName(MeshLib_Element), constructor){

    std::vector<std::shared_ptr<Node>> vertices1;
    vertices1.push_back( std::make_shared<Node>(Node(2, {0.0, 0.0})) );
    vertices1.push_back( std::make_shared<Node>(Node(2, {0.1, 0.0})) );
    vertices1.push_back( std::make_shared<Node>(Node(2, {0.1, 0.1})) );
    vertices1.push_back( std::make_shared<Node>(Node(2, {0.0, 0.1})) );

    Element elem1(2, vertices1);
    EXPECT_DEATH(Element elem(3, vertices1), "");
    vertices1.push_back( std::make_shared<Node>(Node(2, {0.05, 0.05})));
    EXPECT_DEATH(Element elem(2, vertices1), "");

    std::vector<std::shared_ptr<Node>> vertices2;
    vertices2.push_back( std::make_shared<Node>(Node(3, {0.0, 0.0, 0.0})) );
    vertices2.push_back( std::make_shared<Node>(Node(3, {0.0, 0.1, 0.0})) );
    vertices2.push_back( std::make_shared<Node>(Node(3, {0.1, 0.1, 0.0})) );
    vertices2.push_back( std::make_shared<Node>(Node(3, {0.1, 0.0, 0.0})) );
    vertices2.push_back( std::make_shared<Node>(Node(3, {0.0, 0.0, 0.2})) );
    vertices2.push_back( std::make_shared<Node>(Node(3, {0.0, 0.1, 0.2})) );
    vertices2.push_back( std::make_shared<Node>(Node(3, {0.1, 0.1, 0.2})) );
    vertices2.push_back( std::make_shared<Node>(Node(3, {0.1, 0.0, 0.2})) );

    Element elem2(3, vertices2);
    EXPECT_DEATH(Element elem(2, vertices2), "");
    vertices2.pop_back();
    EXPECT_DEATH(Element elem(3, vertices2), "");
    vertices2.push_back( std::make_shared<Node>(Node(2, {0.05, 0.05})) );
    EXPECT_DEATH(Element elem(3, vertices2), "");
    vertices2.pop_back();
    vertices2.push_back( std::make_shared<Node>(Node(3, {0.0, 0.0, 0.2})) );
    EXPECT_DEATH(Element elem(3, vertices2), "");
}

TEST(MeshLib_Element, boundary_node_update){
 
    std::vector<std::shared_ptr<Node>> vertices1;
    vertices1.push_back( std::make_shared<Node>(Node(2, {0.0, 0.0})) );
    vertices1.push_back( std::make_shared<Node>(Node(2, {0.1, 0.0})) );
    vertices1.push_back( std::make_shared<Node>(Node(2, {0.1, 0.1})) );
    vertices1.push_back( std::make_shared<Node>(Node(2, {0.0, 0.1})) );

    Element elem1(2, vertices1);
    EXPECT_EQ(elem1.numBoundaryNodes(), 0);
    auto bndyNodes = elem1.getBoundaryNodes();
    EXPECT_EQ(bndyNodes.size(), 0);

    vertices1[2]->makeBoundaryNode();
    EXPECT_EQ(elem1.numBoundaryNodes(), 1);
    bndyNodes = elem1.getBoundaryNodes();
    EXPECT_EQ(bndyNodes.size(), 1);
    EXPECT_EQ(bndyNodes[0], vertices1[2]);

}

TEST(MeshLib_Element, comparison){
    std::vector<std::shared_ptr<Node>> vert;
    vert.push_back( std::make_shared<Node>(Node(2, {0.0, 0.0})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.1, 0.0})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.2, 0.0})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.0, 0.1})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.1, 0.1})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.2, 0.1})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.0, 0.2})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.1, 0.2})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.2, 0.2})) );

    Element elem1(2, {vert[0], vert[1], vert[4], vert[3]});
    Element elem2(2, {vert[0], vert[1], vert[4], vert[3]});
    Element elem3(2, {vert[0], vert[1], vert[5], vert[3]});
    Element elem4(2, {vert[4], vert[5], vert[8], vert[7]});
    Element elem5(2, {vert[0], vert[4], vert[3], vert[1]});

    EXPECT_TRUE(elem1 == elem2);
    EXPECT_FALSE(elem1 == elem3);
    EXPECT_FALSE(elem1 == elem4);
    EXPECT_TRUE(elem1 == elem5);
}

TEST(MeshLib_Element, compparison_lt){
    std::vector<std::shared_ptr<Node>> vert;
    vert.push_back( std::make_shared<Node>(Node(2, {0.0, 0.0})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.1, 0.0})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.2, 0.0})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.0, 0.1})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.1, 0.1})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.2, 0.1})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.0, 0.2})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.1, 0.2})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.2, 0.2})) );


    Element elem1(2, {vert[0], vert[1], vert[4], vert[3]});
    Element elem2(2, {vert[0], vert[1], vert[4], vert[3]});
    Element elem3(2, {vert[0], vert[1], vert[5], vert[3]});
    Element elem4(2, {vert[4], vert[5], vert[8], vert[7]});
    Element elem5(2, {vert[0], vert[4], vert[3], vert[1]});

    EXPECT_FALSE( elem1 < elem2 );
    EXPECT_FALSE( elem2 < elem1 );
    EXPECT_FALSE( elem1 < elem5 );
    EXPECT_FALSE( elem5 < elem1 );

    EXPECT_TRUE( elem1 < elem3 );
    EXPECT_TRUE( elem1 < elem4 );
    EXPECT_TRUE( elem3 < elem4 );
    EXPECT_FALSE( elem3 < elem1 );
    EXPECT_FALSE( elem4 < elem1 );
    EXPECT_FALSE( elem4 < elem3 );

}

TEST(MeshLib_Element, neighbors){
    
    std::vector<std::shared_ptr<Node>> vert;
    vert.push_back( std::make_shared<Node>(Node(2, {0.0, 0.0})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.1, 0.0})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.2, 0.0})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.3, 0.0})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.0, 0.1})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.1, 0.1})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.2, 0.1})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.3, 0.1})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.0, 0.2})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.1, 0.2})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.2, 0.2})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.3, 0.2})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.0, 0.3})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.1, 0.3})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.2, 0.3})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.3, 0.3})) );

    Element elem1(2, {vert[0], vert[1], vert[5], vert[4]});
    Element elem2(2, {vert[1], vert[2], vert[6], vert[5]});
    Element elem3(2, {vert[2], vert[3], vert[7], vert[6]});
    Element elem4(2, {vert[4], vert[5], vert[9], vert[8]});
    Element elem5(2, {vert[5], vert[6], vert[10], vert[9]});
    Element elem6(2, {vert[6], vert[7], vert[11], vert[10]});
    Element elem7(2, {vert[8], vert[9], vert[13], vert[12]});
    Element elem8(2, {vert[9], vert[10], vert[14], vert[13]});
    Element elem9(2, {vert[10], vert[11], vert[15], vert[14]});

    for (const auto &index : {0, 1, 2, 3, 4, 7, 8, 11, 12, 13, 14, 15})
        vert[index]->makeBoundaryNode();
    EXPECT_EQ( elem1.numBoundaryNodes(), 3);
    EXPECT_EQ( elem2.numBoundaryNodes(), 2);
    EXPECT_EQ( elem5.numBoundaryNodes(), 0);
    EXPECT_EQ( elem8.numBoundaryNodes(), 2);

    elem1.addNeighborElement(std::make_shared<Element>(elem2));
    elem1.addNeighborElement(std::make_shared<Element>(elem4));
    EXPECT_EQ(elem1.numNeighbors(), 2);

    elem2.addNeighborElement(std::make_shared<Element>(elem1));
    elem2.addNeighborElement(std::make_shared<Element>(elem3));
    elem2.addNeighborElement(std::make_shared<Element>(elem5));
    EXPECT_EQ(elem2.numNeighbors(), 3);

    elem3.addNeighborElement(std::make_shared<Element>(elem2));
    elem3.addNeighborElement(std::make_shared<Element>(elem6));
    EXPECT_EQ(elem3.numNeighbors(), 2);

    elem4.addNeighborElement(std::make_shared<Element>(elem1));
    elem4.addNeighborElement(std::make_shared<Element>(elem5));
    elem4.addNeighborElement(std::make_shared<Element>(elem7));

    elem5.addNeighborElement(std::make_shared<Element>(elem2));
    elem5.addNeighborElement(std::make_shared<Element>(elem4));
    elem5.addNeighborElement(std::make_shared<Element>(elem6));
    elem5.addNeighborElement(std::make_shared<Element>(elem8));
    EXPECT_EQ(elem5.numNeighbors(), 4);

    elem6.addNeighborElement(std::make_shared<Element>(elem3));
    elem6.addNeighborElement(std::make_shared<Element>(elem5));
    elem6.addNeighborElement(std::make_shared<Element>(elem9));

    elem7.addNeighborElement(std::make_shared<Element>(elem4));
    elem7.addNeighborElement(std::make_shared<Element>(elem8));

    elem8.addNeighborElement(std::make_shared<Element>(elem5));
    elem8.addNeighborElement(std::make_shared<Element>(elem7));
    elem8.addNeighborElement(std::make_shared<Element>(elem9));

    elem9.addNeighborElement(std::make_shared<Element>(elem6));
    elem9.addNeighborElement(std::make_shared<Element>(elem8));

    elem5.removeNeighborElement(std::make_shared<Element>(elem4));
    EXPECT_EQ(elem5.numNeighbors(), 3);
}

TEST(DebugTestName(MeshLib_Element), neighbors_debug){
    std::vector<std::shared_ptr<Node>> vert;
    vert.push_back( std::make_shared<Node>(Node(2, {0.0, 0.0})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.1, 0.0})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.2, 0.0})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.3, 0.0})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.0, 0.1})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.1, 0.1})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.2, 0.1})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.3, 0.1})) );

    Element elem1(2, {vert[0], vert[1], vert[5], vert[4]});
    Element elem2(2, {vert[1], vert[2], vert[6], vert[5]});
    Element elem3(2, {vert[2], vert[3], vert[7], vert[6]});

    elem1.addNeighborElement(std::make_shared<Element>(elem2));

    // Check death on re-adding neighbor
    EXPECT_DEATH(elem1.addNeighborElement(std::make_shared<Element>(elem2)), "");
    // Check removing non-existent neighbor
    EXPECT_DEATH(elem2.removeNeighborElement(std::make_shared<Element>(elem3)), "");
}

TEST(MeshLib_Element, check_neighbor_2d){
    
    std::vector<std::shared_ptr<Node>> vert;
    vert.push_back( std::make_shared<Node>(Node(2, {0.0, 0.0})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.1, 0.0})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.2, 0.0})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.3, 0.0})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.0, 0.1})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.1, 0.1})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.2, 0.1})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.3, 0.1})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.0, 0.2})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.1, 0.2})) );

    Element elem1(2, {vert[0], vert[1], vert[5], vert[4]});
    Element elem2(2, {vert[1], vert[2], vert[6], vert[5]});
    Element elem4(2, {vert[4], vert[5], vert[9], vert[8]});

    Element elem3(2, {vert[2], vert[3], vert[7], vert[6]});

    // Neighbors
    EXPECT_TRUE( elem1.isNeighborElement(std::make_shared<Element>(elem4)) );
    EXPECT_TRUE( elem4.isNeighborElement(std::make_shared<Element>(elem1)) );
    EXPECT_TRUE( elem1.isNeighborElement(std::make_shared<Element>(elem2)) );
    EXPECT_TRUE( elem2.isNeighborElement(std::make_shared<Element>(elem1)) );

    // Shares 1 node
    EXPECT_FALSE( elem2.isNeighborElement(std::make_shared<Element>(elem4)) );
    EXPECT_FALSE( elem4.isNeighborElement(std::make_shared<Element>(elem2)) );

    // Shares no nodes
    EXPECT_FALSE( elem1.isNeighborElement(std::make_shared<Element>(elem3)) );
    EXPECT_FALSE( elem3.isNeighborElement(std::make_shared<Element>(elem1)) );
    EXPECT_FALSE( elem4.isNeighborElement(std::make_shared<Element>(elem3)) );
    EXPECT_FALSE( elem3.isNeighborElement(std::make_shared<Element>(elem4)) );
}

TEST(MeshLib_Element, shared_vertices_2d){
    
    std::vector<std::shared_ptr<Node>> vert;
    vert.push_back( std::make_shared<Node>(Node(2, {0.0, 0.0})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.1, 0.0})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.2, 0.0})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.3, 0.0})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.0, 0.1})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.1, 0.1})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.2, 0.1})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.3, 0.1})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.0, 0.2})) );
    vert.push_back( std::make_shared<Node>(Node(2, {0.1, 0.2})) );

    Element elem1(2, {vert[0], vert[1], vert[5], vert[4]});
    Element elem2(2, {vert[1], vert[2], vert[6], vert[5]});
    Element elem4(2, {vert[4], vert[5], vert[9], vert[8]});

    Element elem3(2, {vert[2], vert[3], vert[7], vert[6]});

    // Two shared nodes
    EXPECT_THAT(elem1.getSharedVertices(std::make_shared<Element>(elem4)), ElementsAreArray({vert[4], vert[5]}));
    EXPECT_THAT(elem4.getSharedVertices(std::make_shared<Element>(elem1)), ElementsAreArray({vert[4], vert[5]}));
    EXPECT_THAT(elem1.getSharedVertices(std::make_shared<Element>(elem2)), ElementsAreArray({vert[1], vert[5]}));
    EXPECT_THAT(elem2.getSharedVertices(std::make_shared<Element>(elem1)), ElementsAreArray({vert[1], vert[5]}));

    // Shares 1 node
    EXPECT_THAT(elem2.getSharedVertices(std::make_shared<Element>(elem4)), ElementsAreArray({vert[5]}));
    EXPECT_THAT(elem4.getSharedVertices(std::make_shared<Element>(elem2)), ElementsAreArray({vert[5]}));

    // Shares no nodes
    EXPECT_EQ(elem1.getSharedVertices(std::make_shared<Element>(elem3)).size(), 0);
    EXPECT_EQ(elem3.getSharedVertices(std::make_shared<Element>(elem1)).size(), 0);
    EXPECT_EQ(elem4.getSharedVertices(std::make_shared<Element>(elem3)).size(), 0);
    EXPECT_EQ(elem3.getSharedVertices(std::make_shared<Element>(elem4)).size(), 0);
}

TEST(MeshLib_Element, check_neighbor_3d){
    
    std::vector<std::shared_ptr<Node>> vert;
    vert.push_back( std::make_shared<Node>(Node(3, {0.0, 0.0, 0.0})) ); // 0
    vert.push_back( std::make_shared<Node>(Node(3, {0.1, 0.0, 0.0})) ); // 1
    vert.push_back( std::make_shared<Node>(Node(3, {0.2, 0.0, 0.0})) ); // 2

    vert.push_back( std::make_shared<Node>(Node(3, {0.0, 0.1, 0.0})) ); // 3
    vert.push_back( std::make_shared<Node>(Node(3, {0.1, 0.1, 0.0})) ); // 4
    vert.push_back( std::make_shared<Node>(Node(3, {0.2, 0.1, 0.0})) ); // 5
    
    vert.push_back( std::make_shared<Node>(Node(3, {0.0, 0.0, 0.1})) ); // 6
    vert.push_back( std::make_shared<Node>(Node(3, {0.1, 0.0, 0.1})) ); // 7
    vert.push_back( std::make_shared<Node>(Node(3, {0.2, 0.0, 0.1})) ); // 8

    vert.push_back( std::make_shared<Node>(Node(3, {0.0, 0.1, 0.1})) ); // 9
    vert.push_back( std::make_shared<Node>(Node(3, {0.1, 0.1, 0.1})) ); // 10
    vert.push_back( std::make_shared<Node>(Node(3, {0.2, 0.1, 0.1})) ); // 11

    vert.push_back( std::make_shared<Node>(Node(3, {0.1, 0.2, 0.1})) ); // 12
    vert.push_back( std::make_shared<Node>(Node(3, {0.2, 0.2, 0.1})) ); // 13
    vert.push_back( std::make_shared<Node>(Node(3, {0.1, 0.1, 0.2})) ); // 14
    vert.push_back( std::make_shared<Node>(Node(3, {0.2, 0.1, 0.2})) ); // 15
    vert.push_back( std::make_shared<Node>(Node(3, {0.1, 0.2, 0.2})) ); // 16
    vert.push_back( std::make_shared<Node>(Node(3, {0.2, 0.2, 0.2})) ); // 17

    vert.push_back( std::make_shared<Node>(Node(3, {0.3, 0.0, 0.0})) ); // 18
    vert.push_back( std::make_shared<Node>(Node(3, {0.3, 0.1, 0.0})) ); // 19
    vert.push_back( std::make_shared<Node>(Node(3, {0.3, 0.0, 0.1})) ); // 20
    vert.push_back( std::make_shared<Node>(Node(3, {0.3, 0.1, 0.1})) ); // 21

    Element elem1(3, { vert[0],  vert[1],  vert[4],  vert[3],  vert[6],  vert[7], vert[10],  vert[9]});
    Element elem2(3, { vert[1],  vert[2],  vert[5],  vert[4],  vert[7],  vert[8], vert[11], vert[10]});
    Element elem3(3, {vert[10], vert[11], vert[12], vert[13], vert[14], vert[15], vert[16], vert[17]});
    Element elem4(3, { vert[2], vert[18], vert[19],  vert[5],  vert[8], vert[20], vert[11], vert[21]});

    ASSERT_TRUE( elem1.isNeighborElement(std::make_shared<Element>(elem2)) );
    ASSERT_TRUE( elem2.isNeighborElement(std::make_shared<Element>(elem1)) );

    // Shares 2 nodes
    ASSERT_FALSE( elem2.isNeighborElement(std::make_shared<Element>(elem3)) );
    ASSERT_FALSE( elem3.isNeighborElement(std::make_shared<Element>(elem2)) );

    // Shares 1 node
    ASSERT_FALSE( elem1.isNeighborElement(std::make_shared<Element>(elem3)) );
    ASSERT_FALSE( elem3.isNeighborElement(std::make_shared<Element>(elem1)) );

    // Shares no nodes
    ASSERT_FALSE( elem1.isNeighborElement(std::make_shared<Element>(elem4)) );
    ASSERT_FALSE( elem4.isNeighborElement(std::make_shared<Element>(elem1)) );

}

TEST(MeshLib_Element, shared_vertices_3d){
    
    std::vector<std::shared_ptr<Node>> vert;
    vert.push_back( std::make_shared<Node>(Node(3, {0.0, 0.0, 0.0})) ); // 0
    vert.push_back( std::make_shared<Node>(Node(3, {0.1, 0.0, 0.0})) ); // 1
    vert.push_back( std::make_shared<Node>(Node(3, {0.2, 0.0, 0.0})) ); // 2

    vert.push_back( std::make_shared<Node>(Node(3, {0.0, 0.1, 0.0})) ); // 3
    vert.push_back( std::make_shared<Node>(Node(3, {0.1, 0.1, 0.0})) ); // 4
    vert.push_back( std::make_shared<Node>(Node(3, {0.2, 0.1, 0.0})) ); // 5
    
    vert.push_back( std::make_shared<Node>(Node(3, {0.0, 0.0, 0.1})) ); // 6
    vert.push_back( std::make_shared<Node>(Node(3, {0.1, 0.0, 0.1})) ); // 7
    vert.push_back( std::make_shared<Node>(Node(3, {0.2, 0.0, 0.1})) ); // 8

    vert.push_back( std::make_shared<Node>(Node(3, {0.0, 0.1, 0.1})) ); // 9
    vert.push_back( std::make_shared<Node>(Node(3, {0.1, 0.1, 0.1})) ); // 10
    vert.push_back( std::make_shared<Node>(Node(3, {0.2, 0.1, 0.1})) ); // 11

    vert.push_back( std::make_shared<Node>(Node(3, {0.1, 0.2, 0.1})) ); // 12
    vert.push_back( std::make_shared<Node>(Node(3, {0.2, 0.2, 0.1})) ); // 13
    vert.push_back( std::make_shared<Node>(Node(3, {0.1, 0.1, 0.2})) ); // 14
    vert.push_back( std::make_shared<Node>(Node(3, {0.2, 0.1, 0.2})) ); // 15
    vert.push_back( std::make_shared<Node>(Node(3, {0.1, 0.2, 0.2})) ); // 16
    vert.push_back( std::make_shared<Node>(Node(3, {0.2, 0.2, 0.2})) ); // 17

    vert.push_back( std::make_shared<Node>(Node(3, {0.3, 0.0, 0.0})) ); // 18
    vert.push_back( std::make_shared<Node>(Node(3, {0.3, 0.1, 0.0})) ); // 19
    vert.push_back( std::make_shared<Node>(Node(3, {0.3, 0.0, 0.1})) ); // 20
    vert.push_back( std::make_shared<Node>(Node(3, {0.3, 0.1, 0.1})) ); // 21

    Element elem1(3, { vert[0],  vert[1],  vert[4],  vert[3],  vert[6],  vert[7], vert[10],  vert[9]});
    Element elem2(3, { vert[1],  vert[2],  vert[5],  vert[4],  vert[7],  vert[8], vert[11], vert[10]});
    Element elem3(3, {vert[10], vert[11], vert[12], vert[13], vert[14], vert[15], vert[16], vert[17]});
    Element elem4(3, { vert[2], vert[18], vert[19],  vert[5],  vert[8], vert[20], vert[11], vert[21]});

    // Shares 4 nodes
    EXPECT_THAT( elem1.getSharedVertices(std::make_shared<Element>(elem2)), ElementsAreArray({vert[1], vert[7], vert[4], vert[10]}));
    EXPECT_THAT( elem2.getSharedVertices(std::make_shared<Element>(elem1)), ElementsAreArray({vert[1], vert[7], vert[4], vert[10]}));

    // Shares 2 nodes
    EXPECT_THAT( elem2.getSharedVertices(std::make_shared<Element>(elem3)), ElementsAreArray({vert[10], vert[11]}));
    EXPECT_THAT( elem3.getSharedVertices(std::make_shared<Element>(elem2)), ElementsAreArray({vert[10], vert[11]}));

    // Shares 1 node
    EXPECT_THAT( elem1.getSharedVertices(std::make_shared<Element>(elem3)), ElementsAreArray({vert[10]}));
    EXPECT_THAT( elem3.getSharedVertices(std::make_shared<Element>(elem1)), ElementsAreArray({vert[10]}));

    // Shares no nodes
    EXPECT_EQ( elem1.getSharedVertices(std::make_shared<Element>(elem4)).size(), 0);
    EXPECT_EQ( elem4.getSharedVertices(std::make_shared<Element>(elem1)).size(), 0);
}