/*
 * contains git and other info
*/
#ifndef PRIMITIVEBASE_H
#define PRIMITIVEBASE_H
#include <vector>
#include "baseStructure.h"
#include "materialBase.h"
#include "./collision/aabb.h"

class primitiveBase
{
public:
    primitiveBase(std::shared_ptr<materialBase> _mat):mat(_mat){}
    virtual ~primitiveBase(){}
    virtual bool hit(ray &r,hitRecord &h,float minT,float maxT) = 0;
    virtual glm::vec3 normal(const glm::vec3 &surPos) = 0;
    virtual glm::vec3 reflect(const glm::vec3 &inDirec,const glm::vec3 &normal) = 0;
    virtual std::vector<std::vector<float>> getModelLinesAndColors() = 0;
    virtual bool boxHit(const ray &r,float minT,float maxT) = 0;
    virtual void handleMatrix() = 0;
    virtual void setTranslate(const glm::vec3 & trans) = 0;
    virtual void setUniformScale(float s) = 0;
    virtual void setNonUniformScale(const glm::vec3 & s) = 0;
    virtual void setRotate(const glm::vec3 & rotateAxis, float angle) = 0;
    virtual void setModelMatrix(const glm::mat4 & m) = 0;

    bool refract(const glm::vec3 & v, const glm::vec3 & n, float niOvernt, glm::vec3 & refracted)
    {
        float dt = glm::dot(v, n);
        float dis = 1.0 - niOvernt * niOvernt * (1 - dt * dt);
        if(dis > 0)
        {
            refracted = niOvernt * (v - n * dt) - n * (float)sqrt(dis);
            return true;
        }
        else
            return false;
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
