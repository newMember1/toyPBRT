#include"bvh.h"
#include<algorithm>

bvh::bvh(std::vector<std::shared_ptr<primitiveBase>> &datas)
{
    //root is a bvhNode ,recursive construct the bvhTree
    generateBVHTree(root,datas,0,datas.size()-1);
}

void bvh::generateBVHTree(std::shared_ptr<bvhNode> &root,std::vector<std::shared_ptr<primitiveBase>> &datas,int start,int end)
{
    root.reset(new bvhNode());
    int axis=rand()%3;
    if(axis==0)
        sort(datas.begin()+start,datas.begin()+end,x_compare);
    else if(axis==1)
        sort(datas.begin()+start,datas.begin()+end,y_compare);
    else
        sort(datas.begin()+start,datas.begin()+end,z_compare);

    if(start==end)
    {
        root->left.reset(new bvhNode(datas[start]));
        root->right.reset(new bvhNode(datas[end]));
    }
    else if(start+1==end)
    {
        root->left.reset(new bvhNode(datas[start]));
        root->right.reset(new bvhNode(datas[end]));
    }
    else
    {
        int mid=(start+end)/2;
        generateBVHTree(root->left,datas,start,mid);
        generateBVHTree(root->right,datas,mid+1,end);
    }

    root->box=surrounding_box(root->left->box,root->right->box);
}

bool bvh::hit(ray &r, hitRecord &h, float minT, float maxT)
{
    if(root==nullptr)
    {
        std::cout<<"error root is nullptr..."<<std::endl;
        return false;
    }
    return root->hit(r,h,minT,maxT);
}

bool bvh::x_compare(std::shared_ptr<primitiveBase> a, std::shared_ptr<primitiveBase> b)
{
    return a->aabbBox._min.x>b->aabbBox._min.x;
}

bool bvh::y_compare(std::shared_ptr<primitiveBase> a, std::shared_ptr<primitiveBase> b)
{
    return a->aabbBox._min.y>b->aabbBox._min.y;
}

bool bvh::z_compare(std::shared_ptr<primitiveBase> a, std::shared_ptr<primitiveBase> b)
{
     return a->aabbBox._min.z>b->aabbBox._min.z;
}
