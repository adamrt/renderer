#include <array>

#include "Color.h"
#include "Triangle.h"

Triangle::Triangle()
    : points({})
    , color(Color::Red)
{
    points.reserve(3);
}

Triangle::Triangle(Vec2 a, Vec2 b, Vec2 c, Color color)
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

// normal returns the normal of three vertices.  We do this calc
// before projection so we can't use it as a method on Triangle.
Vec3 vertices_normal(const std::array<Vec3, 3>& points)
{
    Vec3 a = points[0];
    Vec3 b = points[1];
    Vec3 c = points[2];

    Vec3 ab = b - a;
    Vec3 ac = c - a;

    Vec3 normal = Vec3::cross(ab, ac);
    return normal.normalized();
}
