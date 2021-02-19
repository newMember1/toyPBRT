#include "disneymatobjects.h"

disneyMatObjects::disneyMatObjects()
{
    createTextures();
    createMatrials();
    createObjects();
}

void disneyMatObjects::createTextures()
{
    auto gold = std::make_shared<constTexture>(glm::vec3(1.0f, 0.82f, 0.61f));
    auto goldBase = std::dynamic_pointer_cast<texture>(std::move(gold));
    textures["goldTex"] = goldBase;

    auto red = std::make_shared<constTexture>(glm::vec3(0.65, 0.05, 0.05));
    auto redBase = std::dynamic_pointer_cast<texture>(std::move(red));
    textures["redTex"] = redBase;

    auto green = std::make_shared<constTexture>(glm::vec3(0.12, 0.45, 0.15));
    auto greenBase = std::dynamic_pointer_cast<texture>(std::move(green));
    textures["greenTex"] = greenBase;

    auto light = std::make_shared<constTexture>(glm::vec3(1.0f));
    auto lightBase = std::dynamic_pointer_cast<texture>(std::move(light));
    textures["lightTex"] = lightBase;
}

void disneyMatObjects::createMatrials()
{
    auto goldBase = getTexture("goldTex");
    auto redBase = getTexture("redTex");
    auto greenBase = getTexture("greenTex");
    auto lightBase = getTexture("lightTex");

    for (int i = 0; i < 5; ++i)
    {
        float subsurface = (float)i / 5;
        for (int j = 0; j < 5; ++j)
        {
            float roughness = glm::clamp((float)j / 5, 0.05f, 1.0f);
            auto goldMat = std::make_shared<disneyBRDFMaterial>(goldBase);
            goldMat->setRoughness(roughness);
            goldMat->setSubSurface(subsurface);
            auto goldMatBase = std::dynamic_pointer_cast<materialBase>(std::move(goldMat));
            int ind = i * 5 + j;
            materials["goldMat" + std::to_string(ind)] = goldMatBase;
        }
    }

//    auto goldMat = std::make_shared<disneyBRDFMaterial>(goldBase);
//    goldMat->setRoughness(0);
//    goldMat->setSubSurface(0);
//    auto goldMatBase = std::dynamic_pointer_cast<materialBase>(std::move(goldMat));
//    materials["goldMat"] = goldMatBase;

//    auto goldMat1 = std::make_shared<disneyBRDFMaterial>(goldBase);
//    goldMat1->setRoughness(0.5);
//    goldMat1->setSubSurface(0.5);
//    auto goldMatBase1 = std::dynamic_pointer_cast<materialBase>(std::move(goldMat1));
//    materials["goldMat1"] = goldMatBase1;

//    auto goldMat2 = std::make_shared<disneyBRDFMaterial>(goldBase);
//    goldMat2->setRoughness(1);
//    goldMat2->setSubSurface(1);
//    auto goldMatBase2 = std::dynamic_pointer_cast<materialBase>(std::move(goldMat2));
//    materials["goldMat2"] = goldMatBase2;

//    auto redMat = std::make_shared<simpleMaterial>(redBase);
//    auto redMatBase = std::dynamic_pointer_cast<materialBase>(std::move(redMat));
//    materials["redMat"] = redMatBase;

//    auto greenMat = std::make_shared<simpleMaterial>(greenBase);
//    auto greenMatBase = std::dynamic_pointer_cast<materialBase>(std::move(greenMat));
//    materials["greenMat"] = greenMatBase;

//    auto lightMat = std::make_shared<simpleMaterial>(lightBase);
//    lightMat->isLight = true;
//    auto lightMatBase = std::dynamic_pointer_cast<materialBase>(std::move(lightMat));
//    materials["lightMat"] = lightMatBase;
}

void disneyMatObjects::createObjects()
{
    auto lightMat = getMatrial("lightMat");
    auto goldMat = getMatrial("goldMat");
    auto goldMat1 = getMatrial("goldMat1");
    auto goldMat2 = getMatrial("goldMat2");

    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            int ind = i * 5 + j;
            auto goldMat = getMatrial("goldMat" + std::to_string(ind));
            auto rect = std::make_shared<rectangle>(glm::vec3(-1, 1, -1), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1), 2, 2, goldMat);
            auto rectBase = std::dynamic_pointer_cast<primitiveBase>(rect);
            objects[std::to_string(ind)] = rectBase;
        }
    }
//    auto rectLight = std::make_shared<rectangle>(glm::vec3(-1, 1, -1), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1), 2, 2, lightMat);
//    auto rectLightBase = std::dynamic_pointer_cast<primitiveBase>(rectLight);
//    objects["rectLight"] = rectLightBase;
//    //bunny
//    auto bunny = std::make_shared<model>("/home/zdxiao/Desktop/resources/models/bunny/bunny.obj", goldMat);
//    bunny->setUniformScale(5);
//    bunny->setTranslate(glm::vec3(-0.6, -0.5, -0.5));
//    auto bunnyBase = std::dynamic_pointer_cast<primitiveBase>(bunny);
//    objects["bunny"] = bunnyBase;
//    //bunny1
//    auto bunny1 = std::make_shared<model>("/home/zdxiao/Desktop/resources/models/bunny/bunny.obj", goldMat1);
//    bunny1->setUniformScale(5);
//    bunny1->setTranslate(glm::vec3(0.2, -0.5, -0.5));
//    auto bunnyBase1 = std::dynamic_pointer_cast<primitiveBase>(bunny1);
//    objects["bunny1"] = bunnyBase1;
//    //bunny
//    auto bunny2 = std::make_shared<model>("/home/zdxiao/Desktop/resources/models/bunny/bunny.obj", goldMat2);
//    bunny2->setUniformScale(5);
//    bunny2->setTranslate(glm::vec3(1.0, -0.5, -0.5));
//    auto bunnyBase2 = std::dynamic_pointer_cast<primitiveBase>(bunny2);
//    objects["bunny2"] = bunnyBase2;
}

using std::cout;
using std::endl;
shared_ptr<texture> disneyMatObjects::getTexture(string name)
{
    if(textures.find(name) != textures.end())
        return textures[name];
    else
    {
        cout<<"texture: "<<name<<" can't found";
        return NULL;
    }
}

shared_ptr<materialBase> disneyMatObjects::getMatrial(string name)
{
    if(materials.find(name) != materials.end())
        return materials[name];
    else
    {
        cout<<"material: "<<name<<" can't found";
        return NULL;
    }
}

shared_ptr<primitiveBase> disneyMatObjects::getObject(string name)
{
    if(objects.find(name) != objects.end())
        return objects[name];
    else
    {
        cout<<"object: "<<name<<" can't found";
        return NULL;
    }
}

disneyMatObjects & disneyMatObjects::getInstance()
{
    static disneyMatObjects instance;
    return instance;
}

unordered_map<string, shared_ptr<primitiveBase>> disneyMatObjects::getAllObjects()
{
    return objects;
}
