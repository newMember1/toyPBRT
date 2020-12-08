#include "dielectricMaterial.h"

dielectricMaterial::dielectricMaterial(std::shared_ptr<texture> tex, float ratio)
    :materialBase(tex)
{
    //default glass refract ratio
    type = materialType::dielectrics;
    setNiOverNt(ratio);
}

glm::vec3 dielectricMaterial::albedo(const hitRecord &hitRec, const glm::vec3 &inDirec, const glm::vec3 &outDirec)
{
    return tex->baseColor(hitRec.u, hitRec.v, hitRec.hitPos);
}

void dielectricMaterial::setNiOverNt(float ratio)
{
    this->niOverNt = ratio;
}
