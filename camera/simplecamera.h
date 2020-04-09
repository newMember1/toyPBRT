#ifndef SIMPLECAMERA_H
#define SIMPLECAMERA_H
#include"../core/cameraBase.h"

class simpleCamera :public cameraBase
{
public:
    simpleCamera(glm::vec3 camPos,float _canvasZ,int _width,int _height):
        cameraBase(camPos,_canvasZ,_width,_height){}
    void emitRay(float x, float y, ray &r) override
    {
        //add noise to make pic edges more smooth
        float dX=2.0/width*ratio;
        float dY=2.0/height;

        float rX=-1.0*ratio+dX*(x+drand48());
        float rY=1.0-dY*(y+drand48());

        r.pos=this->position;
        r.direc=glm::normalize(glm::vec3(rX,rY,canvasZ)-r.pos);
    }
    /*to do... need to handle viewMatrix when combine with ratio*/
};

#endif // SIMPLECAMERA_H
