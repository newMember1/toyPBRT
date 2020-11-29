#ifndef PRIMITIVELIST_H
#define PRIMITIVELIST_H
#include"primitiveBase.h"
#include"../accelerate/bvh.h"
#include<vector>
#include<unordered_map>

class primitiveList
{
public:
    primitiveList(){}
    primitiveList(std::vector<std::shared_ptr<primitiveBase>> &datas);
    primitiveList(std::unordered_map<std::string, std::shared_ptr<primitiveBase>> & datas);
    void setMode(int m);
    void addPrimitive(std::shared_ptr<primitiveBase> ptr);
    bool hit(ray &r,hitRecord &h,float minT,float maxT);
    glm::vec3 color(ray &r, int times);

    std::vector<std::shared_ptr<primitiveBase>> pList;
    std::unique_ptr<bvh> allModels=nullptr;
    std::vector<float> getVertices();
    std::vector<float> getColors();
private:
    glm::vec3 colorHitTest(ray &r,int times);
    glm::vec3 colorRecursive(ray &r,int times);
    glm::vec3 colorIterator(ray &r,int times);
    glm::vec3 colorNormalVis(ray & r, int times);
    glm::vec3 colorNormalTest(ray & r, int times);

    enum colorMode
    {
        iterator,
        recursive,
        hitTest,
        normalVis,
        normalTest
    };
    colorMode mode{iterator};
    std::vector<float> debugVertices;
    std::vector<float> debugColors;
};
#endif // PRIMITIVELIST_H
