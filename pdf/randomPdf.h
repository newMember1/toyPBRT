#ifndef RANDOMPDF_H
#define RANDOMPDF_H

#include "../core/pdfBase.h"

class randomPdf : public pdfBase
{
public:
    glm::vec3 generate(const glm::vec3 & p) const override
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

private:
//    glm::vec3 randomToSphere(double radius, double dis2) const
//    {
//        auto r1 = drand48();
//        auto r2 = drand48();
//        auto z = 1 + r2 * (sqrt(1 - radius * radius / dis2) - 1);

//        auto phi = 2 * PI * r1;
//        auto x = cos(phi)*sqrt(1-z*z);
//        auto y = sin(phi)*sqrt(1-z*z);

//        return glm::vec3(x, y, z);
//    }

};

#endif // RANDOMPDF_H
