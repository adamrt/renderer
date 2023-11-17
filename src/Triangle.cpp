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

// should_cull returns a whether a triangle should be backface culled.
//
// This checks the winding order of the triangle.  If its CCW then you
// can ignore this triangle since it would be back-facing.  This
// method must be done after the vertices have been projected.
bool Triangle::should_cull() const
{
    Vec2 a = points[0];
    Vec2 b = points[1];
    Vec2 c = points[2];

    Vec2 ab = b - a;
    Vec2 ac = c - a;

    float sign = ab.x * ac.y - ac.x * ab.y;
    return sign < 0;
}
