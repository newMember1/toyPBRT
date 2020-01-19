#ifndef PRIMITIVELIST_H
#define PRIMITIVELIST_H
#include"primitiveBase.h"
#include<vector>
class primitiveList
{
public:
    void addPrimitive(std::unique_ptr<primitiveBase*> ptr);
    bool hit(ray &r,hitRecord &h,float minT,float maxT);

    std::vector<std::unique_ptr<primitiveBase *>> pList;
};

#endif // PRIMITIVELIST_H
