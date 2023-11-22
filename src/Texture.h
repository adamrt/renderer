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

    int width() const { return m_width; }
    int height() const { return m_height; }
    int channels() const { return m_channels; }
    u8* data() const { return m_data; }

private:
    u8* m_data = nullptr;

    int m_width = 0;
    int m_height = 0;
    int m_channels = 0;
};
