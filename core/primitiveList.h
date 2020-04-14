#ifndef PRIMITIVELIST_H
#define PRIMITIVELIST_H
#include"primitiveBase.h"
#include"../accelerate/bvh.h"
#include<vector>
class primitiveList
{
public:
    primitiveList(){}
    primitiveList(std::vector<std::shared_ptr<primitiveBase>> &datas);
    void addPrimitive(std::shared_ptr<primitiveBase> ptr);
    bool hit(ray &r,hitRecord &h,float minT,float maxT);

    glm::vec3 colorHitTest(ray &r,int times);
    glm::vec3 color(ray &r,int times);
    std::vector<std::shared_ptr<primitiveBase>> pList;

    std::unique_ptr<bvh> allModels=nullptr;
};
#endif // PRIMITIVELIST_H
