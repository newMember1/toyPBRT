#ifndef UTIL_H
#define UTIL_H
#include<glm.hpp>

inline  glm::vec3 randSphere()
{
    glm::vec3 res;
    do {
        res = glm::vec3(2 * drand48() - 1, 2 * drand48() - 1, 2 * drand48() - 1);
    } while (glm::dot(res, res) >= 1);
    return glm::normalize(res);
}

#endif // UTIL_H
