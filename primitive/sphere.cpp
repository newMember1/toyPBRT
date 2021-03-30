#include "sphere.h"
#include"./core/baseStructure.h"

extern bool debugFlag;

sphere::sphere(float _r,glm::vec3 _center,std::shared_ptr<materialBase> _mat)
    :primitiveBase(_mat)
{
    pType = primitiveType::sphere;
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
        float t = (-b - sqrt(dis)) / 2.0 / a;
        if(t>minT && t<maxT && t<h.t)
        {
            h.t = t;
            h.hitPos = r.pos + t * r.direc;

            h.hitNormal = this->normal(h.hitPos);
            h.hitReflect = reflect(r.direc, normal(h.hitPos));
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
}

glm::vec3 sphere::normal(const glm::vec3 &surPos)
{
    return glm::normalize(surPos-this->center);
}

glm::vec3 sphere::reflect(const glm::vec3 &inDirec, const glm::vec3 &normal)
{
    return glm::normalize(inDirec - 2 * glm::dot(inDirec, normal)*normal);
}

std::vector<std::vector<float>> sphere::getModelLinesAndColors()
{
    createFrame();
    return {verts, colors};
}

bool sphere::boxHit(const ray &r, float minT, float maxT)
{
    return aabbBox.hit(r,minT,maxT);
}

void sphere::createFrame()
{
    if(verts.size() > 0)
        return;

    float XSEGMENT = 64;
    float YSEGMENT = 64;
    for(int j = 0; j < YSEGMENT; ++j)
        for(int i = 0; i < XSEGMENT; ++i)
        {
            float x = (float)(i) / (float)(XSEGMENT);
            float y = (float)(j) / (float)(YSEGMENT);
            float xPos = cos(x * 2.0 * PI) * sin(y * PI) * radius + center.x;
            float yPos = cos(y * PI) * radius + center.y;
            float zPos = sin(x * 2.0 * PI) * sin(y * PI) * radius + center.z;

            pushData(verts, xPos, yPos, zPos);
            pushData(colors, this->mat->tex->baseColor(0, 0, glm::vec3(0), glm::vec3(0)));
        }
}

void sphere::pushData(std::vector<float> &v, const glm::vec3 &d)
{
    v.push_back(d.x);
    v.push_back(d.y);
    v.push_back(d.z);
}

void sphere::pushData(std::vector<float> &v, float a, float b, float c)
{
    v.push_back(a);
    v.push_back(b);
    v.push_back(c);
}

void sphere::setTranslate(const glm::vec3 &trans)
{
    glm::mat4 translate(1.0f);
    translate = glm::translate(translate, trans);
    modelMatrix = translate * modelMatrix;
    invModelMatrix = glm::inverse(modelMatrix);
}

void sphere::setRotate(const glm::vec3 &rotateAxis, float angle)
{
    glm::mat4 rotate;
    rotate = glm::rotate(rotate, glm::radians(angle), rotateAxis);
    modelMatrix = rotate * modelMatrix;
    invModelMatrix = glm::inverse(modelMatrix);
}

void sphere::setUniformScale(float s)
{
    radius *= s;
}

void sphere::setNonUniformScale(const glm::vec3 &s)
{
    std::cout<<"sphere setNonUniformScale not done..."<<std::endl;
}

void sphere::setModelMatrix(const glm::mat4 &m)
{
    modelMatrix = m;
    invModelMatrix = glm::inverse(modelMatrix);
}

void sphere::handleMatrix()
{
    center = glm::vec3(modelMatrix * glm::vec4(center, 1.0));
    this->aabbBox=aabb(this->center-glm::vec3(radius),
                       this->center+glm::vec3(radius));
}

glm::vec3 sphere::getCenter()
{
    return center;
}
