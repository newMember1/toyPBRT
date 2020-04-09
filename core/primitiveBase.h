/*
 * contains git and other info
*/
#ifndef PRIMITIVEBASE_H
#define PRIMITIVEBASE_H
#include"baseStructure.h"
#include"materialBase.h"
#include"./collision/aabb.h"

class primitiveBase
{
public:
    primitiveBase(std::shared_ptr<materialBase> _mat):mat(_mat){}
    virtual ~primitiveBase(){}
    virtual bool hit(ray &r,hitRecord &h,float minT,float maxT)=0;
    virtual glm::vec3 normal(const glm::vec3 &surPos)=0;
    virtual glm::vec3 reflect(const glm::vec3 &inDirec,const glm::vec3 &normal)=0;
    virtual bool boxHit(const ray &r,float minT,float maxT)=0;

    aabb aabbBox;
    std::shared_ptr<materialBase> mat=nullptr;

private:
    primitiveBase(){}
};

#endif // PRIMITIVEBASE_H
