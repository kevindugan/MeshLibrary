#include "../Node.h"
#include "gmock/gmock.h"


TEST(MeshLib_Node, constructor){
//   LinearAlgebra init;
//   Vector_BlockPartition v(128, init);

//   v.setValues(0.12);
//   ASSERT_DOUBLE_EQ(v.l2norm(), 1.3576450198781713);
//   v.zeros();
//   ASSERT_DOUBLE_EQ(v.l2norm(), 0.0);
    Node node1(2, {1.2, 3.2});
    
}