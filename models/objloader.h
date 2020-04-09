#ifndef OBJLOADER_H
#define OBJLOADER_H

#define TINYOBJLOADER_IMPLEMENTATION
#include"../core/baseStructure.h"
#include<vector>
#include<unordered_map>
#include<map>
#include<fstream>
#include<sstream>

struct HashFunc
{
    std::size_t operator()(const glm::vec3 &key) const
    {
        using std::size_t;
        using std::hash;

        return ((hash<int>()(key.x)
                    ^ (hash<int>()(key.y) << 1)) >> 1)
                    ^ (hash<int>()(key.z) << 1);
    }
};

struct EqualFunc
{
    bool operator() (const glm::vec3 &a,const glm::vec3 &b)const
    {
        if (fabs(a.x - b.x) < epslion&&fabs(a.y - b.y) < epslion&&fabs(a.z - b.z) < epslion)
                    return true;
                else
                    return false;
    }
};

class objLoader
{
public:
    static bool load(std::string path,std::vector<glm::vec3>&verts,std::vector<std::vector<int>> &vIndexs, std::vector<glm::vec3> &norms, std::vector<std::vector<int>> &nIndexs);
private:
    objLoader();
};

#endif // OBJLOADER_H
