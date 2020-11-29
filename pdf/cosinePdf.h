#ifndef COSINEPDF_H
#define COSINEPDF_H

#include "./core/pdfBase.h"
#include "./core/onb.h"

class cosinePdf: public pdfBase
{
public:
    glm::vec3 generate(const glm::vec3 & p) const override
    {
        uvw.buildFromW(p);
        return uvw.local(this->randomCosineDirection());
    }

    float value(const glm::vec3 &direction) const override
    {
        auto cosine = glm::dot(glm::normalize(direction), uvw.w());
        return (cosine <= 0) ? 0: cosine / PI;
    }

private:
    mutable onb uvw;

    glm::vec3 randomCosineDirection() const
    {
        auto r1 = drand48();
        auto r2 = drand48();
        auto z = sqrt(1-r2);

        auto phi = 2 * PI * r1;
        auto x = cos(phi)*sqrt(r2);
        auto y = sin(phi)*sqrt(r2);

        return glm::vec3(x, y, z);
    }
};

#endif // COSINEPDF_H
