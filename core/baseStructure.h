/*use glm to handle operation such as rotate and transpose,may use Eigen or other libs in future*/
#ifndef BASESTRUCTURE_H
#define BASESTRUCTURE_H
#include<memory>
#include<3rdparty/glm/glm.hpp>
#include<3rdparty/glm/gtc/matrix_transform.hpp>
#include<iostream>

const float PI = 3.1415026;
const float epslion = 1e-5;

const int MAX_TRACE_TIMES = 20;
static unsigned long long seed = 1;
#define magic1 0x100000000LL
#define magic2 0xB16
#define magic3 0x5DEECE66DLL

inline void srand48(unsigned int i)
{
    seed = (((long long int)i) << 16) | rand();
}

inline double drand48(void)
{
    seed = (magic3 * seed + magic2) & 0xFFFFFFFFFFFFLL;
    unsigned int x = seed >> 16;
    return  ((double)x / (double)magic1);
}

inline float pow2(float a)
{
    return a*a;
}

inline float pow5(float a)
{
    return pow2(a)*pow2(a)*a;
}

inline float schlick(float cosine, float ref)
{
    float r0 = (1 - ref) / (1 + ref);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

struct ray
{
    glm::vec3 pos;
    glm::vec3 direc;
    ray(const glm::vec3 &p,const glm::vec3 &d)
    {
        pos=p;
        direc=glm::normalize(d);
    }
};

struct line
{
    glm::vec3 start;
    glm::vec3 end;
    line(const glm::vec3 & s,const glm::vec3 &e)
    {
        start=s;
        end=e;
    }
    float len2()
    {
        return (start.x-end.x)*(start.x-end.x)+(start.y-end.y)*(start.y-end.y)+(start.z-end.z)*(start.z-end.z);
    }
    float len()
    {
        return sqrt(len2());
    }

    float len2() const
    {
        return (start.x-end.x)*(start.x-end.x)+(start.y-end.y)*(start.y-end.y)+(start.z-end.z)*(start.z-end.z);
    }
    float len() const
    {
        return sqrt(len2());
    }
};

enum class primitiveType
{
    triangle,
    rectangle,
    sphere,
    unknown
};

enum class materialType
{
    simpleMaterial,
    dielectrics,
    basicBRDFMatrial,
    disneyBRDFMaterial
};

enum class colorMode
{
    iterator,
    recursive,
    hitTest,
    normalVis,
    normalTest,
    test
};
#endif // BASESTRUCTURE_H
