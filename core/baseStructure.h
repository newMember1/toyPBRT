/*use glm to handle operation such as rotate and transpose,may use Eigen or other libs in future*/
#ifndef BASESTRUCTURE_H
#define BASESTRUCTURE_H
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
const float PI=3.1415026;
const float epslion=1e-6;
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

struct hitRecord
{
    float t;
    glm::vec3 hitPos;
    glm::vec3 hitNormal;
    glm::vec3 hitReflect;
};

#endif // BASESTRUCTURE_H
