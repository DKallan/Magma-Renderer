#include "EdgeEquation.h"

EdgeEquation::EdgeEquation(const glm::vec2 v0, const glm::vec2 v1)
{
	a = v0.y - v1.y;
	b = v1.x - v0.x;
	c = -(a * (v0.x + v1.x) + b * (v0.y + v1.y)) / 2;
	tie = a != 0 ? a > 0 : b > 0;
}

float EdgeEquation::evaluate(int x, int y) const
{
	return a * x + b * y + c;
}

bool EdgeEquation::test(float x, float y) const
{
	return test(evaluate(x, y));
}

bool EdgeEquation::test(float v) const
{
	return (v > 0 || v == 0 && tie);
}
