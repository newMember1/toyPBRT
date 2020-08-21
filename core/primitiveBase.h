/*
 * contains git and other info
*/
#ifndef PRIMITIVEBASE_H
#define PRIMITIVEBASE_H
#include "baseStructure.h"
#include "materialBase.h"
#include "./collision/aabb.h"

class primitiveBase
{
public:
    primitiveBase(std::shared_ptr<materialBase> _mat):mat(_mat){}
    virtual ~primitiveBase(){}
    virtual bool hit(ray &r,hitRecord &h,float minT,float maxT)=0;
    virtual glm::vec3 normal(const glm::vec3 &surPos)=0;
    virtual glm::vec3 reflect(const glm::vec3 &inDirec,const glm::vec3 &normal)=0;
    virtual bool boxHit(const ray &r,float minT,float maxT)=0;
    virtual void handleMatrix()=0;

    void setTranslate(const glm::vec3 & trans)
    {
        modelMatrix = glm::translate(modelMatrix, trans);
        invModelMatrix = glm::inverse(modelMatrix);
    }

    void setRotate(const glm::vec3 & rotate, float angle)
    {
        modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), rotate);
        invModelMatrix = glm::inverse(modelMatrix);
    }

    void setModelMatrix(const glm::mat4 & m)
    {
        modelMatrix = m;
        invModelMatrix = glm::inverse(modelMatrix);
    }

    aabb aabbBox;
    std::shared_ptr<materialBase> mat=nullptr;
    glm::mat4 modelMatrix {1,0,0,0,
                           0,1,0,0,
                           0,0,1,0,
                           0,0,0,1};
    glm::mat4 invModelMatrix = glm::inverse(modelMatrix);
private:
    primitiveBase(){}
};

#endif // PRIMITIVEBASE_H
