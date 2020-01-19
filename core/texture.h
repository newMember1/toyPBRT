/*
 * can get baseColor if given coordinate
*/
#ifndef TEXTURE_H
#define TEXTURE_H
#include"baseStructure.h"
class texture
{
public:
    virtual ~texture(){}
    virtual glm::vec3 baseColor(float u,float v,const glm::vec3 &pos)
    {
        return glm::vec3(0);
    }
    virtual void get_sphere_uv(const glm::vec3 &pos,float &u,float &v)
    {
        glm::vec3 tPos=glm::normalize(pos);
        float phi=atan2(tPos.z,tPos.x);
        float theta=asin(tPos.y);
        u = 1 - (phi + PI) / (2 * PI);
        v = (theta + PI / 2) / PI;
    }
};

class constTexture:public texture
{
public:
     constTexture(const glm::vec3 &c):color(c){}
     virtual glm::vec3 baseColor(float u, float v, const glm::vec3 &pos) override
     {
         return color;
     }
     glm::vec3 color{0};
};

class imageTexture:public texture
{
public:
    imageTexture(unsigned char *pixels,int A,int B,int C):
        data(pixels),nx(A),ny(B),nc(C){}
    virtual glm::vec3 baseColor(float u, float v, const glm::vec3 &pos) override
    {
        int i=u*nx;
        int j=(1-v)*ny-0.001;
        if(i<0) i=0;
        if(j<0) j=0;
        if(i>nx-1) i=nx-1;
        if(j>ny-1) j=ny-1;

        if(nc==3)
        {
            float r = int(data[3 * i + 3 * nx*j]) / 255.0;
            float g = int(data[3 * i + 3 * nx*j + 1]) / 255.0;
            float b = int(data[3 * i + 3 * nx*j + 2]) / 255.0;
            return glm::vec3(r, g, b);
        }
        if(nc==4)
        {
            float r = int(data[4 * i + 4 * nx*j]) / 255.0;
            float g = int(data[4 * i + 4 * nx*j + 1]) / 255.0;
            float b = int(data[4 * i + 4 * nx*j + 2]) / 255.0;
            return glm::vec3(r, g, b);
        }
    }
    unsigned char *data;
    int nx,ny,nc;
};

class checkTexture:public texture
{
    checkTexture(const glm::vec3 &a,const glm::vec3 &b):
        colorA(a),colorB(b){}
    virtual glm::vec3 baseColor(float u, float v, const glm::vec3 &pos) override
    {
        float sines=sin(10 * pos.x)*sin(10 * pos.y)*sin(10 * pos.z);
        if(sines<0)
            return colorA;
        else
            return colorB;
    }
    glm::vec3 colorA;
    glm::vec3 colorB;
};

#endif // TEXTURE_H
