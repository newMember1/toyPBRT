#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <unordered_map>
#include <memory>

#include "primitiveBase.h"
#include "primitive/model.h"
#include "directionGenerator.h"
#include "./pdf/rectLightPdf.h"
#include "primitiveList.h"

#include "materials/simplematerial.h"

class scene
{
public:
    scene(std::string path);
    scene(std::unordered_map<std::string,std::shared_ptr<primitiveBase>> objects);

    std::vector<std::vector<float>> getSceneFrameInfo();
    std::unique_ptr<primitiveList> & getLists();
private:
    void loadTexture(std::string path);
    void loadMatrials(std::string path);
    void loadObjects(std::string path);

    std::unordered_map<std::string,std::shared_ptr<primitiveBase>> objects;
    std::unordered_map<std::string,std::shared_ptr<texture>> textures;
    std::unordered_map<std::string,std::shared_ptr<materialBase>> materials;
    std::unordered_map<std::string,std::shared_ptr<primitiveBase>> lightObjects;
    std::unique_ptr<primitiveList> worldList;
};

#endif // SCENE_H
