#include "model.h"

model::model(std::string path,std::shared_ptr<materialBase> _mat):primitiveBase(_mat)
{
    std::cout<<"load model: "<<path<<std::endl;
    if(!objLoader::load(path,verts,vIndexs,normals,nIndexs))
        return;

    pType = primitiveType::unknown;
    min = glm::vec3(1e6);
    max = glm::vec3(-1e6);

    for(int i=0;i<vIndexs.size();++i)
    {
        auto idx=vIndexs[i];
        int a = idx[0];
        int b = idx[1];
        int c = idx[2];

        auto p1 = verts[a];
        auto p2 = verts[b];
        auto p3 = verts[c];

        std::shared_ptr<triangle> tri(new triangle(p1, p2, p3, mat));
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

void model::setTranslate(const glm::vec3 &trans)
{
    for(auto & tri : triangles)
        tri->setTranslate(trans);
}

void model::setRotate(const glm::vec3 &rotateAxis, float angle)
{
    for(auto & tri : triangles)
        tri->setRotate(rotateAxis, angle);
}

void model::setUniformScale(float s)
{
    for(auto & tri : triangles)
    {
        tri->setUniformScale(s);
    }
}

void model::setNonUniformScale(const glm::vec3 &s)
{
    for(auto & tri : triangles)
    {
        tri->setNonUniformScale(s);
    }
}

void model::setModelMatrix(const glm::mat4 &m)
{
    for(auto & tri : triangles)
        tri->setModelMatrix(m);
}

bool model::boxHit(const ray &r, float minT, float maxT)
{
    return aabbBox.hit(r,minT,maxT);
}

std::vector<std::vector<float>> model::getModelLinesAndColors()
{
    std::vector<float> verts;
    std::vector<float> colors;
    for(auto tri : triangles)
    {
        auto datas = tri->getModelLinesAndColors();
        auto vert = datas[0];
        auto color = datas[1];
        for(auto v : vert)
            verts.push_back(v);
        for(auto c : color)
            colors.push_back(c);
    }

    return {verts, colors};
}

void model::handleMatrix()
{
    for(auto & tri : triangles)
    {
        tri->handleMatrix();
    }

    //update verts and aabb box
    min = glm::vec3(1e6);
    max = glm::vec3(-1e6);
    for(auto tri : triangles)
    {
        min.x = std::fmin(min.x, tri->aabbBox._min.x);
        min.y = std::fmin(min.y, tri->aabbBox._min.y);
        min.z = std::fmin(min.z, tri->aabbBox._min.z);

        max.x = std::fmax(max.x, tri->aabbBox._max.x);
        max.y = std::fmax(max.y, tri->aabbBox._max.y);
        max.z = std::fmax(max.z, tri->aabbBox._max.z);
    }

    this->aabbBox._min=min;
    this->aabbBox._max=max;

    modelBVH.reset(new bvh(triangles));
}

glm::vec3 model::hitXAxis(const glm::vec3 & hitPos)
{
	//triangle's x axis
	std::cout << "warning you are using model's hitYAxis, please check your code";
	return glm::vec3(0.0f);
}

glm::vec3 model::hitYAxis(const glm::vec3 & hitPos)
{
	//triangle's x axis
	std::cout << "warning you are using model's hitYAxis, please check your code";
	return glm::vec3(0.0f);
}
