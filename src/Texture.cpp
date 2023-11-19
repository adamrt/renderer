#include <iostream>
#include <stdexcept>

#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const char* filename)
{
    stbi_set_flip_vertically_on_load(true);
    m_data = stbi_load(filename, &m_width, &m_height, &m_channels, 0);
    if (m_data == NULL) {
        throw std::runtime_error("Error loading image");
    }
}

Texture::~Texture()
{
    if (m_data) {
        stbi_image_free(m_data);
    }
}
