#ifndef BVH_H
#define BVH_H
#include"./core/baseStructure.h"
#include"./core/primitiveBase.h"
#include<vector>

class bvh
{
public:
    //to do
    bvh(std::vector<std::shared_ptr<primitiveBase*>> &pList);
}
#endif // BVH_H
