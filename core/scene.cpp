#include "scene.h"

scene::scene(std::string)
{

}

scene::scene(std::unordered_map<std::string,std::shared_ptr<primitiveBase>> objects)
{
    this->objects.clear();
    this->lightObjects.clear();

    this->objects = objects;

    shared_ptr<pdfBase> rect;
    for(auto obj : objects)
    {
        //if object is light, add to lightObjects for light pdf's calculate
        if(obj.second->mat->isLight)
        {
            lightObjects[obj.first] = obj.second;
            auto recPdf = std::make_shared<rectLightPdf>(std::dynamic_pointer_cast<rectangle>(obj.second));
            rect = std::dynamic_pointer_cast<pdfBase>(recPdf);
        }

        //handle every object's matrix;
        obj.second->handleMatrix();
    }

    //handle direction generator
    directionGenerator::getInstance().setLightPdf(rect);

    //reset the whole world
    worldList.reset(new primitiveList(objects));
}

std::unique_ptr<primitiveList> & scene::getLists()
{
    return this->worldList;
}

std::vector<std::vector<float>> scene::getSceneFrameInfo()
{
    std::vector<float> verts;
    std::vector<float> colors;
    for(auto obj : objects)
    {
        auto datas = obj.second->getModelLinesAndColors();
        auto vs = datas[0];
        auto cs = datas[1];
        //1.get verts
        for(auto v : vs)
            verts.push_back(v);
        //2.get relative color
        for(auto c : cs)
            colors.push_back(c);
    }

    return {verts, colors};
}

void scene::loadObjects(std::string path)
{
    auto deer = std::make_shared<model>("/home/zdxiao/Desktop/resources/models/deer/deer.obj", materials["goldMat"]);
    auto deerBase = std::dynamic_pointer_cast<primitiveBase>(std::move(deer));
    objects["deer"] = deerBase;

    auto planeLight = std::make_shared<rectangle>(glm::vec3(-1, -1, 3), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0), 2, 2, materials["whiteLightMat"]);
    auto planeLightBase = std::dynamic_pointer_cast<primitiveBase>(planeLight);
    objects["planeLight"] = planeLightBase;

    auto recPdf = std::make_shared<rectLightPdf>(planeLight);
    auto recPdfBase = std::dynamic_pointer_cast<pdfBase>(std::move(recPdf));
    directionGenerator::getInstance().setLightPdf(recPdfBase);
}

void scene::loadTexture(std::string path)
{
    std::cout<<"loadTextures."<<std::endl;

    auto red = std::make_shared<constTexture>(glm::vec3(0.65, 0.05, 0.05));
    auto redBase = std::dynamic_pointer_cast<texture>(std::move(red));
    textures["redTex"] = redBase;

    auto green = std::make_shared<constTexture>(glm::vec3(0.12, 0.45, 0.15));
    auto greenBase = std::dynamic_pointer_cast<texture>(std::move(green));
    textures["greenTex"] = greenBase;

    auto blue = std::make_shared<constTexture>(glm::vec3(0, 0, 1.0f));
    auto blueBase = std::dynamic_pointer_cast<texture>(std::move(blue));
    textures["blueTex"] = blueBase;

    auto gold = std::make_shared<constTexture>(glm::vec3(1.022f,0.782f,0.344f));
    auto goldBase = std::dynamic_pointer_cast<texture>(std::move(gold));
    textures["goldTex"] = goldBase;

    auto white = std::make_shared<constTexture>(glm::vec3(1.0f));
    auto whiteBase = std::dynamic_pointer_cast<texture>(std::move(white));
    textures["whiteTex"] = whiteBase;

    std::cout<<"loadTextures done."<<std::endl<<std::endl;
}

void scene::loadMatrials(std::string path)
{
    std::cout<<"loadMaterials."<<std::endl;

//    auto red = std::make_shared<simpleMaterial>(textures["redTex"]);
//    auto redBase = std::dynamic_pointer_cast<materialBase>(std::move(red));
//    materials["redMat"] = redBase;

//    auto green = std::make_shared<simpleMaterial>(textures["greenTex"]);
//    auto greenBase = std::dynamic_pointer_cast<materialBase>(std::move(green));
//    materials["greenMat"] = greenBase;

//    auto blue = std::make_shared<simpleMaterial>(textures["blueTex"]);
//    auto blueBase = std::dynamic_pointer_cast<materialBase>(std::move(blue));
//    materials["blueMat"] = blueBase;

//    auto gold = std::make_shared<simpleMaterial>(textures["goldTex"]);
//    auto goldBase = std::dynamic_pointer_cast<materialBase>(std::move(gold));
//    materials["goldMat"] = goldBase;

//    auto whiteLight = std::make_shared<simpleMaterial>(textures["whiteTex"]);
//    auto whiteLightBase = std::dynamic_pointer_cast<materialBase>(std::move(whiteLight));
//    whiteLightBase->isLight = true;
//    materials["whiteLightMat"] = whiteLightBase;

    std::cout<<"loadMaterials done."<<std::endl<<std::endl;
}
