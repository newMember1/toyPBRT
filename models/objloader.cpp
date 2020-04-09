#include "objloader.h"
#include<iostream>
#include"../3rdparty/tiny_obj_loader.h"
objLoader::objLoader()
{

}

bool objLoader::load(std::string path, std::vector<glm::vec3> &verts, std::vector<std::vector<int> > &vIndexs, std::vector<glm::vec3> &norms, std::vector<std::vector<int> > &nIndexs)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn,err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str()))
    {
        std::cout << "error in open objModel :" << path << std::endl;
    }

    std::unordered_map<glm::vec3,unsigned int,HashFunc,EqualFunc> allVerts;
    std::vector<int> vInds;

    std::unordered_map<glm::vec3,unsigned int,HashFunc,EqualFunc> allNormals;
    std::vector<int> nInds;

    bool noLoadNormals=false;
    for (const auto & shape : shapes)
    {
        for (int i=0;i<shape.mesh.indices.size();++i)
        {
            auto index = shape.mesh.indices[i];
            //1.vertex
            int ia = 3 * index.vertex_index;
            int ib = 3 * index.vertex_index + 1;
            int ic = 3 * index.vertex_index + 2;
            float a = attrib.vertices[ia];
            float b = attrib.vertices[ib];
            float c = attrib.vertices[ic];

            glm::vec3 vert(a, b, c);
            if (allVerts.count(vert) == 0)
            {
                allVerts.insert(std::make_pair(vert, allVerts.size()));
                verts.push_back(vert);
            }
            vInds.push_back(allVerts[vert]);

            //2.normal
            if (!noLoadNormals)
            {
                int na = 3 * index.normal_index;
                int nb = 3 * index.normal_index + 1;

                int nc = 3 * index.normal_index + 2;
                if (na < 0)
                {
                    noLoadNormals = true;
                    continue;
                }
                a = attrib.normals[na];
                b = attrib.normals[nb];
                c = attrib.normals[nc];
                glm::vec3 norm(a, b, c);
                if (allNormals.count(norm) == 0)
                {
                    allNormals.insert(std::make_pair(norm, allNormals.size()));
                    norms.push_back(norm);
                }
                nInds.push_back(allNormals[norm]);
             }
        }

        for (int i = 0; i < vInds.size(); i+=3)
        {
            std::vector<int> ind;
            ind.push_back(vInds[i]);
            ind.push_back(vInds[i+1]);
            ind.push_back(vInds[i+2]);
            vIndexs.push_back(ind);
        }
        for (int i = 0; i < nInds.size(); i+=3)
        {
            std::vector<int> ind;
            ind.push_back(nInds[i]);
            ind.push_back(nInds[i + 1]);
            ind.push_back(nInds[i + 2]);
            nIndexs.push_back(ind);
        }
    }
    return true;
}
