/*
 * interface
 * may extend to different brdf matrial
*/
#ifndef MATERIALBASE_H
#define MATERIALBASE_H
#include <memory>
#include "baseStructure.h"
#include "hitRecord.h"
#include "texture.h"
#include <iostream>

class materialBase
{
public:
    materialBase(std::shared_ptr<textureBase> _tex)
    {
        this->tex=_tex;
    }
    virtual ~materialBase(){}
    virtual glm::vec3 albedo(const hitRecord &hitRec,const glm::vec3 & lightDirec,const glm::vec3 & eyeDirec)=0;

    bool isLight=false;
    std::shared_ptr<textureBase> tex=nullptr;

    float niOverNt = 1.0;
    enum materialType type;
};

#endif // MATERIALBASE_H
