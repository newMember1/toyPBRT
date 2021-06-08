#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <unordered_map>
#include <memory>

#include "primitiveBase.h"
#include "primitive/model.h"
#include "primitive/cube.h"
#include "directionPdfAdaptor.h"
#include "./pdf/rectLightPdf.h"
#include "./pdf/sphereLightPdf.h"
#include "./pdf/ggxPdf.h"
#include "primitiveList.h"

#include "materials/simplematerial.h"

class sceneLists
{
public:
    sceneLists(std::string path);
    sceneLists(std::unordered_map<std::string,std::shared_ptr<primitiveBase>> objects);

    std::vector<std::vector<float>> getSceneFrameInfo();
    std::unique_ptr<primitiveList> & getLists();
private:
    void loadTexture(std::string path);
    void loadMatrials(std::string path);
    void loadObjects(std::string path);

    std::unordered_map<std::string,std::shared_ptr<primitiveBase>> objects;
    std::unordered_map<std::string,std::shared_ptr<textureBase>> textures;
    std::unordered_map<std::string,std::shared_ptr<materialBase>> materials;
    std::unordered_map<std::string,std::shared_ptr<primitiveBase>> lightObjects;
    std::unique_ptr<primitiveList> worldList;
};

#endif // SCENE_H
