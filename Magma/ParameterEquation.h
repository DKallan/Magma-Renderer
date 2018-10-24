#ifndef _PARAMETEREQUATION_H_
#define _PARAMETEREQUATION_H_

#include "EdgeEquation.h"

struct ParameterEquation
{
public:
	float a, b, c;

public:
	ParameterEquation(float, float, float, const EdgeEquation &e0, const EdgeEquation &e1, const EdgeEquation &e2, float);
	float evaluate(float, float) const;
};

#endif