#pragma once


#include <string>


class Texture
{
private:
    unsigned int m_rendererId;
    std::string m_filepath;
    unsigned char* m_localBuffer;
    int m_height;
    int m_width;
    int m_BPP;

public:
    Texture(const std::string& path);
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    int GetWidth();
    int GetHeight();

};