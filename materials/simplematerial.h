#ifndef SIMPLEMATERIAL_H
#define SIMPLEMATERIAL_H
#include "../core/materialBase.h"

class simpleMaterial : public materialBase
{
public:
    simpleMaterial(std::shared_ptr<texture> tex);
    glm::vec3 albedo(const hitRecord & hitRec, const glm::vec3 & inDirec, const glm::vec3 & outDirec) override;
};

#endif // SIMPLEMATERIAL_H
