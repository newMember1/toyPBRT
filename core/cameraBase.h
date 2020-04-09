#ifndef CAMERABASE_H
#define CAMERABASE_H
#include"./baseStructure.h"

class cameraBase
{
public:
    cameraBase(glm::vec3 camPos,float _canvasZ,int _width,int _height)
    {
        position=camPos;
        this->canvasZ=_canvasZ;
        this->width=_width;
        this->height=_height;
        this->ratio=1.0*width/height;
    }

    virtual ~cameraBase(){}

    virtual void emitRay(float x,float y,ray&r)
    {
        float dX=2.0/width*ratio;
        float dY=2.0/height;

        float rX=-1.0*ratio+dX*x;
        float rY=1.0-dY*y;

        r.pos=this->position;
        r.direc=glm::normalize(glm::vec3(rX,rY,canvasZ)-r.pos);
    }

    glm::vec3 position;
    int width;
    int height;
    float canvasZ;
    float ratio;
    glm::vec3 origin;
};

#endif // CAMERABASE_H
