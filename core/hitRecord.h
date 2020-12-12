#ifndef HITRECORD_H
#define HITRECORD_H
#include "baseStructure.h"

class materialBase;

class hitRecord
{
public:
    hitRecord() {}

    float t = 1e6;
    float u,v;
    float hitPdf = 1;
    glm::vec3 hitPos;
    glm::vec3 hitNormal;
    glm::vec3 hitOutDirec;
    glm::vec3 hitReflect;
    glm::vec3 hitRefract;

    //a material pointer and anisotropy attributes
    //default xAxis and yAxis
    glm::vec3 xAxis=glm::vec3(0,1,0);
    glm::vec3 yAxis=glm::vec3(1,0,0);
    std::shared_ptr<materialBase> matPtr;
};

#endif // HITRECORD_H
