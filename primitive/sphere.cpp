#include "sphere.h"
#include"./core/baseStructure.h"

extern bool debugFlag;

sphere::sphere(float _r,glm::vec3 _center,std::shared_ptr<materialBase> _mat)
    :primitiveBase(_mat)
{
    this->radius=_r;
    this->center=_center;
    this->aabbBox=aabb(this->center-glm::vec3(radius),
                       this->center+glm::vec3(radius));
}

bool sphere::hit(ray &r, hitRecord &h, float minT, float maxT)
{
    if(!boxHit(r,minT,maxT))
        return false;

    glm::vec3 oc=r.pos-this->center;
    float a=glm::dot(r.direc,r.direc);
    float b=2*glm::dot(r.direc,oc);
    float c=glm::dot(oc,oc)-pow2(radius);

    float dis=b*b-4*a*c;
    if(dis<0)
        return false;
    else
    {
        float t=(-b-sqrt(dis))/2.0/a;
        if(t>minT&&t<maxT&&t<h.t)
        {
            h.t=t;
            h.hitPos=r.pos+t*r.direc;
            h.hitNormal=this->normal(h.hitPos);

            h.hitOutDirec=this->reflect(r.direc,h.hitNormal);
            h.matPtr=this->mat;
            return true;
        }
        else
            return false;
    }
}

glm::vec3 sphere::normal(const glm::vec3 &surPos)
{
    return glm::normalize(surPos-this->center);
}

glm::vec3 sphere::reflect(const glm::vec3 &inDirec, const glm::vec3 &normal)
{
    return glm::normalize(inDirec - 2 * glm::dot(inDirec, normal)*normal);
}

bool sphere::boxHit(const ray &r, float minT, float maxT)
{
    return aabbBox.hit(r,minT,maxT);
}

void sphere::handleMatrix()
{
    center = glm::vec3(glm::vec4(center, 1.0) * modelMatrix);
}
