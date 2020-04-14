/*
 * use bvh to accelerate search
 * now used in objmodel and primitiveList
 *
*/
#ifndef BVH_H
#define BVH_H
#include"./core/baseStructure.h"
#include"./core/primitiveBase.h"
#include<vector>

extern bool debugFlag;
struct bvhNode
{
    std::shared_ptr<bvhNode> left=nullptr;
    std::shared_ptr<bvhNode> right=nullptr;

    aabb box;
    std::shared_ptr<primitiveBase> leaf=nullptr;
    //if a node is leaf node then it must include the index
    bool hit(ray &r,hitRecord &h,float minT,float maxT)
    {
        if(!box.hit(r,minT,maxT))
            return false;

        if(leaf==nullptr)
        {
            bool lFlag=left->hit(r,h,minT,maxT);
            bool rFlag=right->hit(r,h,minT,maxT);
            return lFlag||rFlag;
        }
        else
        {
            return leaf->hit(r,h,minT,maxT);
        }
    }

    bvhNode(std::shared_ptr<primitiveBase> _leaf)
    {
        leaf=_leaf;
        box=leaf->aabbBox;
    }
    bvhNode(){}
};

//bvh tree,root is a bvhNode
class bvh
{
public:
    bvh(std::vector<std::shared_ptr<primitiveBase>> &datas);
    bool hit(ray &r,hitRecord &h,float minT,float maxT);
private:
    std::shared_ptr<bvhNode> root=nullptr;
    void generateBVHTree(std::shared_ptr<bvhNode> &root,std::vector<std::shared_ptr<primitiveBase>> &datas,int start,int end);
    static bool x_compare(std::shared_ptr<primitiveBase> a,std::shared_ptr<primitiveBase>b);
    static bool y_compare(std::shared_ptr<primitiveBase> a,std::shared_ptr<primitiveBase>b);
    static bool z_compare(std::shared_ptr<primitiveBase> a,std::shared_ptr<primitiveBase>b);
};
#endif // BVH_H
