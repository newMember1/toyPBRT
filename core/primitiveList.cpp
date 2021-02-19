#include"primitiveList.h"
#include<iostream>

extern bool debugFlag;

primitiveList::primitiveList(std::vector<std::shared_ptr<primitiveBase>> &datas)
{
    pList.reserve(datas.size());
    for(int i=0; i<datas.size(); ++i)
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

void primitiveList::setMode(colorMode m)
{
    mode = m;
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

std::vector<float> primitiveList::getColors()
{
    return debugColors;
}

std::vector<float> primitiveList::getVertices()
{
    return debugVertices;
}

glm::vec3 primitiveList::color(ray &r, int times)
{
    switch (mode)
    {
    case colorMode::iterator:
        return colorIterator(r, times);
        break;
    case colorMode::recursive:
        return colorRecursive(r, times);
        break;
    case colorMode::hitTest:
        return colorHitTest(r, times);
        break;
    case colorMode::normalVis:
        return colorNormalVis(r, times);
        break;
    case colorMode::normalTest:
        return colorNormalTest(r, times);
        break;
    case colorMode::test:
        return test(r, times);
        break;
    }
}

glm::vec3 primitiveList::colorHitTest(ray &r, int times)
{
    hitRecord h;
    if(debugFlag)
    {
        debugVertices.push_back(r.pos.x);
        debugVertices.push_back(r.pos.y);
        debugVertices.push_back(r.pos.z);

        debugColors.push_back(0);
        debugColors.push_back(0);
        debugColors.push_back(0);
    }

    if(hit(r,h,0.001,1e6))
    {
        if(debugFlag)
        {
            debugVertices.push_back(h.hitPos.x);
            debugVertices.push_back(h.hitPos.y);
            debugVertices.push_back(h.hitPos.z);

            debugColors.push_back(h.matPtr->tex->baseColor(0, 0, glm::vec3(0)).x);
            debugColors.push_back(h.matPtr->tex->baseColor(0, 0, glm::vec3(0)).y);
            debugColors.push_back(h.matPtr->tex->baseColor(0, 0, glm::vec3(0)).z);
        }

        return h.matPtr->tex->baseColor(h.u, h.v, h.hitPos);
    }
    else
    {
        if(debugFlag)
        {
            debugVertices.push_back(h.hitPos.x + h.hitOutDirec.x * 2);
            debugVertices.push_back(h.hitPos.y + h.hitOutDirec.y * 2);
            debugVertices.push_back(h.hitPos.z + h.hitOutDirec.z * 2);

            debugColors.push_back(0);
            debugColors.push_back(0);
            debugColors.push_back(0);
        }
        return glm::vec3(0);
    }
}

glm::vec3 primitiveList::colorNormalVis(ray &r, int times)
{
    hitRecord h;
    glm::vec3 hitRes;
    if(hit(r, h, epslion, 1e6))
    {
        hitRes.x = h.hitNormal.x > 0 ? h.hitNormal.x : 0;
        hitRes.y = h.hitNormal.y > 0 ? h.hitNormal.y : 0;
        hitRes.z = h.hitNormal.z > 0 ? h.hitNormal.z : 0;
        return hitRes;
    }
    else
        return glm::vec3(0.1, 0.2, 0.3);
}

glm::vec3 primitiveList::colorNormalTest(ray &r, int times)
{
    hitRecord h;
    glm::vec3 hitRes;
    if(hit(r, h, epslion, 1e6))
    {
        float res = glm::dot(r.direc, h.hitNormal);
        if(glm::dot(r.direc ,h.hitNormal) < 0)
            hitRes = glm::vec3(pow(abs(res), 3), pow(abs(res), 2), pow(abs(res), 20));
        else
            hitRes = glm::vec3(0, 0, 0);

        return hitRes;
    }
    else
    {
        return glm::vec3(0.1, 0.2, 0.3);
    }
}

glm::vec3 primitiveList::colorRecursive(ray &r, int times)
{
    hitRecord h;

    if(hit(r,h,0.001,1e6))
    {
        if(times>MAX_TRACE_TIMES)
            return glm::vec3(0);

        //notice that ray.indirec means the -outDirec in brdf and outDirec
        glm::vec3 albe=h.matPtr->albedo(h,h.hitOutDirec,-r.direc) / h.hitPdf;
        r.pos=h.hitPos;
        r.direc=h.hitOutDirec;

        return albe*colorRecursive(r,times+1);
    }
    else
    {
        return glm::vec3(0);
    }
}

glm::vec3 primitiveList::colorIterator(ray &r, int times)
{
    //need to expand to avoid using stack
    if(debugFlag)
    {
        debugVertices.push_back(r.pos.x);
        debugVertices.push_back(r.pos.y);
        debugVertices.push_back(r.pos.z);

        debugColors.push_back(0);
        debugColors.push_back(0);
        debugColors.push_back(0);
    }

    glm::vec3 res{1,1,1};
    for(int i = 0; i < MAX_TRACE_TIMES; ++i)
    {
        hitRecord h;
        if(hit(r, h, 0.001, 1e6))
        {
            if(debugFlag)
            {
                debugVertices.push_back(h.hitPos.x);
                debugVertices.push_back(h.hitPos.y);
                debugVertices.push_back(h.hitPos.z);

                debugColors.push_back(h.matPtr->tex->baseColor(0, 0, glm::vec3(0)).x);
                debugColors.push_back(h.matPtr->tex->baseColor(0, 0, glm::vec3(0)).y);
                debugColors.push_back(h.matPtr->tex->baseColor(0, 0, glm::vec3(0)).z);

                /*auto co = h.matPtr->tex->baseColor(h.u, h.v, h.hitPos);
                std::cout<<"h.hitMatBaseColor: "<<co.x<<" "<<co.y<<" "<<co.z<<std::endl;
                std::cout<<"r.pos: "<<r.pos.x<<" "<<r.pos.y<<" "<<r.pos.z<<std::endl;
                std::cout<<"r.direc: "<<r.direc.x<<" "<<r.direc.y<<" "<<r.direc.z<<std::endl;
                std::cout<<"h.hitPos: "<<h.hitPos.x<<" "<<h.hitPos.y<<" "<<h.hitPos.z<<std::endl;
                std::cout<<"h.hitNormal: "<<h.hitNormal.x<<" "<<h.hitNormal.y<<" "<<h.hitNormal.z<<std::endl;
                std::cout<<"h.hitOutDirec: "<<h.hitOutDirec.x<<" "<<h.hitOutDirec.y<<" "<<h.hitOutDirec.z<<std::endl<<std::endl;*/
            }

            //if light
            if(h.matPtr->isLight)
            {
                if(glm::dot(r.direc, h.hitNormal) < 0)
                    return res * h.matPtr->tex->baseColor(h.u, h.v, h.hitPos);
                else
                    return glm::vec3{0, 0, 0};
            }

            //if glass
            if(h.matPtr->type == materialType::dielectrics)
            {
                h.hitOutDirec = h.hitRefract;
            }

            glm::vec3 albe = h.matPtr->albedo(h, h.hitOutDirec, - r.direc) / h.hitPdf;
            res *= albe;
            r.pos = h.hitPos;
            r.direc = h.hitOutDirec;
        }
        else
        {
            if(debugFlag)
            {
                /*std::cout<<"r.pos: "<<r.pos.x<<" "<<r.pos.y<<" "<<r.pos.z<<std::endl;
                std::cout<<"r.direc: "<<r.direc.x<<" "<<r.direc.y<<" "<<r.direc.z<<std::endl;
                std::cout<<"h.hitNormal: "<<h.hitNormal.x<<" "<<h.hitNormal.y<<" "<<h.hitNormal.z<<std::endl;
                std::cout<<"h.hitOutDirec: "<<h.hitOutDirec.x<<" "<<h.hitOutDirec.y<<" "<<h.hitOutDirec.z<<std::endl<<std::endl;*/

                debugVertices.push_back(h.hitPos.x + h.hitOutDirec.x * 700);
                debugVertices.push_back(h.hitPos.y + h.hitOutDirec.y * 700);
                debugVertices.push_back(h.hitPos.z + h.hitOutDirec.z * 700);

                debugColors.push_back(0);
                debugColors.push_back(0);
                debugColors.push_back(0);
            }
            return glm::vec3{0, 0, 0};
        }
    }

    return glm::vec3{0, 0, 0};
}

glm::vec3 primitiveList::test(ray &r, int times)
{
    //sun light
    hitRecord h;
    glm::vec3 lightDirec{0.0f, 0.0f, -1.0f};
    ray temp = r;
    if(hit(r, h, epslion, 1e6))
    {
        if(h.matPtr->isLight)
            return h.matPtr->tex->baseColor(h.u, h.v, h.hitPos);
        glm::vec3 albe = h.matPtr->albedo(h, -lightDirec, -temp.direc);
        return albe;
    }
    else
        return glm::vec3(0);

}
