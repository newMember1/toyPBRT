#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "./core/primitiveBase.h"
#include "./core/pdfGenerate.h"

class triangle :public primitiveBase
{
public:
    triangle(const glm::vec3 &_a,const glm::vec3 &_b,const glm::vec3 &_c,const glm::vec3 &_n,std::shared_ptr<materialBase> _mat);
    bool hit(ray &r, hitRecord &h, float minT, float maxT) override;
    glm::vec3 normal(const glm::vec3 &surPos) override;
    glm::vec3 reflect(const glm::vec3 &inDirec, const glm::vec3 &normal) override;
    bool boxHit(const ray &r, float minT, float maxT) override;
    void handleMatrix() override;
    bool rayTriangle(ray &r,float &u,float &v,float &t);
    void setTriangle(glm::vec3 & a, glm::vec3 & b, glm::vec3 & c, glm::vec3 & n);
    glm::vec3 pa;
    glm::vec3 pb;
    glm::vec3 pc;
    glm::vec3 n;
};

#endif // TRIANGLE_H
