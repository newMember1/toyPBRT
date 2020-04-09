#ifndef MODEL_H
#define MODEL_H
#include"./core/primitiveBase.h"
#include"./models/objloader.h"
#include"./accelerate/bvh.h"
#include"./triangle.h"
#include<vector>

class model : public primitiveBase
{
public:
    model(std::string path,std::shared_ptr<materialBase> _mat);
    bool hit(ray &r, hitRecord &h, float minT, float maxT) override;
    glm::vec3 normal(const glm::vec3 &surPos) override;
    glm::vec3 reflect(const glm::vec3 &inDirec, const glm::vec3 &normal) override;
    bool boxHit(const ray &r, float minT, float maxT) override;

private:
    std::vector<glm::vec3> verts;
    std::vector<std::vector<int>> vIndexs;
    std::vector<glm::vec3> normals;
    std::vector<std::vector<int>> nIndexs;
    std::unique_ptr<bvh> tree;

    std::vector<std::shared_ptr<primitiveBase>> triangles;
    glm::vec3 min;
    glm::vec3 max;

    std::unique_ptr<bvh> modelBVH=nullptr;
};

#endif // OBJMODEL_H
