#ifndef SIMPLECAMERA_H
#define SIMPLECAMERA_H
#include"../core/cameraBase.h"

class simpleCamera :public cameraBase
{
public:
    simpleCamera(glm::vec3 lookFrom,glm::vec3 lookTo,int _width,int _height):
        cameraBase(lookFrom,lookTo,_width,_height){}
    void emitRay(float x, float y, ray &r) override
    {
        //add noise to make pic edges more smooth
        float dX=2.0/width*ratio;
        float dY=2.0/height;

        float rX=-1.0*ratio+dX*(x+drand48());
        float rY=1.0-dY*(y+drand48());

        r.pos=this->lookFrom;
        r.direc=glm::normalize((lookTo-u*rX+v*rY)-r.pos);
    }
};

#endif // SIMPLECAMERA_H
