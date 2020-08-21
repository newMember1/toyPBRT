#include "triangle.h"

triangle::triangle(const glm::vec3 &_a,const glm::vec3 &_b,const glm::vec3 &_c,const glm::vec3 &_n,std::shared_ptr<materialBase> _mat):
    primitiveBase(_mat)
{
    pa=_a;
    pb=_b;
    pc=_c;
    n=_n;
    glm::vec3 tmin,tmax;
    tmin.x=std::min(pa.x,std::min(pb.x,pc.x))-epslion;
    tmin.y=std::min(pa.y,std::min(pb.y,pc.y))-epslion;
    tmin.z=std::min(pa.z,std::min(pb.z,pc.z))-epslion;

    tmax.x=std::max(pa.x,std::max(pb.x,pc.x));
    tmax.y=std::max(pa.y,std::max(pb.y,pc.y));
    tmax.z=std::max(pa.z,std::max(pb.z,pc.z));

    this->n = glm::normalize(this->n);
    this->aabbBox._min=tmin;
    this->aabbBox._max=tmax;
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

void triangle::handleMatrix()
{
    pa = glm::vec3(glm::vec4(pa, 1.0) * modelMatrix);
    pb = glm::vec3(glm::vec4(pb, 1.0) * modelMatrix);
    pc = glm::vec3(glm::vec4(pc, 1.0) * modelMatrix);

    glm::mat4 G = glm::transpose(invModelMatrix);
    n = glm::vec3(glm::vec4(n, 1.0) * G);
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
        if(glm::dot(r.direc, h.hitNormal) > 0)
            h.hitNormal = -h.hitNormal;

        //h.hitOutDirec=this->reflect(r.direc,h.hitNormal);
        h.hitOutDirec = pdfGenerate::getInstance().generate(h.hitNormal);
        h.hitPdf = pdfGenerate::getInstance().value(h.hitOutDirec);
        h.matPtr = this->mat;
        return true;
    }
    else
        return false;
}

void triangle::setTriangle(glm::vec3 &a, glm::vec3 &b, glm::vec3 &c, glm::vec3 &n)
{
    this->pa = a;
    this->pb = b;
    this->pc = c;
    this->n = n;
}
