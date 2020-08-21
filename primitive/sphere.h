#ifndef SPHERE_H
#define SPHERE_H
#include"./core/primitiveBase.h"

class sphere: public primitiveBase
{
public:
    sphere(float _r,glm::vec3 _center,std::shared_ptr<materialBase> _mat);
    bool hit(ray &r, hitRecord &h, float minT, float maxT) override;
    glm::vec3 normal(const glm::vec3 &surPos) override;
    glm::vec3 reflect(const glm::vec3 &inDirec, const glm::vec3 &normal) override;
    bool boxHit(const ray &r, float minT, float maxT) override;
    void handleMatrix() override;
    bool isLight=false;
private:
    float radius;
    glm::vec3 center;
};


#endif // SPHERE_H
