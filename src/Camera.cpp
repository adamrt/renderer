#include "Camera.h"
#include "AK.h"

Camera::Camera(i32 width, i32 height, f32 fov, f32 near, f32 far)
    : width(width)
    , height(height)
    , aspect((f32)height / width)
    , fov(fov)
    , near(near)
    , far(far)
{
    // The perspective projection matrix is static, so we can calculate it once.
    // The orthographic projection matrix is dynamic, so we need to calculate if distance changes.
    projection = Projection::Perspective;
    perspective = Mat4::perspective(fov, aspect, near, far);
};

Mat4 Camera::projection_matrix() const
{
    if (projection == Projection::Perspective) {
        return perspective;
    } else {
        return orthographic;
    }
}

Mat4 Camera::view_matrix() const
{
    return view;
}

void Camera::orbit(f32 dx, f32 dy)
{
    theta += dx;
    if (theta < 0.0f) {
        theta += 360.0f;
    }
    if (theta > 360.0f) {
        theta -= 360.0f;
    }

    const f32 min_phi = -85.0f;
    const f32 max_phi = 85.0f;
    phi = kclamp(phi + dy, min_phi, max_phi);
}

void Camera::zoom(f32 d)
{
    const f32 min_dist = 0.1f;
    const f32 max_dist = 25.0f;
    distance = kclamp(distance + d, min_dist, max_dist);
}

// I'm not sure if I understand the negation of x and z here, but it
// gives me the results I want. If they aren't negated, then phi=0,
// theta=0, distance=5 results in the camera being at (0,0,5) when we
// want (0,0,-5).
Vec3 Camera::calculate_eye() const
{
    f32 lat = kradians(phi);
    f32 lng = kradians(theta);

    const f32 x = -distance * std::cos(lat) * std::sin(lng);
    const f32 y = distance * std::sin(lat);
    const f32 z = -distance * std::cos(lat) * std::cos(lng);

    return Vec3 { x, y, z };
}

void Camera::update()
{
    const Vec3 up(0.0f, 1.0f, 0.0f);
    const Vec3 target(0.0f, 0.0f, 0.0f);
    const Vec3 pos = target + calculate_eye();

    view = Mat4::look_at(pos, target, up);

    // Perspective doesn't change, but orthographic does.
    // Orthographic needs to be recreated for zooming since
    // orthographic isn't affected by z position (zoomed out).
    // You have to change the fov via distance.

    if (projection == Projection::Orthographic) {
        // The (distance / 2.0) makes the ortho and perspective be
        // zoomed at roughly the same level.
        const auto w = 1.0 * (distance / 2.0);
        const auto h = 1.0 * aspect * (distance / 2.0);
        orthographic = Mat4::orthographic(-w, w, -h, h, near, far);
    }
}
