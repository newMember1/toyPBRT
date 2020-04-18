#ifndef CAMERABASE_H
#define CAMERABASE_H
#include"./baseStructure.h"
#include<iostream>
class cameraBase
{
public:
    cameraBase(glm::vec3 _lookFrom,glm::vec3 _lookTo,int _width,int _height)
    {
        this->lookFrom=_lookFrom;
        this->lookTo=_lookTo;
        this->width=_width;
        this->height=_height;
        this->ratio=1.0*width/height;

        this->w=glm::normalize(_lookFrom-_lookTo);
        this->u=glm::cross(glm::vec3(0,1,0),w);
        this->v=glm::cross(w,u);
    }

    virtual ~cameraBase(){}

    virtual void emitRay(float x,float y,ray&r)
    {
        float dX=2.0/width*ratio;
        float dY=2.0/height;

        float rX=-1.0*ratio+dX*x;
        float rY=1.0-dY*y;

        r.pos=this->lookFrom;
        r.direc=glm::normalize((lookTo+u*rX+v*rY)-r.pos);
    }


    glm::vec3 lookFrom;
    glm::vec3 lookTo;
    glm::vec3 lookUp;

    glm::vec3 u,v,w;

    int width;
    int height;
    float ratio;
};

#endif // CAMERABASE_H
