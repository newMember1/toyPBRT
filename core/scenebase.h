#ifndef SCENEBASE_H
#define SCENEBASE_H

#include <unordered_map>
#include "texture.h"
#include "materialBase.h"
#include "primitiveBase.h"
#include "cameraBase.h"

using std::unordered_map;
using std::shared_ptr;
using std::string;

class sceneBase
{
public:
    sceneBase();
    virtual ~sceneBase();
    virtual void initResources() = 0;
    virtual unordered_map<std::string, shared_ptr<primitiveBase>> getAllObjects();
    virtual shared_ptr<cameraBase> getCamera(int width, int height, float fov, float aspect, const glm::vec3 & lookFrom, const glm::vec3 & lookAt) = 0;

    unordered_map<string, shared_ptr<texture>> textures;
    unordered_map<string, shared_ptr<materialBase>> materials;
    unordered_map<string, shared_ptr<primitiveBase>> objects;

    virtual void createTextures() = 0;
    virtual void createMatrials() = 0;
    virtual void createObjects() = 0;
    virtual shared_ptr<texture> getTexture(string name);
    virtual shared_ptr<materialBase> getMatrial(string name);
    virtual shared_ptr<primitiveBase> getObject(string name);
};

#endif // SCENEBASE_H
