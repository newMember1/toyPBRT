#ifndef PDFBASE_H
#define PDFBASE_H

#include "baseStructure.h"

class pdfBase
{
public:
    virtual glm::vec3 generate(const glm::vec3 & surfPos) const  = 0;
    virtual float value(const glm::vec3 & direction) const = 0;

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

    glm::vec3 randomToSphere(double radius, double dis2) const
    {
        auto r1 = drand48();
        auto r2 = drand48();
        auto z = 1 + r2 * (sqrt(1 - radius * radius / dis2) - 1);

        auto phi = 2 * PI * r1;
        auto x = cos(phi)*sqrt(1-z*z);
        auto y = sin(phi)*sqrt(1-z*z);

        return glm::vec3(x, y, z);
    }
};

#endif // PDF_H
