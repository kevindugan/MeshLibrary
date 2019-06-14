#include "Element.h"
#include "gmock/gmock.h"

TEST(MeshLib_Element, constructor){

    std::vector<Node> vertices1;
    vertices1.push_back( Node(2, {0.0, 0.0}) );
    vertices1.push_back( Node(2, {0.1, 0.0}) );
    vertices1.push_back( Node(2, {0.1, 0.1}) );
    vertices1.push_back( Node(2, {0.0, 0.1}) );

    Element elem1(2, vertices1);
    EXPECT_DEATH(Element elem(3, vertices1), "");
    vertices1.push_back( Node(2, {0.05, 0.05}));
    EXPECT_DEATH(Element elem(2, vertices1), "");

    std::vector<Node> vertices2;
    vertices2.push_back( Node(3, {0.0, 0.0, 0.0}) );
    vertices2.push_back( Node(3, {0.0, 0.1, 0.0}) );
    vertices2.push_back( Node(3, {0.1, 0.1, 0.0}) );
    vertices2.push_back( Node(3, {0.1, 0.0, 0.0}) );
    vertices2.push_back( Node(3, {0.0, 0.0, 0.2}) );
    vertices2.push_back( Node(3, {0.0, 0.1, 0.2}) );
    vertices2.push_back( Node(3, {0.1, 0.1, 0.2}) );
    vertices2.push_back( Node(3, {0.1, 0.0, 0.2}) );

    Element elem2(3, vertices2);
    EXPECT_DEATH(Element elem(2, vertices2), "");
    vertices2.pop_back();
    EXPECT_DEATH(Element elem(3, vertices2), "");
    vertices2.push_back( Node(2, {0.05, 0.05}) );
    EXPECT_DEATH(Element elem(3, vertices2), "");
    vertices2.pop_back();
    vertices2.push_back( Node(3, {0.0, 0.0, 0.2}) );
    EXPECT_DEATH(Element elem(3, vertices2), "");

}