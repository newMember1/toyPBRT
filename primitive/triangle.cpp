#include "triangle.h"

triangle::triangle(const glm::vec3 &_a,const glm::vec3 &_b,const glm::vec3 &_c,const glm::vec3 &_n,std::shared_ptr<materialBase> _mat):
    primitiveBase(_mat)
{
    pa=_a;
    pb=_b;
    pc=_c;
    n=_n;
}

glm::vec3 triangle::normal(const glm::vec3 &surPos)
{
    return n;
}

glm::vec3 triangle::reflect(const glm::vec3 &inDirec, const glm::vec3 &normal)
{
    return glm::normalize(inDirec - 2 * glm::dot(inDirec, normal)*normal);
}

bool triangle::boxHit(const ray &r, float minT, float maxT)
{
    return aabbBox.hit(r,minT,maxT);
}

bool triangle::rayTriangle(ray &r,float &u,float &v,float &t)
{
    //moller's ray triangle algorithm
    glm::vec3 E1=pb-pa;
    glm::vec3 E2=pc-pa;
    glm::vec3 P=glm::cross(r.direc,E2);
    float det=glm::dot(E1,P);

    glm::vec3 T;
    if(det>0.0f)
        T=r.pos-pa;
    else
    {
        det=-det;
        T=pa-r.pos;
    }
    if(det<epslion)
        return false;

    u=glm::dot(T,P);
    if(u<0.0f||u>det)
        return false;

    glm::vec3 Q=glm::cross(T,E1);
    v=glm::dot(r.direc,Q);
    if(v<0.0f||u+v>det)
        return false;

    float fInvDet=1.0/det;
    t=glm::dot(E2,Q);
    if(t<epslion)
        return false;

    t*=fInvDet;
    u*=fInvDet;
    v*=fInvDet;
    return true;
}

bool triangle::hit(ray &r, hitRecord &h, float minT, float maxT)
{
    if(!boxHit(r,minT,maxT))
        return false;

    float u,v,t;
    if(rayTriangle(r,u,v,t)&&t>minT&&t<maxT&&t<h.t)
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
