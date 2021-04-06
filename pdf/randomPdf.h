#ifndef RANDOMPDF_H
#define RANDOMPDF_H

#include "../core/pdfBase.h"

class randomPdf : public pdfBase
{
public:
    glm::vec3 generate(const glm::vec3 & p, float roughnessA, float roughnessB) const override
    {
        glm::vec3 dir;
        do
        {
            dir.x = drand48() * 2 - 1;
            dir.y = drand48() * 2 - 1;
            dir.z = drand48() * 2 - 1;
        }
        while(glm::length(dir) > 1);

        return p + dir;
    }

    float value(const glm::vec3 & direction) const override
    {
        return 2.0;
    }

    lightType type() const override
    {
        return lightType::diffuse;
    }
};

#endif // RANDOMPDF_H
