#include "spherescene.h"

sphereScene::sphereScene()
{
    createTextures();
    createMatrials();
    createObjects();
}

void sphereScene::createTextures()
{
    auto red = std::make_shared<constTexture>(glm::vec3(0.65, 0.05, 0.05));
    auto redBase = std::dynamic_pointer_cast<texture>(std::move(red));
    textures["redTex"] = redBase;

    auto green = std::make_shared<constTexture>(glm::vec3(0.12, 0.45, 0.15));
    auto greenBase = std::dynamic_pointer_cast<texture>(std::move(green));
    textures["greenTex"] = greenBase;

    auto blue = std::make_shared<constTexture>(glm::vec3(0, 0, 0.99f));
    auto blueBase = std::dynamic_pointer_cast<texture>(std::move(blue));
    textures["blueTex"] = blueBase;

    auto white = std::make_shared<constTexture>(glm::vec3(0.73f));
    auto whiteBase = std::dynamic_pointer_cast<texture>(std::move(white));
    textures["whiteTex"] = whiteBase;

    auto light = std::make_shared<constTexture>(glm::vec3(15.0f));
    auto lightBase = std::dynamic_pointer_cast<texture>(std::move(light));
    textures["lightTex"] = lightBase;

    auto check = std::make_shared<checkTexture>(glm::vec3(1, 0.05, 0.05), glm::vec3(0.12, 1, 0.15));
    auto checkBase = std::dynamic_pointer_cast<texture>(std::move(check));
    textures["checkTex"] = checkBase;
}

void sphereScene::createMatrials()
{
    auto redBase = getTexture("redTex");
    auto greenBase = getTexture("greenTex");
    auto blueBase = getTexture("blueTex");
    auto whiteBase = getTexture("whiteTex");
    auto lightBase = getTexture("lightTex");
    auto checkBase = getTexture("checkTex");

    auto redMat = std::make_shared<simpleMaterial>(redBase);
    auto redMatBase = std::dynamic_pointer_cast<materialBase>(std::move(redMat));
    materials["redMat"] = redMatBase;

    auto greenMat = std::make_shared<simpleMaterial>(greenBase);
    auto greenMatBase = std::dynamic_pointer_cast<materialBase>(std::move(greenMat));
    materials["greenMat"] = greenMatBase;

    auto blueMat = std::make_shared<simpleMaterial>(blueBase);
    auto blueMatBase = std::dynamic_pointer_cast<materialBase>(std::move(blueMat));
    materials["blueMat"] = blueMatBase;

    auto whiteMat = std::make_shared<simpleMaterial>(whiteBase);
    auto whiteMatBase = std::dynamic_pointer_cast<materialBase>(std::move(whiteMat));
    materials["whiteMat"] = whiteMatBase;

    auto lightMat = std::make_shared<simpleMaterial>(lightBase);
    lightMat->isLight = true;
    auto lightMatBase = std::dynamic_pointer_cast<materialBase>(std::move(lightMat));
    materials["lightMat"] = lightMatBase;

    auto glassMat = std::make_shared<dielectricMaterial>(whiteBase, 1.5);
    auto glassMatBase = std::dynamic_pointer_cast<materialBase>(glassMat);
    materials["glassMat"] = glassMatBase;

    auto checkMat = std::make_shared<simpleMaterial>(checkBase);
    auto checkMatBase = std::dynamic_pointer_cast<materialBase>(checkMat);
    materials["checkMat"] = checkMatBase;
}

void sphereScene::createObjects()
{
    auto greenMat = getMatrial("greenMat");
    auto redMat = getMatrial("redMat");
    auto blueMat = getMatrial("blueMat");
    auto whiteMat = getMatrial("whiteMat");
    auto lightMat = getMatrial("lightMat");
    auto glassMat = getMatrial("glassMat");
    auto checkMat = getMatrial("checkMat");

    //load the big box
    auto left = std::make_shared<rectangle>(glm::vec3(0, 0, 0), y, z, 555, 555, redMat);
    auto leftBase = std::dynamic_pointer_cast<primitiveBase>(std::move(left));
    objects["left"] = leftBase;

    auto right = std::make_shared<rectangle>(glm::vec3(555, 0, 555), y, -z, 555, 555, checkMat);
    auto rightBase = std::dynamic_pointer_cast<primitiveBase>(std::move(right));
    objects["right"] = rightBase;

    auto up = std::make_shared<rectangle>(glm::vec3(0, 555, 0), x, z, 555, 555, whiteMat);
    auto upBase = std::dynamic_pointer_cast<primitiveBase>(std::move(up));
    objects["up"] = upBase;

    auto bottom = std::make_shared<rectangle>(glm::vec3(0, 0, 0), z, x, 555, 555, whiteMat);
    auto bottomBase = std::dynamic_pointer_cast<primitiveBase>(std::move(bottom));
    objects["bottom"] = bottomBase;

    auto near = std::make_shared<rectangle>(glm::vec3(555, 0, 555), -x, y, 555, 555, greenMat);
    auto nearBase = std::dynamic_pointer_cast<primitiveBase>(std::move(near));
    objects["near"] = nearBase;

    //load light
    auto upLight = std::make_shared<rectangle>(glm::vec3(213, 554, 227), x, z, 130, 105, lightMat);
    auto upLightBase = std::dynamic_pointer_cast<primitiveBase>(upLight);
    objects["upLight"] = upLightBase;

    //load sphere
    auto ball = std::make_shared<sphere>(180, glm::vec3(277, 277, 277), glassMat);
    auto ballBase = std::dynamic_pointer_cast<primitiveBase>(ball);
    objects["ball"] = ballBase;
}

using std::cout;
using std::endl;
shared_ptr<texture> sphereScene::getTexture(string name)
{
    if(textures.find(name) != textures.end())
        return textures[name];
    else
    {
        cout<<"texture: "<<name<<" can't found";
        return NULL;
    }
}

shared_ptr<materialBase> sphereScene::getMatrial(string name)
{
    if(materials.find(name) != materials.end())
        return materials[name];
    else
    {
        cout<<"material: "<<name<<" can't found";
        return NULL;
    }
}

shared_ptr<primitiveBase> sphereScene::getObject(string name)
{
    if(objects.find(name) != objects.end())
        return objects[name];
    else
    {
        cout<<"object: "<<name<<" can't found";
        return NULL;
    }
}

sphereScene & sphereScene::getInstance()
{
    static sphereScene instance;
    return instance;
}

unordered_map<string, shared_ptr<primitiveBase>> sphereScene::getAllObjects()
{
    return objects;
}

