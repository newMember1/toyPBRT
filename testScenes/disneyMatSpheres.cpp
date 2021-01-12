#include "disneyMatSpheres.h"
#include <string>

disneyMatSpheres::disneyMatSpheres()
{
    createTextures();
    createMatrials();
    createObjects();
}

void disneyMatSpheres::createTextures()
{
    auto red = std::make_shared<constTexture>(glm::vec3(0.5, 0, 0));
    auto redBase = std::dynamic_pointer_cast<texture>(std::move(red));
    textures["redTex"] = redBase;

    for(int i = 0; i < lightColors.size(); ++i)
    {
        auto lc = lightColors[i];
        auto lcolor = std::make_shared<constTexture>(lc);
        auto lcolorBase = std::dynamic_pointer_cast<texture>(std::move(lcolor));
        textures["lightColor" + std::to_string(i)] = lcolorBase;
    }
}

void disneyMatSpheres::createMatrials()
{
    auto redBase = getTexture("redTex");
    for(int i = 0; i < lightColors.size(); ++i)
    {
        auto texBase = getTexture("lightColor" + std::to_string(i));
        auto lightMat = std::make_shared<simpleMaterial>(texBase);
        lightMat->isLight = true;
        auto lightMatase = std::dynamic_pointer_cast<materialBase>(lightMat);
        materials["lightMaterial" + std::to_string(i)] = lightMatase;
    }

    for(int i = 0; i < nrRows; ++i)
    {
        float metallic = (float)(i) / (float)(nrRows) + 0.04;
        for(int j = 0; j < nrColumns; ++j)
        {
            float roughness = (float)(j) / (float)(nrColumns) + 0.04;
            auto redMat = std::make_shared<basicBRDFMaterial>(redBase);
            redMat->setMetallic(metallic);
            redMat->setRoughness(roughness);
            auto redMatBase = std::dynamic_pointer_cast<materialBase>(std::move(redMat));
            materials["sphere" + std::to_string(i * nrColumns + j)] = redMatBase;
        }
    }
}

void disneyMatSpheres::createObjects()
{
    for(int i = 0; i < lightColors.size(); ++i)
    {
        auto mat = getMatrial("lightMaterial" + std::to_string(i));
        auto ball = std::make_shared<sphere>(0.1, lightPositions[i], mat);
        auto ballBase = std::dynamic_pointer_cast<primitiveBase>(ball);
        objects["lightSphere" + std::to_string(i)] = ballBase;
    }

    for(int i = 0; i < nrRows; ++i)
    {
        for(int j = 0; j < nrColumns; ++j)
        {
            float x = -0.8 + 1.6 / 6 * j;
            float y = -0.8 + 1.6 / 6 * i;
            float z = -0.5;
            auto mat = getMatrial("sphere" + std::to_string(i * nrColumns + j));
            auto ball = std::make_shared<sphere>(0.12, glm::vec3(x, y, z), mat);
            auto ballBase = std::dynamic_pointer_cast<primitiveBase>(ball);
            objects["redSphere" + std::to_string(i * nrColumns + j)] = ballBase;
        }
    }
}

using std::cout;
using std::endl;
shared_ptr<texture> disneyMatSpheres::getTexture(string name)
{
    if(textures.find(name) != textures.end())
        return textures[name];
    else
    {
        cout<<"texture: "<<name<<" can't found";
        return NULL;
    }
}

shared_ptr<materialBase> disneyMatSpheres::getMatrial(string name)
{
    if(materials.find(name) != materials.end())
        return materials[name];
    else
    {
        cout<<"material: "<<name<<" can't found";
        return NULL;
    }
}

shared_ptr<primitiveBase> disneyMatSpheres::getObject(string name)
{
    if(objects.find(name) != objects.end())
        return objects[name];
    else
    {
        cout<<"object: "<<name<<" can't found";
        return NULL;
    }
}

disneyMatSpheres & disneyMatSpheres::getInstance()
{
    static disneyMatSpheres instance;
    return instance;
}

unordered_map<string, shared_ptr<primitiveBase>> disneyMatSpheres::getAllObjects()
{
    return objects;
}

