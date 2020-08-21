#ifndef COSINEPDF_H
#define COSINEPDF_H

#include "./core/pdfBase.h"
#include "./core/onb.h"

class cosinePdf: public pdfBase
{
public:
    glm::vec3 generate(const glm::vec3 & surfPos) const override
    {
        uvw.buildFromW(surfPos);
        return uvw.local(this->randomCosineDirection());
    }

    float value(const glm::vec3 &direction) const override
    {
        auto cosine = glm::dot(glm::normalize(direction), uvw.w());
        return (cosine <= 0) ? 0: cosine / PI;
    }

private:
    mutable onb uvw;
};

#endif // COSINEPDF_H
