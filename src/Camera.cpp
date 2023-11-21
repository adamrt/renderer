#include "Camera.h"
#include "AK.h"

Camera::Camera(UI& ui)
    : m_ui(ui)
{
    // The perspective projection matrix is static, so we can calculate it once.
    // The orthographic projection matrix is dynamic, so we need to calculate if distance changes.
    f32 fov = K_PI / 3.0f;
    static_perspective = Mat4::perspective(fov, aspect, ZNEAR, ZFAR);
};

void Camera::orbit(f32 dx, f32 dy)
{
    m_ui.camera_theta += dx;
    if (m_ui.camera_theta < 0.0f) {
        m_ui.camera_theta += 360.0f;
    }
    if (m_ui.camera_theta > 360.0f) {
        m_ui.camera_theta -= 360.0f;
    }

    const f32 min_phi = -85.0f;
    const f32 max_phi = 85.0f;
    m_ui.camera_phi = kclamp(m_ui.camera_phi + dy, min_phi, max_phi);
}

void Camera::zoom(f32 d)
{
    const f32 min_dist = 2.5f;
    const f32 max_dist = 25.0f;
    distance = kclamp(distance + d, min_dist, max_dist);
    update();
}

Vec3 Camera::eye() const
{
    f32 lat = kradians(m_ui.camera_phi);
    f32 lng = kradians(m_ui.camera_theta);

    const f32 x = distance * std::cos(lat) * std::sin(lng);
    const f32 y = distance * std::sin(lat);
    const f32 z = distance * std::cos(lat) * std::cos(lng);

    return Vec3 { x, y, z };
}

void Camera::update()
{
    view_matrix = Mat4::look_at(eye(), Vec3 { 0.0f, 0.0f, 0.0f }, Vec3 { 0.0f, 1.0f, 0.0f });

    if (m_ui.projection == Projection::Perspective) {
        proj_matrix = static_perspective;
    } else {
        // Zooming doesn't work for orthographic, so we adjust the
        // FOV.  The (distance / 2.0) makes the ortho and perspective
        // be zoomed at roughly the same level.
        const auto w = 1.0 * (distance / 2.0);
        const auto h = 1.0 * aspect * (distance / 2.0);
        proj_matrix = Mat4::orthographic(-w, w, -h, h, ZNEAR, ZFAR);
    }
}
