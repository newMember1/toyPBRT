#ifndef ONB_H
#define ONB_H

#include "baseStructure.h"

class onb
{
public:
    onb(){}

    glm::vec3 u() const {return axis[0];}
    glm::vec3 v() const {return axis[1];}
    glm::vec3 w() const {return axis[2];}

    glm::vec3 local(float a, float b, float c)
    {
        return a * u() + b * v() + c * w();
    }
    glm::vec3 local(const glm::vec3 & a) const
    {
        return a.x * u() + a.y * v() + a.z * w();
    }

    void buildFromW(const glm::vec3 & n)
    {
        axis[2] = glm::normalize(n);
        glm::vec3 a = (fabs(w().x > 0.9) ? glm::vec3(0, 1, 1): glm::vec3(1, 0, 0));
        axis[1] = glm::normalize(glm::cross(w(), a));
        axis[0] = glm::cross(w(), v());
    }
private:
    glm::vec3 axis[3];
};

#endif // ONB_H
