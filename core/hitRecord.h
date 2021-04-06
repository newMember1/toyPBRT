#ifndef HITRECORD_H
#define HITRECORD_H
#include "baseStructure.h"

class materialBase;
class primitiveBase;

class hitRecord
{
public:
    hitRecord() {}

    float t = 1e6;
    float u,v;
    float hitPdf = 1;
    float hitRoughnessX = 0;
    float hitRoughnessY = 0;
    glm::vec3 hitPos;
    glm::vec3 hitNormal;
	glm::vec3 hitInDirec;
    glm::vec3 hitOutDirec;
    glm::vec3 hitReflect;
    glm::vec3 hitRefract;

    //a material pointer and anisotropy attributes
    //default xAxis and yAxis
    glm::vec3 xAxis;
    glm::vec3 yAxis;
    std::shared_ptr<materialBase> matPtr;
	glm::mat4 invModel;
};

#endif // HITRECORD_H
