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
    auto env = std::make_shared<envImageTexture>(envMap.c_str());
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
	envLightMat->isLight = true;
	auto evnLightMatBase = std::dynamic_pointer_cast<materialBase>(envLightMat);
    materials["envMat"] = evnLightMatBase;

    auto redBase = textures["redTex"];
    auto redDisneyMat = std::make_shared<disneyBRDFMaterial>(redBase);
	auto redDisneyMatBase = std::dynamic_pointer_cast<materialBase>(redDisneyMat);
    materials["redMat"] = redDisneyMatBase;
}

void imageBasedLight::createObjects()
{
    auto envMat = getMatrial("envMat");
    auto cornellBox = std::make_shared<cube>(glm::vec3(-50, -50, -50), glm::vec3(50, 50, 50), envMat, true);
	//cornellBox->setRotate(glm::vec3(1.0f), 20.0f);
    auto cornellBoxBase = std::dynamic_pointer_cast<primitiveBase>(cornellBox);
    objects["cornellBox"] = cornellBoxBase;

	auto redMat = getMatrial("redMat");
	auto ball = std::make_shared<sphere>(1.0f, glm::vec3(0.0f, 0.0f, -10.0f), redMat);
	auto ballBase = std::dynamic_pointer_cast<primitiveBase>(ball);
	objects["ball"] = ballBase;
}

unordered_map<string, shared_ptr<primitiveBase>> imageBasedLight::getAllObjects()
{
    return objects;
}
