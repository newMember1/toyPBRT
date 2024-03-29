#ifndef SPHERE_H
#define SPHERE_H
#include "./core/primitiveBase.h"
#include "./core/directionPdfAdaptor.h"
class sphere: public primitiveBase
{
public:
    sphere(float _r,glm::vec3 _center,std::shared_ptr<materialBase> _mat);
    bool hit(ray &r, hitRecord &h, float minT, float maxT) override;
    glm::vec3 normal(const glm::vec3 &surPos) override;
    glm::vec3 reflect(const glm::vec3 &inDirec, const glm::vec3 &normal) override;
    std::vector<std::vector<float> > getModelLinesAndColors() override;
    bool boxHit(const ray &r, float minT, float maxT) override;
    void handleMatrix() override;
    void setTranslate(const glm::vec3 &trans) override;
    void setRotate(const glm::vec3 &rotateAxis, float angle) override;
    void setUniformScale(float s) override;
    void setNonUniformScale(const glm::vec3 &s) override;
    void setModelMatrix(const glm::mat4 &m) override;
	virtual glm::vec3 hitXAxis(const glm::vec3 & hitPos);
	virtual glm::vec3 hitYAxis(const glm::vec3 & hitPos);

    glm::vec3 getCenter();
private:
    void createFrame();
    void pushData(std::vector<float> & v, float a, float b, float c);
    void pushData(std::vector<float> &v, const glm::vec3 & d);
    std::vector<float> verts;
    std::vector<float> colors;

    float radius;
    glm::vec3 center;
};


#endif // SPHERE_H
