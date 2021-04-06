#ifndef RECT_H
#define RECT_H
#include "./core/primitiveBase.h"

class rectangle : public primitiveBase
{
public:
    rectangle(const glm::vec3 & _oriPos, const glm::vec3 & _wDirec, const glm::vec3 & _hDirec, float _w, float _h, std::shared_ptr<materialBase> _mat, bool flipNormal = false);

    bool hit(ray &r, hitRecord &h, float minT, float maxT) override;
    glm::vec3 normal(const glm::vec3 &surPos) override;
    glm::vec3 reflect(const glm::vec3 &inDirec, const glm::vec3 &normal) override;
    bool boxHit(const ray &r, float minT, float maxT) override;
    void setTranslate(const glm::vec3 &trans) override;
    void setRotate(const glm::vec3 &rotateAxis, float angle) override;
    void setUniformScale(float s) override;
    void setNonUniformScale(const glm::vec3 &s) override;
    void setModelMatrix(const glm::mat4 &m) override;
    std::vector<std::vector<float> > getModelLinesAndColors() override;
    void handleMatrix() override;
	virtual glm::vec3 hitXAxis(const glm::vec3 & hitPos);
	virtual glm::vec3 hitYAxis(const glm::vec3 & hitPos);

    bool rayRect(ray & r, float &u, float &v, float &t);

    glm::vec3 n;
    glm::vec3 oriPos;
    glm::vec3 wDirec;
    glm::vec3 hDirec;

    glm::vec3 pa;
    glm::vec3 pb;
    glm::vec3 pc;
    glm::vec3 pd;
    float w;
    float h;

private:
    void pushData(std::vector<float> &v, const glm::vec3 & d);
};

#endif // RECT_H
