#include "refractcheck.h"
#include <string>
refractCheck::refractCheck()
{
    createTextures();
    createMatrials();
    createObjects();
}

void refractCheck::createTextures()
{
    auto green = std::make_shared<constTexture>(glm::vec3(0.8, 0.8, 0.0));
    auto greenBase = std::dynamic_pointer_cast<textureBase>(std::move(green));
    textures["greenTex"] = greenBase;

    auto blue = std::make_shared<constTexture>(glm::vec3(0.1, 0.2, 0.5));
    auto blueBase = std::dynamic_pointer_cast<textureBase>(std::move(blue));
    textures["blueTex"] = blueBase;
}

void refractCheck::createMatrials()
{
    auto greenBase = getTexture("greenTex");
    auto blueBase = getTexture("blueTex");

    auto greenMat = std::make_shared<simpleMaterial>(greenBase);
    auto greenMatBase = std::dynamic_pointer_cast<materialBase>(std::move(greenMat));
    materials["greenMat"] = greenMatBase;

    auto blueMat = std::make_shared<simpleMaterial>(blueBase);
    auto blueMatBase = std::dynamic_pointer_cast<materialBase>(std::move(blueMat));
    materials["blueMat"] = blueMatBase;

    auto glassMat = std::make_shared<dielectricMaterial>(greenBase, 1.5);
    auto glassMatBase = std::dynamic_pointer_cast<materialBase>(std::move(glassMat));
    materials["glassMat"] = glassMatBase;
}

void refractCheck::createObjects()
{
    auto greenMat = getMatrial("greenMat");
    auto blueMat = getMatrial("blueMat");
    auto glassMat = getMatrial("glassMat");

    auto ball1 = std::make_shared<sphere>(0.5, glm::vec3(0, 0, -1), blueMat);
    auto ball1Base = std::dynamic_pointer_cast<primitiveBase>(ball1);
    objects["ball1"] = ball1Base;

    auto ball2 = std::make_shared<sphere>(100.0f, glm::vec3(0, -100.5, -1), greenMat);
    auto ball2Base = std::dynamic_pointer_cast<primitiveBase>(ball2);
    objects["ball2"] = ball2Base;

    auto ball3 = std::make_shared<sphere>(0.5, glm::vec3(-1, 0, -1), glassMat);
    auto ball3Base = std::dynamic_pointer_cast<primitiveBase>(ball3);
    objects["ball3"] = ball3Base;

    auto ball4 = std::make_shared<sphere>(0.5, glm::vec3(1, 0, -1), greenMat);
    auto ball4Base = std::dynamic_pointer_cast<primitiveBase>(ball4);
    objects["ball4"] = ball4Base;
}

using std::cout;
using std::endl;
shared_ptr<textureBase> refractCheck::getTexture(string name)
{
    if(textures.find(name) != textures.end())
        return textures[name];
    else
    {
        cout<<"texture: "<<name<<" can't found";
        return NULL;
    }
}

shared_ptr<materialBase> refractCheck::getMatrial(string name)
{
    if(materials.find(name) != materials.end())
        return materials[name];
    else
    {
        cout<<"material: "<<name<<" can't found";
        return NULL;
    }
}

shared_ptr<primitiveBase> refractCheck::getObject(string name)
{
    if(objects.find(name) != objects.end())
        return objects[name];
    else
    {
        cout<<"object: "<<name<<" can't found";
        return NULL;
    }
}

refractCheck & refractCheck::getInstance()
{
    static refractCheck instance;
    return instance;
}

unordered_map<string, shared_ptr<primitiveBase>> refractCheck::getAllObjects()
{
    return objects;
}
