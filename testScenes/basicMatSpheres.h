#ifndef SPHERESCENE_H
#define SPHERESCENE_H

#include <unordered_map>
#include "../primitive/rectangle.h"
#include "../primitive/sphere.h"
#include "../materials/simplematerial.h"
#include "../materials/dielectricMaterial.h"
#include "../materials/disneybrdfmaterial.h"
#include "../materials/basicbrdfmaterial.h"
#include "../camera/fovCamera.h"

using std::unordered_map;
using std::shared_ptr;
using std::string;

class basicMatSpheres
{
public:
    void createTextures();
    void createMatrials();
    void createObjects();

    static basicMatSpheres & getInstance();
    std::shared_ptr<cameraBase> getCamera() { return cam; }
    unordered_map<std::string, shared_ptr<primitiveBase>> getAllObjects();
private:
    std::vector<glm::vec3> lightPositions =
    {
        glm::vec3(-10.0f,  10.0f, 10.0f),
        glm::vec3( 10.0f,  10.0f, 10.0f),
        glm::vec3(-10.0f, -10.0f, 10.0f),
        glm::vec3( 10.0f, -10.0f, 10.0f),
    };

    std::vector<glm::vec3> lightColors =
    {
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f)
    };

    int nrRows = 7;
    int nrColumns = 7;
    float spacing = 2.5;

    basicMatSpheres();

    glm::vec3 x{1, 0, 0};
    glm::vec3 y{0, 1, 0};
    glm::vec3 z{0, 0, 1};

    std::shared_ptr<cameraBase> cam;

    unordered_map<std::string, shared_ptr<texture>> textures;
    unordered_map<std::string, shared_ptr<materialBase>> materials;
    unordered_map<std::string, shared_ptr<primitiveBase>> objects;

    shared_ptr<texture> getTexture(string name);
    shared_ptr<materialBase> getMatrial(string name);
    shared_ptr<primitiveBase> getObject(string name);
};

#endif // SPHERESCENE_H
