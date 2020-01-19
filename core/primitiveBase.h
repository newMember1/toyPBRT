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
    virtual ~primitiveBase(){}
    virtual bool hit(ray &r,hitRecord &hR,float minT,float maxT){}
    virtual glm::vec3 normal(const glm::vec3 &surPos){}
    virtual glm::vec3 reflect(const glm::vec3 &inDirec,const glm::vec3 surPos){}
    virtual aabb box();

    std::unique_ptr<materialBase *> mat;
};

#endif // PRIMITIVEBASE_H
