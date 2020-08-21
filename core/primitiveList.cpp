#include"primitiveList.h"
#include<iostream>

extern bool debugFlag;

primitiveList::primitiveList(std::vector<std::shared_ptr<primitiveBase>> &datas)
{
    pList.reserve(datas.size());
    for(int i=0;i<datas.size();++i)
       pList.push_back(datas[i]);

    allModels.reset(new bvh(pList));
}

primitiveList::primitiveList(std::unordered_map<std::string, std::shared_ptr<primitiveBase>> & datas)
{
    pList.reserve(datas.size());
    for(auto data:datas)
        pList.push_back(data.second);

    allModels.reset(new bvh(pList));
}

void primitiveList::addPrimitive(std::shared_ptr<primitiveBase> ptr)
{
    pList.push_back(std::move(ptr));
    allModels.reset(new bvh(pList));
}

bool primitiveList::hit(ray &r, hitRecord &h, float minT, float maxT)
{
    //this function is used to find the nearest hit point with hit information
    return allModels->hit(r,h,minT,maxT);
}

glm::vec3 primitiveList::colorHitTest(ray &r, int times)
{
    hitRecord h;

    if(debugFlag)
        int a=1;
    if(hit(r,h,epslion,1e6))
        return glm::vec3(0.8,0.7,0.6);
    else
        return glm::vec3(0.1,0.2,0.3);
}

glm::vec3 primitiveList::colorNormalVis(ray &r, int times)
{
    hitRecord h;
    if(hit(r, h, epslion, 1e6))
        return h.hitNormal;
    else
        return glm::vec3(0.1, 0.2, 0.3);
}

glm::vec3 primitiveList::color(ray &r, int times)
{
    hitRecord h;

    if(hit(r,h,0.001,1e6))
    {
        if(h.matPtr->isLight)
            return h.matPtr->tex->baseColor(h.u,h.v,h.hitPos);

        if(times>20)
            return glm::vec3(0);

        //notice that ray.indirec means the -outDirec in brdf and outDirec
        glm::vec3 albe=h.matPtr->albedo(h,h.hitOutDirec,-r.direc) / h.hitPdf;
        r.pos=h.hitPos;
        r.direc=h.hitOutDirec;
        return albe*color(r,times+1);
    }
    else
    {
        float t=0.5*(r.direc.y+1);
        return glm::vec3(t);
    }
}

glm::vec3 primitiveList::colorIterator(ray &r, int times)
{
    hitRecord h;
    glm::vec3 res(1.0);
    for(int i=0;i<times;++i)
    {
        if(hit(r,h,0.001,1e6))
        {
            if(h.matPtr->isLight)
                return h.matPtr->tex->baseColor(h.u,h.v,h.hitPos)*res;
            glm::vec3 albe=h.matPtr->albedo(h,h.hitOutDirec,-r.direc) / h.hitPdf;
            res=res*albe;
            r.pos=h.hitPos;
            r.direc=h.hitOutDirec;
        }
        else
        {
            return glm::vec3(0);
        }
    }
    return glm::vec3(0);
}
