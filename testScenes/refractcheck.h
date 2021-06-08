#ifndef REFRACTCHECK_H
#define REFRACTCHECK_H

#include <unordered_map>
#include "../primitive/rectangle.h"
#include "../primitive/sphere.h"
#include "../materials/simplematerial.h"
#include "../materials/dielectricMaterial.h"

using std::unordered_map;
using std::shared_ptr;
using std::string;

class refractCheck
{
public:
    void createTextures();
    void createMatrials();
    void createObjects();

    static refractCheck & getInstance();
    unordered_map<std::string, shared_ptr<primitiveBase>> getAllObjects();
private:
    unordered_map<std::string, shared_ptr<textureBase>> textures;
    unordered_map<std::string, shared_ptr<materialBase>> materials;
    unordered_map<std::string, shared_ptr<primitiveBase>> objects;

    shared_ptr<textureBase> getTexture(string name);
    shared_ptr<materialBase> getMatrial(string name);
    shared_ptr<primitiveBase> getObject(string name);

    refractCheck();
};

#endif // REFRACTCHECK_H
