#ifndef BASICBRDFMATERIAL_H
#define BASICBRDFMATERIAL_H

#include "../core/materialBase.h"

class basicBRDFMaterial : public materialBase
{
public:
    basicBRDFMaterial(std::shared_ptr<texture> texture);
    basicBRDFMaterial(std::shared_ptr<texture> texture, float m, float r);

    void setMetallic(float m);
    void setRoughness(float r);
    glm::vec3 albedo(const hitRecord &hitRec, const glm::vec3 &lightDirec, const glm::vec3 &eyeDirec) override;
private:
    float D();
    glm::vec3 F(float a);
    float G();
    float GSub(float a);

    float metallic = 0;
    float roughness = 0;

    float NdotH;
    float NdotL;
    float NdotV;
    float LdotH;

    glm::vec3 F0{0.04f};

    float pow5(float a)
    {
        return a * a * a * a *a;
    }
};

#endif // BASICBRDFMATERIAL_H
