#ifndef SPHERESCENE_H
#define SPHERESCENE_H

#include <unordered_map>
#include "../primitive/rectangle.h"
#include "../primitive/sphere.h"
#include "../materials/simplematerial.h"
#include "../materials/dielectricMaterial.h"

using std::unordered_map;
using std::shared_ptr;
using std::string;

class sphereScene
{
public:
    void createTextures();
    void createMatrials();
    void createObjects();

    static sphereScene & getInstance();
    unordered_map<std::string, shared_ptr<primitiveBase>> getAllObjects();
private:
    sphereScene();

    glm::vec3 x{1, 0, 0};
    glm::vec3 y{0, 1, 0};
    glm::vec3 z{0, 0, 1};

    unordered_map<std::string, shared_ptr<texture>> textures;
    unordered_map<std::string, shared_ptr<materialBase>> materials;
    unordered_map<std::string, shared_ptr<primitiveBase>> objects;

    shared_ptr<texture> getTexture(string name);
    shared_ptr<materialBase> getMatrial(string name);
    shared_ptr<primitiveBase> getObject(string name);
};

#endif // SPHERESCENE_H
