#ifndef _EDGEEQUATION_H_
#define _EDGEEQUATION_H_

#include "glm.hpp"

struct EdgeEquation
{
public:
	float a, b, c;
	bool tie;

public:
	EdgeEquation(const glm::vec2, const glm::vec2);

	float evaluate(int x, int y) const;
	bool test(float x, float y) const;
	bool test(float v) const;
};

#endif