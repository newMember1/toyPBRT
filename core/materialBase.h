/*
 * interface
 * may extend to different brdf matrial
*/
#ifndef MATERIALBASE_H
#define MATERIALBASE_H
#include<memory>
#include"baseStructure.h"
#include"texture.h"

class materialBase
{
public:
    virtual ~materialBase(){}
    virtual glm::vec3 albedo(const hitRecord &hitRec) const=0;
    std::unique_ptr<texture*> tex;
};

#endif // MATERIALBASE_H
