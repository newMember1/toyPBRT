#include "scene.h"

scene::scene(std::string)
{

}

scene::scene(std::unordered_map<std::string,std::shared_ptr<primitiveBase>> objects)
{
    this->objects.clear();
    this->lightObjects.clear();

    //store all objects
    this->objects = objects;

    //store all light objects
    unordered_map<string, shared_ptr<pdfBase>> lights;
    for(auto & obj : objects)
    {
        if(obj.second->mat->isLight)
        {
            if(obj.second->pType == primitiveType::rectangle)
            {
                lightObjects[obj.first] = obj.second;
                auto recPdf = std::make_shared<rectLightPdf>(std::dynamic_pointer_cast<rectangle>(obj.second));
                lights[obj.first] = std::dynamic_pointer_cast<pdfBase>(recPdf);
            }

            else if(obj.second->pType == primitiveType::triangle)
            {
                //(TO DO)make triangle light pdf
            }

            else if(obj.second->pType == primitiveType::sphere)
            {
                lightObjects[obj.first] = obj.second;
                auto spherePdf = std::make_shared<sphereLightPdf>(std::dynamic_pointer_cast<sphere>(obj.second));
                lights[obj.first] = std::dynamic_pointer_cast<pdfBase>(spherePdf);
            }
        }

        //handle every object's matrix;
        obj.second->handleMatrix();
    }

    //handle direction generator
    directionGenerator::getInstance().setLightPdfs(lights);
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

    auto red = std::make_shared<simpleMaterial>(textures["redTex"]);
    auto redBase = std::dynamic_pointer_cast<materialBase>(std::move(red));
    materials["redMat"] = redBase;

    auto green = std::make_shared<simpleMaterial>(textures["greenTex"]);
    auto greenBase = std::dynamic_pointer_cast<materialBase>(std::move(green));
    materials["greenMat"] = greenBase;

    auto blue = std::make_shared<simpleMaterial>(textures["blueTex"]);
    auto blueBase = std::dynamic_pointer_cast<materialBase>(std::move(blue));
    materials["blueMat"] = blueBase;

    auto gold = std::make_shared<simpleMaterial>(textures["goldTex"]);
    auto goldBase = std::dynamic_pointer_cast<materialBase>(std::move(gold));
    materials["goldMat"] = goldBase;

    auto whiteLight = std::make_shared<simpleMaterial>(textures["whiteTex"]);
    auto whiteLightBase = std::dynamic_pointer_cast<materialBase>(std::move(whiteLight));
    whiteLightBase->isLight = true;
    materials["whiteLightMat"] = whiteLightBase;

    std::cout<<"loadMaterials done."<<std::endl<<std::endl;
}
