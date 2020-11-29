#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "./core/primitiveBase.h"
#include "./core/directionGenerator.h"

class triangle :public primitiveBase
{
public:
    triangle(const glm::vec3 & a, const glm::vec3 & b, const glm::vec3 & c, const glm::vec3 & n,std::shared_ptr<materialBase> mat);
    triangle(const glm::vec3 & a, const glm::vec3 & b, const glm::vec3 & c, std::shared_ptr<materialBase> mat);
    bool hit(ray &r, hitRecord &h, float minT, float maxT) override;
    glm::vec3 normal(const glm::vec3 &surPos) override;
    glm::vec3 reflect(const glm::vec3 &inDirec, const glm::vec3 &normal) override;
    std::vector<std::vector<float> > getModelLinesAndColors() override;
    bool boxHit(const ray &r, float minT, float maxT) override;
    void setTranslate(const glm::vec3 &trans) override;
    void setRotate(const glm::vec3 &rotateAxis, float angle) override;
    void setUniformScale(float s) override;
    void setNonUniformScale(const glm::vec3 &s) override;
    void setModelMatrix(const glm::mat4 &m) override;
    void handleMatrix() override;
    bool rayTriangle(ray &r,float &u,float &v,float &t);
    void setTriangle(glm::vec3 & a, glm::vec3 & b, glm::vec3 & c, glm::vec3 & n);
    glm::vec3 pa;
    glm::vec3 pb;
    glm::vec3 pc;
    glm::vec3 n;

private:
    void pushData(std::vector<float> & v, const glm::vec3 & d);
};

#endif // TRIANGLE_H
