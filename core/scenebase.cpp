#include "scenebase.h"
#include <string>
using std::cout;
using std::endl;

sceneBase::sceneBase()
{
}

sceneBase::~sceneBase()
{
}

shared_ptr<textureBase> sceneBase::getTexture(string name)
{
    if(textures.find(name) != textures.end())
        return textures[name];
    else
    {
        cout<<"texture: "<<name<<" can't found";
        return NULL;
    }
}

shared_ptr<materialBase> sceneBase::getMatrial(string name)
{
    if(materials.find(name) != materials.end())
        return materials[name];
    else
    {
        cout<<"material: "<<name<<" can't found";
        return NULL;
    }
}

shared_ptr<primitiveBase> sceneBase::getObject(string name)
{
    if(objects.find(name) != objects.end())
        return objects[name];
    else
    {
        cout<<"object: "<<name<<" can't found";
        return NULL;
    }
}


unordered_map<string, shared_ptr<primitiveBase>> sceneBase::getAllObjects()
{
    return objects;
}
