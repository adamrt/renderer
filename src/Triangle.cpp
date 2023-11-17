#include "Triangle.h"

Triangle::Triangle()
    : points({})
    , color(0xFF0000FF)
{
    points.reserve(3);
}

Triangle::Triangle(Vec2 a, Vec2 b, Vec2 c, uint32_t color)
    : points({ a, b, c })
    , color(color)
{
}
