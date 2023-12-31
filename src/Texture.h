#pragma once

#include "AK.h"

struct Texture {
public:
    Texture(const char* filename);
    Texture() {};
    ~Texture();

    // Prevent copy
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    bool is_valid() const { return m_data != nullptr; }

    i32 width() const { return m_width; }
    i32 height() const { return m_height; }
    i32 channels() const { return m_channels; }
    u8* data() const { return m_data; }

private:
    u8* m_data { nullptr };

    i32 m_width {};
    i32 m_height {};
    i32 m_channels {};
};
