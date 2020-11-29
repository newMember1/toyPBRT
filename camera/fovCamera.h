#ifndef FOVCAMERA_H
#define FOVCAMERA_H
#include "../core/cameraBase.h"

class fovCamera :public cameraBase
{
public:
    fovCamera(glm::vec3 _lookFrom, glm::vec3 _lookAt, float degreeFov, float aspect, int _width, int _height):
        cameraBase(_lookFrom, _lookAt, _width, _height)
    {
        //nx and ny means how much pixels int each row and column
        //can be used to calculate ratio
        //aspect are used to calculate how much width and height pixels we can see
        float theta = degreeFov * PI / 180;
        float tanTheta = tan(theta / 2);

        float dis = glm::distance(_lookFrom, _lookAt);
        H = tanTheta * dis;
        W = H * aspect;
    }

    void emitRay(float x, float y, ray &r) override
    {
        //add noise to make pic edges more smooth
        float dX = 2.0 * W / width * ratio;
        float dY = 2.0 * H / height;

        float rX = -1.0 * W + dX * (x + drand48());
        float rY = -1.0 * H + dY * (y + drand48());

        r.pos=this->lookFrom;
        r.direc=glm::normalize((lookTo+u*rX+v*rY)-r.pos);
    }

    float W = 1.0;
    float H = 1.0;
};

#endif // FOVCAMERA_H
