#include "triangle.h"
#include "../core/baseStructure.h"

triangle::triangle(const glm::vec3 & a,const glm::vec3 & b,const glm::vec3 & c,const glm::vec3 & N,std::shared_ptr<materialBase> mat):
    primitiveBase(mat)
{
    pType = primitiveType::triangle;
    pa = a;
    pb = b;
    pc =c;
    this->n = N;
    glm::vec3 tmin,tmax;
    tmin.x=std::min(pa.x,std::min(pb.x,pc.x)) - epslion;
    tmin.y=std::min(pa.y,std::min(pb.y,pc.y)) - epslion;
    tmin.z=std::min(pa.z,std::min(pb.z,pc.z)) - epslion;

    tmax.x=std::max(pa.x,std::max(pb.x,pc.x)) + epslion;
    tmax.y=std::max(pa.y,std::max(pb.y,pc.y)) + epslion;
    tmax.z=std::max(pa.z,std::max(pb.z,pc.z)) + epslion;

    this->n = glm::normalize(this->n);
    this->aabbBox._min=tmin;
    this->aabbBox._max=tmax;
}

triangle::triangle(const glm::vec3 &a,const glm::vec3 &b,const glm::vec3 &c,std::shared_ptr<materialBase> _mat):
    primitiveBase(_mat)
{
    pType = primitiveType::triangle;
    pa = a;
    pb = b;
    pc = c;

    //clockwise calculate normal
    float na = (pb.y - pa.y)*(pc.z - pa.z) - (pc.y - pa.y)*(pb.z - pa.z);
    float nb = (pb.z - pa.z)*(pc.x - pa.x) - (pc.z - pa.z)*(pb.x - pa.x);
    float nc = (pb.x - pa.x)*(pc.y - pa.y) - (pc.x - pa.x)*(pb.y - pa.y);

    n = glm::vec3(na, nb, nc);
    n = glm::normalize(n);
    glm::vec3 tmin,tmax;
    tmin.x=std::min(pa.x,std::min(pb.x,pc.x)) - epslion;
    tmin.y=std::min(pa.y,std::min(pb.y,pc.y)) - epslion;
    tmin.z=std::min(pa.z,std::min(pb.z,pc.z)) - epslion;

    tmax.x=std::max(pa.x,std::max(pb.x,pc.x)) + epslion;
    tmax.y=std::max(pa.y,std::max(pb.y,pc.y)) + epslion;
    tmax.z=std::max(pa.z,std::max(pb.z,pc.z)) + epslion;
    this->aabbBox._min=tmin;
    this->aabbBox._max=tmax;
}

glm::vec3 triangle::normal(const glm::vec3 &surPos)
{
    return glm::normalize(n);
}

glm::vec3 triangle::reflect(const glm::vec3 &inDirec, const glm::vec3 &normal)
{
    return glm::normalize(inDirec - 2 * glm::dot(inDirec, normal)*normal);
}

bool triangle::boxHit(const ray &r, float minT, float maxT)
{
    return aabbBox.hit(r,minT,maxT);
}

void triangle::setTranslate(const glm::vec3 &trans)
{
    glm::mat4 translate(1.0f);
    translate = glm::translate(translate, trans);
    modelMatrix = translate * modelMatrix;
    invModelMatrix = glm::inverse(modelMatrix);
}

void triangle::setRotate(const glm::vec3 &rotateAxis, float angle)
{
    glm::mat4 rotate(1.0f);
    rotate = glm::rotate(rotate, glm::radians(angle), rotateAxis);
    modelMatrix = rotate * modelMatrix;
    invModelMatrix = glm::inverse(modelMatrix);

    n = glm::vec3(rotate * glm::vec4(n, 1.0));
}

void triangle::setUniformScale(float s)
{
    glm::mat4 scale(1.0f);
    scale = glm::scale(scale, glm::vec3(s));
    modelMatrix = scale * modelMatrix;
    invModelMatrix = glm::inverse(modelMatrix);
}

void triangle::setNonUniformScale(const glm::vec3 &s)
{
    glm::mat4 scale(1.0f);
    scale = glm::scale(scale, s);
    modelMatrix = scale * modelMatrix;
    invModelMatrix = glm::inverse(modelMatrix);

    //need to update normal
    glm::mat4 G = glm::transpose(invModelMatrix);
    n = glm::vec3(G * glm::vec4(n, 0.0));
}

void triangle::setModelMatrix(const glm::mat4 &m)
{
    modelMatrix = m;
    invModelMatrix = glm::inverse(modelMatrix);
}

void triangle::handleMatrix()
{
    pa = glm::vec3(modelMatrix * glm::vec4(pa, 1.0));
    pb = glm::vec3(modelMatrix * glm::vec4(pb, 1.0));
    pc = glm::vec3(modelMatrix * glm::vec4(pc, 1.0));

    this->aabbBox._min.x = std::min(std::min(pa.x, pb.x), pc.x) - epslion;
    this->aabbBox._min.y = std::min(std::min(pa.y, pb.y), pc.y) - epslion;
    this->aabbBox._min.z = std::min(std::min(pa.z, pb.z), pc.z) - epslion;

    this->aabbBox._max.x = std::max(std::max(pa.x, pb.x), pc.x) + epslion;
    this->aabbBox._max.y = std::max(std::max(pa.y, pb.y), pc.y) + epslion;
    this->aabbBox._max.z = std::max(std::max(pa.z, pb.z), pc.z) + epslion;
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
        h.t = t;
        h.u = u;
        h.v = v;
        h.hitPos = r.pos + t * r.direc;

        h.hitNormal = this->normal(h.hitPos);
        h.hitReflect = reflect(r.direc, h.hitNormal);
        h.hitOutDirec = directionGenerator::getInstance().generate(h.hitPos, h.hitNormal);

        //for refract's calculate
        if(mat->type == materialType::dielectrics)
        {
            glm::vec3 outNormal;
            float niOverNt;
            float reflectProb;
            float cosine;

            if(glm::dot(r.direc, h.hitNormal) > 0)
            {
                outNormal = - h.hitNormal;
                niOverNt = mat->niOverNt;
                cosine = niOverNt * glm::dot(r.direc, h.hitNormal);
            }
            else
            {
                outNormal = h.hitNormal;
                niOverNt = 1.0 / mat->niOverNt;
                cosine = -niOverNt * glm::dot(r.direc, h.hitNormal);
            }

            if(refract(r.direc, outNormal, niOverNt, h.hitRefract))
            {
                reflectProb = schlick(cosine, niOverNt);
            }
            else
            {
                h.hitRefract = h.hitReflect;
                reflectProb = 1.0;
            }

            //since we could only pick reflect ray or refrac ray, thus we random chose
            if(drand48() < reflectProb)
                h.hitOutDirec = h.hitReflect;
            else
                h.hitOutDirec = h.hitRefract;

            h.hitOutDirec = h.hitRefract;
        }

        h.hitPdf = directionGenerator::getInstance().value(h.hitOutDirec);
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

std::vector<std::vector<float>> triangle::getModelLinesAndColors()
{
    std::vector<float> verts;
    std::vector<float> colors;

    //first line
    pushData(verts, pa);
    pushData(verts, pb);
    pushData(colors, this->mat->tex->baseColor(0, 0, glm::vec3(0)));
    pushData(colors, this->mat->tex->baseColor(0, 0, glm::vec3(0)));

    //second line
    pushData(verts, pb);
    pushData(verts, pc);
    pushData(colors, this->mat->tex->baseColor(0, 0, glm::vec3(0)));
    pushData(colors, this->mat->tex->baseColor(0, 0, glm::vec3(0)));

    //third line
    pushData(verts, pc);
    pushData(verts, pa);
    pushData(colors, this->mat->tex->baseColor(0, 0, glm::vec3(0)));
    pushData(colors, this->mat->tex->baseColor(0, 0, glm::vec3(0)));

    return {verts, colors};
}

void triangle::pushData(std::vector<float> &v, const glm::vec3 &d)
{
    v.push_back(d.x);
    v.push_back(d.y);
    v.push_back(d.z);
}

