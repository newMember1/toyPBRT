#include "simplematerial.h"

simpleMaterial::simpleMaterial(std::shared_ptr<texture> tex)
    :materialBase(tex)
{
    type = materialType::simpleMaterial;
}

glm::vec3 simpleMaterial::albedo(const hitRecord &hitRec, const glm::vec3 &inDirec, const glm::vec3 &outDirec)
{
    //simple material don't consider the energy dispear
    return this->tex->baseColor(hitRec.u, hitRec.v, hitRec.hitPos);
}
