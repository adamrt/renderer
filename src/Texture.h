#include <cstdint>

struct Texture {
public:
    Texture(const char* filename);
    ~Texture();

    // Prevent copy
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    int width() const { return m_width; }
    int height() const { return m_height; }
    int channels() const { return m_channels; }
    uint8_t* data() const { return m_data; }

private:
    uint8_t* m_data = nullptr;

    int m_width = 0;
    int m_height = 0;
    int m_channels = 0;
};
