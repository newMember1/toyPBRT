/*
 * interface
 * may extend to different brdf matrial
*/
#ifndef MATERIALBASE_H
#define MATERIALBASE_H
#include<memory>
#include"baseStructure.h"
#include"texture.h"
#include<iostream>

class materialBase
{
public:
    materialBase(std::shared_ptr<texture> _tex)
    {
        this->tex=_tex;
    }
    virtual ~materialBase(){}
    virtual glm::vec3 albedo(const hitRecord &hitRec,const glm::vec3 & inDirec,const glm::vec3 & outDirec)=0;
    bool isLight=false;
    std::shared_ptr<texture> tex=nullptr;
};

#endif // MATERIALBASE_H