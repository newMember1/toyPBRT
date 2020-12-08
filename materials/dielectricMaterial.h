#ifndef GLASSMATRIAL_H
#define GLASSMATRIAL_H
#include "../core/materialBase.h"

class dielectricMaterial : public materialBase
{
public:
    dielectricMaterial(std::shared_ptr<texture> tex, float ratio);
    glm::vec3 albedo(const hitRecord &hitRec, const glm::vec3 &inDirec, const glm::vec3 &outDirec) override;

    void setNiOverNt(float ratio);
};

#endif // GLASSMATRIAL_H
