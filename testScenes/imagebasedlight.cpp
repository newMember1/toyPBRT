#include "imagebasedlight.h"

imageBasedLight::imageBasedLight()
{
    createTextures();
    createMatrials();
    createObjects();
}

void imageBasedLight::initResources()
{
    createTextures();
    createMatrials();
    createObjects();
}

shared_ptr<cameraBase> imageBasedLight::getCamera(int width, int height, float fov, float aspect, const glm::vec3 & lookFrom, const glm::vec3 & lookAt)
{
    std::cout<<"getCamera"<<std::endl;
    auto camera = std::make_shared<fovCamera>(lookFrom, lookAt, fov, aspect, width, height);
    return std::dynamic_pointer_cast<cameraBase>(camera);
}

void imageBasedLight::createTextures()
{
    auto env = std::make_shared<imageTexture>(envMap.c_str());
    auto envBase = std::dynamic_pointer_cast<texture>(env);
    textures["envTex"] = envBase;

    auto red = std::make_shared<constTexture>(glm::vec3(0.5, 0, 0));
    auto redBase = std::dynamic_pointer_cast<texture>(std::move(red));
    textures["redTex"] = redBase;
}

void imageBasedLight::createMatrials()
{
    auto envBase = textures["envTex"];
    auto envLightMat = std::make_shared<simpleMaterial>(envBase);
    materials["envMat"] = envLightMat;

    auto redBase = textures["redTex"];
    auto redDisneyMat = std::make_shared<disneyBRDFMaterial>(redBase);
    materials["redMat"] = redDisneyMat;
}

void imageBasedLight::createObjects()
{
    auto envMat = getMatrial("envMat");
    auto cornellBox = std::make_shared<cube>(glm::vec3(-50, -50, -50), glm::vec3(50, 50, 50), envMat);
    auto cornellBoxBase = std::dynamic_pointer_cast<primitiveBase>(cornellBox);
    objects["cornellBox"] = cornellBoxBase;
}

unordered_map<string, shared_ptr<primitiveBase>> imageBasedLight::getAllObjects()
{
    return objects;
}
