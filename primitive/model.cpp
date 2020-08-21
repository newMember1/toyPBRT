#include "model.h"

model::model(std::string path,std::shared_ptr<materialBase> _mat):primitiveBase(_mat)
{
    std::cout<<"load model: "<<path<<std::endl;
    if(!objLoader::load(path,verts,vIndexs,normals,nIndexs))
        return;

    float maxRange=-1e6;
    for (auto v : verts)
        maxRange = fmax(std::fmax(abs(v.x), maxRange), std::fmax(abs(v.y),abs(v.z)));

    for (auto &v : verts)
        v /= maxRange;

    min = glm::vec3(1e6);
    max = glm::vec3(-1e6);

    for (auto v : verts)
    {
        min.x = std::fmin(min.x, v.x);
        min.y = std::fmin(min.y, v.y);
        min.z = std::fmin(min.z, v.z);

        max.x = std::fmax(max.x, v.x);
        max.y = std::fmax(max.y, v.y);
        max.z = std::fmax(max.z, v.z);
    }
    this->aabbBox._min=min;
    this->aabbBox._max=max;

    if (normals.size() == 0)
    {
        std::cout<<"model::calculate normals..."<<std::endl;
        for (int i=0;i<vIndexs.size();++i)
        {
            auto idx = vIndexs[i];
            int a = idx[0];
            int b = idx[1];
            int c = idx[2];

            //clockwise calculate normal
            glm::vec3 p1 = verts[a];
            glm::vec3 p2 = verts[b];
            glm::vec3 p3 = verts[c];
            float na = (p2.y - p1.y)*(p3.z - p1.z) - (p3.y - p1.y)*(p2.z - p1.z);
            float nb = (p2.z - p1.z)*(p3.x - p1.x) - (p3.z - p1.z)*(p2.x - p1.x);
            float nc = (p2.x - p1.x)*(p3.y - p1.y) - (p3.x - p1.x)*(p2.y - p1.y);

            glm::vec3 n(na, nb, nc);

            n = glm::normalize(n);
            normals.push_back(n);
        }
    }

    for(int i=0;i<vIndexs.size();++i)
    {
        auto idx=vIndexs[i];
        int a=idx[0];
        int b=idx[1];
        int c=idx[2];

        auto p1=verts[a];
        auto p2=verts[b];
        auto p3=verts[c];
        auto n=normals[i];
        std::shared_ptr<triangle> tri(new triangle(p1,p2,p3,n,mat));
        triangles.push_back(std::dynamic_pointer_cast<primitiveBase>(std::move(tri)));
    }

    std::cout<<"constructor model..."<<std::endl;
    modelBVH.reset(new bvh(triangles));
}

bool model::hit(ray &r, hitRecord &h, float minT, float maxT)
{
    if(modelBVH==nullptr)
        std::cout<<"error,no data in model..."<<std::endl;
    if(!aabbBox.hit(r,minT,maxT))
        std::cout<<"don't hit model..."<<std::endl;
    return modelBVH->hit(r,h,minT,maxT);
}

glm::vec3 model::normal(const glm::vec3 &surPos)
{
    return glm::vec3(0);
}

glm::vec3 model::reflect(const glm::vec3 &inDirec, const glm::vec3 &normal)
{
    return glm::normalize(inDirec - 2 * glm::dot(inDirec, normal)*normal);
}

bool model::boxHit(const ray &r, float minT, float maxT)
{
    return aabbBox.hit(r,minT,maxT);
}

void model::handleMatrix()
{
    //update verts and aabb box
    for(auto &v : verts)
    {
        v = glm::vec3(glm::vec4(v, 1.0) * modelMatrix);
        min.x = std::fmin(min.x, v.x);
        min.y = std::fmin(min.y, v.y);
        min.z = std::fmin(min.z, v.z);

        max.x = std::fmax(max.x, v.x);
        max.y = std::fmax(max.y, v.y);
        max.z = std::fmax(max.z, v.z);
    }
    this->aabbBox._min=min;
    this->aabbBox._max=max;

    //update normals
    glm::mat4 G = glm::transpose(invModelMatrix);
    for (auto &n : normals)
        n = glm::vec3(glm::vec4(n, 1.0) * G);

    //update triangles
    for (int i=0; i<vIndexs.size(); ++i)
    {
        auto idx = vIndexs[i];
        int a = idx[0];
        int b = idx[1];
        int c = idx[2];

        auto p1 = verts[a];
        auto p2 = verts[b];
        auto p3 = verts[c];
        auto n = normals[i];
        std::shared_ptr<triangle> tri(new triangle(p1,p2,p3,n,mat));
        triangles[i] = std::move(tri);
    }

    modelBVH.reset(new bvh(triangles));
}
