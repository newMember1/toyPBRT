#ifndef SPHERELIGHTPDF_H
#define SPHERELIGHTPDF_H

#include "../core/pdfBase.h"
#include "../primitive/sphere.h"

class sphereLightPdf : public pdfBase
{
public:
    sphereLightPdf(std::shared_ptr<sphere> ball)
    {
        lightSphere = ball;
    }

    glm::vec3 generate(const glm::vec3 &p, float roughnessA, float roughnessB) const override
    {
        //(TO DO)need to sample area in whole sphere
        //simple generate light toward this sphere
        return glm::normalize(lightSphere->getCenter() - p);
    }

    float value(const glm::vec3 &direction) const override
    {
        //(TO DO)need to change with generate function
        return 1.0;
    }

    lightType type() const override
    {
        return lightType::sphere;
    }

private:
    std::shared_ptr<sphere> lightSphere;
};

#endif // SPHERELIGHTPDF_H
