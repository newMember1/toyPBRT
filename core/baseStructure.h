/*use glm to handle operation such as rotate and transpose,may use Eigen or other libs in future*/
#ifndef BASESTRUCTURE_H
#define BASESTRUCTURE_H
#include<memory>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
const float PI=3.1415026;
const float epslion=1e-6;
static unsigned long long seed = 1;
#define magic1 0x100000000LL
#define magic2 0xB16
#define magic3 0x5DEECE66DLL

class materialBase;
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
    float t=1e6;

    float u,v;
    float hitPdf;
    glm::vec3 hitPos;
    glm::vec3 hitNormal;
    glm::vec3 hitOutDirec;
    glm::vec3 hitReflect;

    //a material pointer and anisotropy attributes
    //default xAxis and yAxis
    glm::vec3 xAxis=glm::vec3(0,1,0);
    glm::vec3 yAxis=glm::vec3(1,0,0);
    std::shared_ptr<materialBase> matPtr;
};

#endif // BASESTRUCTURE_H
