#ifndef AABB_H
#define AABB_H
#include"./core/baseStructure.h"

class aabb
{
public:
    aabb()
    {
        _min=glm::vec3(0);
        _max=glm::vec3(0);
    }
    aabb(const glm::vec3 &a,const glm::vec3 &b)
    {
        _min=a;
        _max=b;
    }
    ~aabb(){}
    bool hit(const ray &r,float minT,float maxT)
    {
        for(int i=0;i<3;++i)
        {
            float t0=fmin((_min[i]-r.pos[i])/r.direc[i],
                          (_max[i]-r.pos[i])/r.direc[i]);
            float t1=fmax((_min[i]-r.pos[i])/r.direc[i],
                          (_max[i]-r.pos[i])/r.direc[i]);

            minT=fmax(t0,minT);
            maxT=fmin(t1,maxT);
            if(maxT<=minT)
                return false;
        }
        return true;
    }
    glm::vec3 _min;
    glm::vec3 _max;
};

inline aabb surrounding_box(const aabb &box0,const aabb &box1)
{
    glm::vec3 small(fmin(box0._min.x, box1._min.x),
                    fmin(box0._min.y, box1._min.y),
                    fmin(box0._min.z, box1._min.z));

    glm::vec3 big(fmax(box0._max.x, box1._max.x),
                  fmax(box0._max.y, box1._max.y),
                  fmax(box0._max.z, box1._max.z));

    return aabb(small,big);
}

#endif // AABB_H
