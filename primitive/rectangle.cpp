#include "rectangle.h"
#include "core/directionPdfAdaptor.h"
#include <algorithm>
extern bool debugFlag;

rectangle::rectangle(const glm::vec3 & _oriPos, const glm::vec3 & _wDirec, const glm::vec3 & _hDirec, float _w, float _h, std::shared_ptr<materialBase> _mat, bool flipNormal)
    :primitiveBase(_mat)
{
    pType = primitiveType::rectangle;
    this->oriPos = _oriPos;
    this->wDirec = _wDirec;
    this->hDirec = _hDirec;
    this->w = _w;
    this->h = _h;

    this->pa = oriPos;
    this->pb = oriPos + glm::normalize(wDirec) * w;
    this->pd = oriPos + glm::normalize(hDirec) * h;
    this->pc = pb + glm::normalize(hDirec) * h;
    this->n = glm::normalize(glm::cross(wDirec, hDirec));
	if (flipNormal)
        this->n = -this->n;

    glm::vec3 tmin{1.0};
    glm::vec3 tmax{1.0};

    tmin.x = std::min(std::min(pa.x, pb.x), std::min(pc.x, pd.x)) - epslion;
    tmin.y = std::min(std::min(pa.y, pb.y), std::min(pc.y, pd.y)) - epslion;
    tmin.z = std::min(std::min(pa.z, pb.z), std::min(pc.z, pd.z)) - epslion;

    tmax.x = std::max(std::max(pa.x, pb.x), std::max(pc.x, pd.x)) + epslion;
    tmax.y = std::max(std::max(pa.y, pb.y), std::max(pc.y, pd.y)) + epslion;
    tmax.z = std::max(std::max(pa.z, pb.z), std::max(pc.z, pd.z)) + epslion;

    this->aabbBox._min=tmin;
    this->aabbBox._max=tmax;

	float y = -(n.x / n.z);
	glm::vec3 xAxis{ 1.0f, 0.0f, y };
	xAxis = glm::normalize(xAxis);
	if (n.z < 0)
		xAxis = -xAxis;
	yAxis = glm::normalize(glm::cross(n, xAxis));
}

glm::vec3 rectangle::normal(const glm::vec3 &surPos)
{
    return glm::normalize(n);
}

glm::vec3 rectangle::reflect(const glm::vec3 &inDirec, const glm::vec3 &normal)
{
    return glm::normalize(inDirec - 2 * glm::dot(inDirec, normal)*normal);
}

std::vector<std::vector<float>> rectangle::getModelLinesAndColors()
{
    std::vector<float> verts;
    std::vector<float> colors;
    //first line
    pushData(verts, pa);
    pushData(verts, pb);
    pushData(colors, this->mat->tex->baseColor(0, 0, glm::vec3(0)));
    pushData(colors, this->mat->tex->baseColor(0, 0, glm::vec3(0)));

    //second line
    pushData(verts, pb);
    pushData(verts, pc);
    pushData(colors, this->mat->tex->baseColor(0, 0, glm::vec3(0)));
    pushData(colors, this->mat->tex->baseColor(0, 0, glm::vec3(0)));

    //third line
    pushData(verts, pc);
    pushData(verts, pd);
    pushData(colors, this->mat->tex->baseColor(0, 0, glm::vec3(0)));
    pushData(colors, this->mat->tex->baseColor(0, 0, glm::vec3(0)));

    //fourth line
    pushData(verts, pd);
    pushData(verts, pa);
    pushData(colors, this->mat->tex->baseColor(0, 0, glm::vec3(0)));
    pushData(colors, this->mat->tex->baseColor(0, 0, glm::vec3(0)));

    return {verts, colors};
}

void rectangle::pushData(std::vector<float> &v, const glm::vec3 &d)
{
    v.push_back(d.x);
    v.push_back(d.y);
    v.push_back(d.z);
}

bool rectangle::boxHit(const ray &r, float minT, float maxT)
{
    return aabbBox.hit(r,minT,maxT);
}

void rectangle::setTranslate(const glm::vec3 &trans)
{
    glm::mat4 translate(1.0f);
    translate = glm::translate(translate, trans);
    modelMatrix = translate * modelMatrix;
    invModelMatrix = glm::inverse(modelMatrix);
}

void rectangle::setRotate(const glm::vec3 &rotateAxis, float angle)
{
    glm::mat4 rotate(1.0f);
    rotate = glm::rotate(rotate, glm::radians(angle), rotateAxis);
    modelMatrix = rotate * modelMatrix;
    invModelMatrix = glm::inverse(modelMatrix);

    n = glm::vec3(rotate * glm::vec4(n, 0.0));
}

void rectangle::setUniformScale(float s)
{
    glm::mat4 scale(1.0);
    scale = glm::scale(scale, glm::vec3(s));
    modelMatrix = scale * modelMatrix;
    invModelMatrix = glm::inverse(modelMatrix);
}

void rectangle::setNonUniformScale(const glm::vec3 &s)
{
    glm::mat4 scale(1.0f);
    scale = glm::scale(scale, s);
    modelMatrix = scale * modelMatrix;
    invModelMatrix = glm::inverse(modelMatrix);

    //need to update normal
    glm::mat4 G = glm::transpose(invModelMatrix);
    n = glm::vec3(G * glm::vec4(n, 0.0));
}

void rectangle::setModelMatrix(const glm::mat4 &m)
{
    modelMatrix = m;
    invModelMatrix = glm::inverse(modelMatrix);
}

bool rectangle::rayRect(ray &r, float &u, float &v, float &t)
{
    //adapt from Moller-triangle algorithm
    //note here we must use pd - pa and pb - pa
    glm::vec3 E1 = pb - pa;
    glm::vec3 E2 = pd - pa;

    glm::vec3 P = glm::cross(r.direc, E2);
    float det = glm::dot(E1, P);

    glm::vec3 T;
    if(det > 0.0f)
        T = r.pos - pa;
    else
    {
        det = -det;
        T = pa - r.pos;
    }
    if(det < epslion)
        return false;

    u = glm::dot(T, P);
    if(u < 0.0f || u > det)
        return false;

    glm::vec3 Q = glm::cross(T, E1);
    v = glm::dot(r.direc, Q);
    if(v < 0.0f || v > det)
        return false;

    float invDet = 1.0 / det;
    t = glm::dot(E2, Q);
    if(t < epslion)
        return false;

    t *= invDet;
    u *= invDet;
    v *= invDet;

    return true;
}

bool rectangle::hit(ray &r, hitRecord &h, float minT, float maxT)
{
    if(!boxHit(r, minT, maxT))
        return false;

    float u, v, t;
    if(rayRect(r, u, v, t) && t > minT && t < maxT && t < h.t)
    {
        h.t = t;
        h.u = u;
        h.v = v;
        h.hitPos = r.pos + t * r.direc;

        h.hitNormal = normal(h.hitPos);
        h.hitReflect = reflect(r.direc, h.hitNormal);
        auto generateRes = directionPdfAdaptor::getInstance().generate(h.hitPos, h.hitNormal, h.hitRoughnessX, h.hitRoughnessY);
		h.hitInDirec = r.direc;
        h.hitOutDirec = glm::vec3(generateRes);

        //for refract's calculate
        if(mat->type == materialType::dielectrics)
        {
            glm::vec3 outNormal;
            float niOverNt;
            if(glm::dot(r.direc, h.hitNormal) > 0)
            {
                outNormal = - h.hitNormal;
                niOverNt = mat->niOverNt;
            }
            else
            {
                outNormal = h.hitNormal;
                niOverNt = 1.0 / mat->niOverNt;
            }
            if(! refract(r.direc, outNormal, niOverNt, h.hitRefract))
                h.hitRefract = h.hitReflect;

            h.hitOutDirec = h.hitRefract;
        }

        h.hitPdf = generateRes.w;
        h.matPtr = this->mat;
		h.invModel = this->invModelMatrix;
		h.xAxis = this->hitXAxis(h.hitPos);
		h.yAxis = this->hitYAxis(h.hitPos);

        return true;
    }
    else
        return false;
}

void rectangle::handleMatrix()
{
    pa = glm::vec3(modelMatrix * glm::vec4(pa, 1.0));
    pb = glm::vec3(modelMatrix * glm::vec4(pb, 1.0));
    pc = glm::vec3(modelMatrix * glm::vec4(pc, 1.0));
    pd = glm::vec3(modelMatrix * glm::vec4(pd, 1.0));

    //update aabb box
    this->aabbBox._min.x = std::min(std::min(pa.x, pb.x), std::min(pc.x, pd.x)) - epslion;
    this->aabbBox._min.y = std::min(std::min(pa.y, pb.x), std::min(pc.x, pd.x)) - epslion;
    this->aabbBox._min.z = std::min(std::min(pa.z, pb.z), std::min(pc.x, pc.z)) - epslion;

    this->aabbBox._max.x = std::max(std::max(pa.x, pb.x), std::max(pc.x, pd.x)) + epslion;
    this->aabbBox._max.y = std::max(std::max(pa.y, pb.y), std::max(pc.y, pd.y)) + epslion;
    this->aabbBox._max.z = std::max(std::max(pa.z, pb.z), std::max(pc.z, pd.z)) + epslion;
}

glm::vec3 rectangle::hitXAxis(const glm::vec3 & hitPos)
{
	//default x axis parallel to XOZ
	return xAxis;
}

glm::vec3 rectangle::hitYAxis(const glm::vec3 & hitPos)
{
	//y axis is related with x axis
	return yAxis;
}