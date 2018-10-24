#ifndef _PARAMETEREQUATION_H_
#define _PARAMETEREQUATION_H_

#include "EdgeEquation.h"

struct ParameterEquation
{
public:
	float a, b, c;

public:
	ParameterEquation(float p0, float p1, float p2, const EdgeEquation &e0, const EdgeEquation &e1, const EdgeEquation &e2, float);
	float evaluate(float x, float y) const;
};

#endif