#ifndef RECTLIGHTPDF_H
#define RECTLIGHTPDF_H

#include "../core/pdfBase.h"
#include "../primitive/rectangle.h"

class rectLightPdf : public pdfBase
{
public:
    rectLightPdf(std::shared_ptr<rectangle> rec)
    {
        lightRect = rec;
    }

    glm::vec3 generate(const glm::vec3 & p, float roughnessA, float roughnessB) const override
    {
        float e1 = drand48();
        float e2 = drand48();
        glm::vec3 t = lightRect->oriPos + lightRect->wDirec * e1 * lightRect->w + lightRect->hDirec * e2 * lightRect->h;
        toLight = t - p;
        return glm::normalize(toLight);
    }

    float value(const glm::vec3 & direction) const override
    {
        //note that we integral dA
        //in fact the pdf is 1/area because we integral on lightRect
        //so the brdf item should multiply cosTheta/dis2
        //cause we want to combine them together so we could divede dis2/cosTheta
        //after combine the pdf become 1/area*dis2/cosTheta = dis2/(area*cosTheta)
        float area = lightRect->w * lightRect->h;
        float dis2 = toLight.length() * toLight.length();
        float cosTheta = glm::dot(glm::normalize(-toLight), lightRect->normal(glm::vec3(0)));
        return dis2 / (area * cosTheta);
    }

    lightType type() const override
    {
        return lightType::rectangle;
    }

private:
    std::shared_ptr<rectangle> lightRect;
    mutable glm::vec3 toLight;
};

#endif // RECTLIGHTPDF_H
