#include "cube.h"
#include <QDebug>
extern bool debugFlag;

cube::cube(const glm::vec3 & _min, const glm::vec3 & _max, std::shared_ptr<materialBase> _mat)
    :primitiveBase(_mat)
{
    pType = primitiveType::unknown;
    auto min = _min;
    auto max = _max;
    this->aabbBox._min = min - glm::vec3(epslion);
    this->aabbBox._max = max + glm::vec3(epslion);

    glm::vec3 X{1, 0, 0};
    glm::vec3 Y{0, 1, 0};
    glm::vec3 Z{0, 0, 1};

    float lenX = max.x - min.x;
    float lenY = max.y - min.y;
    float lenZ = max.z - min.z;

    //left
    glm::vec3 leftS{min.x, min.y, min.z};
    faces[0] = std::make_shared<rectangle>(leftS, Z, Y, lenZ, lenY, _mat);

    //right
    glm::vec3 rightS{max.x, min.y, max.z};
    faces[1] = std::make_shared<rectangle>(rightS, -Z, Y, lenZ, lenY, _mat);

    //front
    glm::vec3 frontS{min.x, min.y, max.z};
    faces[2] = std::make_shared<rectangle>(frontS, X, Y, lenX, lenY, _mat);

    //back
    glm::vec3 backS{max.x, min.y, min.z};
    faces[3] = std::make_shared<rectangle>(backS, -X, Y, lenX, lenY, _mat);

    //top
    glm::vec3 topS{min.x, max.y, max.z};
    faces[4] = std::make_shared<rectangle>(topS, X, -Z, lenX, lenZ, _mat);

    //bottom
    glm::vec3 bottomS{min.x, min.y, min.z};
    faces[5] = std::make_shared<rectangle>(bottomS, X, Z, lenX, lenZ, _mat);
}

void cube::setTranslate(const glm::vec3 &trans)
{
    for(auto & face : faces)
        face->setTranslate(trans);
}

void cube::setRotate(const glm::vec3 &rotateAxis, float angle)
{
    for(auto & face : faces)
        face->setRotate(rotateAxis, angle);
}

void cube::setUniformScale(float s)
{
    for(auto & face : faces)
    {
        face->setUniformScale(s);
    }
}

void cube::setNonUniformScale(const glm::vec3 &s)
{
    for(auto & face : faces)
    {
        face->setNonUniformScale(s);
    }
}

void cube::setModelMatrix(const glm::mat4 &m)
{
    for(auto & face : faces)
        face->setModelMatrix(m);
}

bool cube::boxHit(const ray &r, float minT, float maxT)
{
    return aabbBox.hit(r,minT,maxT);
}

bool cube::hit(ray &r, hitRecord &h, float minT, float maxT)
{
    bool res = false;
    for(int i = 0; i < 6; ++i)
    {
        if(faces[i]->hit(r, h, minT, maxT))
            res = true;
    }

    return res;
}

glm::vec3 cube::normal(const glm::vec3 &surPos)
{
    return glm::vec3(0);
}

glm::vec3 cube::reflect(const glm::vec3 &inDirec, const glm::vec3 &normal)
{
    return glm::normalize(inDirec - 2 * glm::dot(inDirec, normal)*normal);
}

std::vector<std::vector<float>> cube::getModelLinesAndColors()
{
    std::vector<float> verts;
    std::vector<float> colors;

    for(int i = 0; i < 6; ++i)
    {
        auto datas = faces[i]->getModelLinesAndColors();
        auto vert = datas[0];
        auto color = datas[1];
        for(auto v : vert)
            verts.push_back(v);
        for(auto c : color)
            colors.push_back(c);
    }

    return {verts, colors};
}

void cube::handleMatrix()
{
    for(int i = 0; i < 6; ++i)
    {
        faces[i]->handleMatrix();
    }

    //update aabb box
    auto box1 = surrounding_box(faces[0]->aabbBox, faces[1]->aabbBox);
    auto box2 = surrounding_box(faces[2]->aabbBox, faces[3]->aabbBox);
    auto box3 = surrounding_box(faces[4]->aabbBox, faces[5]->aabbBox);
    auto box4 = surrounding_box(box1, box2);
    this->aabbBox = surrounding_box(box3, box4);
}
