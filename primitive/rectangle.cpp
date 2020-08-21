#include "rectangle.h"

extern bool debugFlag;

rectangle::rectangle(const glm::vec3 & _oriPos, const glm::vec3 & _wDirec, const glm::vec3 & _hDirec, float _w, float _h, std::shared_ptr<materialBase> _mat)
    :primitiveBase(_mat)
{
    this->oriPos = _oriPos;
    this->wDirec = _wDirec;
    this->hDirec = _hDirec;
    this->w = _w;
    this->h = _h;

    this->pa = oriPos;
    this->pb = oriPos + glm::normalize(wDirec) * w;
    this->pd = oriPos + glm::normalize(hDirec) * h;
    this->pc = pb + glm::normalize(hDirec) * h;
    this->N = glm::normalize(glm::cross(wDirec, hDirec));

    glm::vec3 tmin,tmax;

    tmin.x = std::min(std::min(pa.x, pb.x), std::min(pc.x, pd.x)) - epslion;
    tmin.y = std::min(std::min(pa.y, pb.y), std::min(pc.y, pd.y)) - epslion;
    tmin.z = std::min(std::min(pa.z, pb.z), std::min(pc.z, pd.z)) - epslion;

    tmax.x = std::max(std::max(pa.x, pb.x), std::max(pc.x, pd.x)) + epslion;
    tmax.y = std::max(std::max(pa.y, pb.y), std::max(pc.y, pd.y)) + epslion;
    tmax.z = std::max(std::max(pa.z, pb.z), std::max(pc.z, pd.z)) + epslion;

    this->aabbBox._min=tmin;
    this->aabbBox._max=tmax;
}

glm::vec3 rectangle::normal(const glm::vec3 &surPos)
{
    return N;
}

glm::vec3 rectangle::reflect(const glm::vec3 &inDirec, const glm::vec3 &normal)
{
    return glm::normalize(inDirec - 2 * glm::dot(inDirec, normal)*normal);
}

bool rectangle::boxHit(const ray &r, float minT, float maxT)
{
    return aabbBox.hit(r,minT,maxT);
}

bool rectangle::rayRect(ray &r, float &u, float &v, float &t)
{
    //adapt from Moller-triangle algorithm
    //note here we must use pd - pa and pb - pa
    glm::vec3 E1 = pb - pa;
    glm::vec3 E2 = pd - pa;

    glm::vec3 P = glm::cross(r.direc, E2);
    float det = glm::dot(E1, P);

    glm::vec3 T;
    if(det > 0.0f)
        T = r.pos - pa;
    else
    {
        det = -det;
        T = pa - r.pos;
    }
    if(det < epslion)
        return false;

    u = glm::dot(T, P);
    if(u < 0.0f || u > det)
        return false;

    glm::vec3 Q = glm::cross(T, E1);
    v = glm::dot(r.direc, Q);
    if(v < 0.0f || v > det)
        return false;

    float invDet = 1.0 / det;
    t = glm::dot(E2, Q);
    if(t < epslion)
        return false;

    t *= invDet;
    u *= invDet;
    v *= invDet;

    return true;
}

bool rectangle::hit(ray &r, hitRecord &h, float minT, float maxT)
{
    if(!boxHit(r, minT, maxT))
        return false;

    float u, v, t;
    if(rayRect(r, u, v, t) && t > minT && t < maxT && t < h.t)
    {
        h.t = t;
        h.hitPos = r.pos + t * r.direc;
        h.hitNormal = normal(h.hitPos);

        h.hitOutDirec = this->reflect(r.direc, h.hitNormal);
        h.matPtr = this->mat;
        return true;
    }
    else
        return false;
}

void rectangle::handleMatrix()
{
    pa = glm::vec3(glm::vec4(pa, 1.0) * modelMatrix);
    pb = glm::vec3(glm::vec4(pb, 1.0) * modelMatrix);
    pc = glm::vec3(glm::vec4(pc, 1.0) * modelMatrix);
    pd = glm::vec3(glm::vec4(pd, 1.0) * modelMatrix);
}
