#ifndef CUBE_H
#define CUBE_H
#include "./core/primitiveBase.h"
#include "./rectangle.h"

class cube : public primitiveBase
{
public:
    cube(const glm::vec3 & _min, const glm::vec3 & _max, std::shared_ptr<materialBase> _mat, bool _isInner = false);
    bool hit(ray &r, hitRecord &h, float minT, float maxT) override;
    glm::vec3 normal(const glm::vec3 &surPos) override;
    glm::vec3 reflect(const glm::vec3 &inDirec, const glm::vec3 &normal) override;
    std::vector<std::vector<float>> getModelLinesAndColors() override;
    void setTranslate(const glm::vec3 &trans) override;
    void setRotate(const glm::vec3 &rotateAxis, float angle) override;
    void setUniformScale(float s) override;
    void setNonUniformScale(const glm::vec3 &s) override;
    void setModelMatrix(const glm::mat4 &m) override;
    bool boxHit(const ray &r, float minT, float maxT) override;
    void handleMatrix() override;
	virtual glm::vec3 hitXAxis(const glm::vec3 & hitPos);
	virtual glm::vec3 hitYAxis(const glm::vec3 & hitPos);

private:
    std::shared_ptr<rectangle> faces[6];
    std::vector<std::shared_ptr<materialBase>> mats;
};

#endif // CUBE_H
