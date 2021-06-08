#ifndef DISNEYMATOBJECTS_H
#define DISNEYMATOBJECTS_H

#include <unordered_map>
#include "../primitive/rectangle.h"
#include "../primitive/sphere.h"
#include "../materials/simplematerial.h"
#include "../materials/dielectricMaterial.h"
#include "../materials/disneybrdfmaterial.h"
#include "../camera/fovCamera.h"
#include "../primitive/model.h"

using std::unordered_map;
using std::shared_ptr;
using std::string;
class disneyMatObjects
{
public:
    void createTextures();
    void createMatrials();
    void createObjects();

    static disneyMatObjects & getInstance();
    std::shared_ptr<cameraBase> getCamera() { return cam; }
    unordered_map<std::string, shared_ptr<primitiveBase>> getAllObjects();
private:
    disneyMatObjects();

    std::shared_ptr<cameraBase> cam;

    unordered_map<std::string, shared_ptr<textureBase>> textures;
    unordered_map<std::string, shared_ptr<materialBase>> materials;
    unordered_map<std::string, shared_ptr<primitiveBase>> objects;

    shared_ptr<textureBase> getTexture(string name);
    shared_ptr<materialBase> getMatrial(string name);
    shared_ptr<primitiveBase> getObject(string name);
};

#endif // DISNEYMATOBJECTS_H
