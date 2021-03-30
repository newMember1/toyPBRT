/*
 * can get baseColor if given coordinate
*/
#ifndef TEXTURE_H
#define TEXTURE_H

#include "baseStructure.h"
#include "./3rdparty/stb_image.h"

class texture
{
public:
    virtual ~texture(){}
    virtual glm::vec3 baseColor(float u, float v, const glm::vec3 &pos, const glm::vec3 &direc)
    {
        std::cout<<"Warning: you are using base class's baseColorImpl function, please check your code!"<<std::endl;
        return baseColorImpl(u, v, pos);
    }

    virtual glm::vec3 baseColorImpl(float u,float v,const glm::vec3 &pos)
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
     constTexture(const glm::vec3 &c)
     {
         color.x=c.x;
         color.y=c.y;
         color.z=c.z;
     }

     constTexture(constTexture * t)
     {
         color=t->color;
     };

private:
     virtual glm::vec3 baseColorImpl(float u, float v, const glm::vec3 &pos) override
     {
         return color;
     }
     glm::vec3 color{0};
};

class imageTexture:public texture
{
public:
    imageTexture(const char * path)
    {
        stbi_set_flip_vertically_on_load(true);
        data = stbi_load(path, &nx, &ny, &nc, 0);
    }

    virtual glm::vec3 baseColor(float u, float v, const glm::vec3 &pos, const glm::vec3 &direc) override
    {
        return baseColorImpl(u, v, pos);
    }
private:
    virtual glm::vec3 baseColorImpl(float u, float v, const glm::vec3 &pos) override
    {
        int i = u * nx;
        int j = (1 - v) * ny - 0.001;
        if(i < 0) i = 0;
        if(j < 0) j = 0;
        if(i > nx - 1) i = nx - 1;
        if(j > ny - 1) j = ny - 1;

        if(nc == 3)
        {
            float r = int(data[3 * i + 3 * nx * j]) / 255.0;
            float g = int(data[3 * i + 3 * nx * j + 1]) / 255.0;
            float b = int(data[3 * i + 3 * nx * j + 2]) / 255.0;
            return glm::vec3(r, g, b);
        }
        else if(nc == 4)
        {
            float r = int(data[4 * i + 4 * nx * j]) / 255.0;
            float g = int(data[4 * i + 4 * nx * j + 1]) / 255.0;
            float b = int(data[4 * i + 4 * nx * j + 2]) / 255.0;
            return glm::vec3(r, g, b);
        }
        else
        {
            std::cout<<"Warning: picture channel is not as standard, please check!"<<std::endl;
            return glm::vec3(0.0f);
        }
    }
    unsigned char *data;
    int nx,ny,nc;
};

class envImageTexture:public texture
{
public:
    envImageTexture(const char * path)
    {
        stbi_set_flip_vertically_on_load(true);
        data = stbi_load(path, &nx, &ny, &nc, 0);
    }

    virtual glm::vec3 baseColor(float u, float v, const glm::vec3 &pos, const glm::vec3 &direc) override
    {
        return baseColorImpl(u, v, direc);
    }
private:
    glm::vec3 baseColorImpl(float u, float v, const glm::vec3 &direc) override
    {
        glm::vec2 uv = sampleSphericalMap(direc);
        u = uv.x;
        v = uv.y;
        int i = u * nx;
        int j = (1 - v) * ny - 0.001;
        if(i < 0) i = 0;
        if(j < 0) j = 0;
        if(i > nx - 1) i = nx - 1;
        if(j > ny - 1) j = ny - 1;

        if(nc == 3)
        {
            float r = int(data[3 * i + 3 * nx * j]) / 255.0;
            float g = int(data[3 * i + 3 * nx * j + 1]) / 255.0;
            float b = int(data[3 * i + 3 * nx * j + 2]) / 255.0;
            return glm::vec3(r, g, b);
        }
        else if(nc == 4)
        {
            float r = int(data[4 * i + 4 * nx * j]) / 255.0;
            float g = int(data[4 * i + 4 * nx * j + 1]) / 255.0;
            float b = int(data[4 * i + 4 * nx * j + 2]) / 255.0;
            return glm::vec3(r, g, b);
        }
        else
        {
            std::cout<<"Warning: picture channel is not as standard, please check!"<<std::endl;
            return glm::vec3(0.0f);
        }
    }

    const glm::vec2 invAtan{0.1591, 0.3183};
    glm::vec2 sampleSphericalMap(const glm::vec3 &v)
    {
        glm::vec2 uv = glm::vec2(glm::atan(v.z, v.x), glm::asin(v.y));
        uv *= invAtan;
        uv += 0.5;
        return uv;
    }
    unsigned char *data;
    int nx,ny,nc;
};

class checkTexture:public texture
{
public:
    checkTexture(const glm::vec3 &a,const glm::vec3 &b):
        colorA(a),colorB(b){}
    virtual glm::vec3 baseColor(float u, float v, const glm::vec3 &pos, const glm::vec3 &direc) override
    {
        return baseColorImpl(u, v, pos);
    }
private:
    virtual glm::vec3 baseColorImpl(float u, float v, const glm::vec3 &pos) override
    {
        float sines=sin(pos.x / 10.0) + sin(pos.y / 10.0) + sin(pos.z / 10.0);
        return glm::vec3(abs(sines));
        if(sines<0)
            return colorA;
        else
            return colorB;
    }
    glm::vec3 colorA;
    glm::vec3 colorB;
};

#endif // TEXTURE_H
