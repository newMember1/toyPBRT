#ifndef CORNELLBOXDISNEY_H
#define CORNELLBOXDISNEY_H

#include <unordered_map>
#include "../materials/disneybrdfmaterial.h"
#include "../materials/simplematerial.h"
#include "../materials/dielectricMaterial.h"
#include "../primitive/rectangle.h"
#include "../primitive/sphere.h"
#include "../primitive/triangle.h"
#include "../primitive/model.h"

using std::unordered_map;
using std::shared_ptr;
using std::string;

class glassBunny
{
public:
    void createTextures();
    void createMatrials();
    void createObjects();

    unordered_map<std::string, shared_ptr<primitiveBase>> getAllObjects();
    static glassBunny & getInstance();

private:
    glm::vec3 x{1, 0, 0};
    glm::vec3 y{0, 1, 0};
    glm::vec3 z{0, 0, 1};
    unordered_map<std::string, shared_ptr<textureBase>> textures;
    unordered_map<std::string, shared_ptr<materialBase>> materials;
    unordered_map<std::string, shared_ptr<primitiveBase>> objects;

    shared_ptr<textureBase> getTexture(string name);
    shared_ptr<materialBase> getMatrial(string name);
    shared_ptr<primitiveBase> getObject(string name);

    glassBunny();
};

#endif // CORNELLBOXDISNEY_H
