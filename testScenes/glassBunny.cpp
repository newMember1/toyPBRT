#include "glassBunny.h"

glassBunny::glassBunny()
{
    createTextures();
    createMatrials();
    createObjects();
}

void glassBunny::createTextures()
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

    auto white = std::make_shared<constTexture>(glm::vec3(0.73f));
    auto whiteBase = std::dynamic_pointer_cast<texture>(std::move(white));
    textures["whiteTex"] = whiteBase;

    auto light = std::make_shared<constTexture>(glm::vec3(15.0f));
    auto lightBase = std::dynamic_pointer_cast<texture>(std::move(light));
    textures["lightTex"] = lightBase;
}

void glassBunny::createMatrials()
{
    auto goldBase = getTexture("goldTex");
    auto redBase = getTexture("redTex");
    auto greenBase = getTexture("greenTex");
    auto whiteBase = getTexture("whiteTex");
    auto lightBase = getTexture("lightTex");

    auto goldMat = std::make_shared<dielectricMaterial>(goldBase, 1.5);
    auto goldMatBase = std::dynamic_pointer_cast<materialBase>(std::move(goldMat));
    materials["goldMat"] = goldMatBase;

    auto redMat = std::make_shared<simpleMaterial>(redBase);
    auto redMatBase = std::dynamic_pointer_cast<materialBase>(std::move(redMat));
    materials["redMat"] = redMatBase;

    auto greenMat = std::make_shared<simpleMaterial>(greenBase);
    auto greenMatBase = std::dynamic_pointer_cast<materialBase>(std::move(greenMat));
    materials["greenMat"] = greenMatBase;

    auto whiteMat = std::make_shared<simpleMaterial>(whiteBase);
    auto whiteMatBase = std::dynamic_pointer_cast<materialBase>(std::move(whiteMat));
    materials["whiteMat"] = whiteMatBase;

    auto lightMat = std::make_shared<simpleMaterial>(lightBase);
    lightMat->isLight = true;
    auto lightMatBase = std::dynamic_pointer_cast<materialBase>(std::move(lightMat));
    materials["lightMat"] = lightMatBase;
}

void glassBunny::createObjects()
{
    auto goldMat = getMatrial("goldMat");
    auto redMat = getMatrial("redMat");
    auto greenMat = getMatrial("greenMat");
    auto whiteMat = getMatrial("whiteMat");
    auto lightMat = getMatrial("lightMat");

    auto left = std::make_shared<rectangle>(glm::vec3(0, 0, 555), -z, y, 555, 555, redMat);
    auto leftBase = std::dynamic_pointer_cast<primitiveBase>(std::move(left));
    objects["left"] = leftBase;

    auto right = std::make_shared<rectangle>(glm::vec3(555, 0, 0), z, y, 555, 555, greenMat);
    auto rightBase = std::dynamic_pointer_cast<primitiveBase>(std::move(right));
    objects["right"] = rightBase;

    auto up = std::make_shared<rectangle>(glm::vec3(555, 555, 555), -x, -z, 555, 555, whiteMat);
    auto upBase = std::dynamic_pointer_cast<primitiveBase>(std::move(up));
    objects["up"] = upBase;

    auto bottom = std::make_shared<rectangle>(glm::vec3(555, 0, 0), -x, z, 555, 555, whiteMat);
    auto bottomBase = std::dynamic_pointer_cast<primitiveBase>(std::move(bottom));
    objects["bottom"] = bottomBase;

    auto near = std::make_shared<rectangle>(glm::vec3(555, 0, 555), -x, y, 555, 555, whiteMat);
    auto nearBase = std::dynamic_pointer_cast<primitiveBase>(std::move(near));
    objects["near"] = nearBase;

    //load light
    auto upLight = std::make_shared<rectangle>(glm::vec3(213, 554, 227), x, z, 130, 105, lightMat);
    auto upLightBase = std::dynamic_pointer_cast<primitiveBase>(upLight);
    objects["upLight"] = upLightBase;

    //bunny
    auto bunny = std::make_shared<model>("/home/zdxiao/Desktop/resources/models/bunny/bunny.obj", goldMat);
    bunny->setUniformScale(1000);
    bunny->setTranslate(glm::vec3(300, 125, 227.5));
    auto bunnyBase = std::dynamic_pointer_cast<primitiveBase>(bunny);
    objects["bunny"] = bunnyBase;
}

using std::cout;
using std::endl;
shared_ptr<texture> glassBunny::getTexture(string name)
{
    if(textures.find(name) != textures.end())
        return textures[name];
    else
    {
        cout<<"texture: "<<name<<" can't found";
        return NULL;
    }
}

shared_ptr<materialBase> glassBunny::getMatrial(string name)
{
    if(materials.find(name) != materials.end())
        return materials[name];
    else
    {
        cout<<"material: "<<name<<" can't found";
        return NULL;
    }
}

shared_ptr<primitiveBase> glassBunny::getObject(string name)
{
    if(objects.find(name) != objects.end())
        return objects[name];
    else
    {
        cout<<"object: "<<name<<" can't found";
        return NULL;
    }
}

glassBunny & glassBunny::getInstance()
{
    static glassBunny instance;
    return instance;
}

unordered_map<string, shared_ptr<primitiveBase>> glassBunny::getAllObjects()
{
    return objects;
}
