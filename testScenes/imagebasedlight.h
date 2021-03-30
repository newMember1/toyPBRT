#ifndef IMAGEBASEDLIGHT_H
#define IMAGEBASEDLIGHT_H

#include "../core/scenebase.h"
#include "../primitive/cube.h"
#include "../primitive/sphere.h"
#include "../materials/simplematerial.h"
#include "../materials/dielectricMaterial.h"
#include "../materials/disneybrdfmaterial.h"
#include "../materials/basicbrdfmaterial.h"
#include "../camera/fovCamera.h"

class imageBasedLight :
        public sceneBase
{
public:
    imageBasedLight();
    virtual void initResources() override;
    virtual unordered_map<std::string, shared_ptr<primitiveBase>> getAllObjects() override;
    virtual shared_ptr<cameraBase> getCamera(int nx, int ny, float fov, float aspect, const glm::vec3 & lookFrom, const glm::vec3 & lookAt) override;

private:
    virtual void createTextures() override;
    virtual void createMatrials() override;
    virtual void createObjects() override;

    string envMap = ":/pictures/resources/Mt-Washington-Gold-Room_Bg";
    string ironMap = "";
};

#endif // IMAGEBASEDLIGHT_H
