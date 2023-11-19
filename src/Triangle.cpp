#include "Triangle.h"
#include "Color.h"

Triangle::Triangle()
    : points({})
    , color(Color::Red)
{
    points.reserve(3);
}

Triangle::Triangle(Vec4 a, Vec4 b, Vec4 c, Color color)
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
    Vec4 a = points[0];
    Vec4 b = points[1];
    Vec4 c = points[2];

    Vec4 ab = b - a;
    Vec4 ac = c - a;

    float sign = ab.x * ac.y - ac.x * ab.y;
    return sign < 0;
}
