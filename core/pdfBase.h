#ifndef PDFBASE_H
#define PDFBASE_H

#include "baseStructure.h"

class pdfBase
{
public:
	virtual glm::vec3 generate(const glm::vec3 & p, float roughnessA, float roughnessB) const = 0;
    //virtual glm::vec3 generate(const glm::vec3 & p) const  = 0;
    virtual float value(const glm::vec3 & direction) const = 0;
    virtual lightType type() const = 0;
};

#endif // PDF_H
