#pragma once

#include "glm/glm.hpp"

#include <string>
#include <unordered_map>


class Shader
{
private:
    unsigned int m_rendererId;
    std::string m_filePath;
    mutable std::unordered_map<std::string, int> m_uniformLocationCache;

public:
    Shader(const std::string& filepath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    // Set uniform
    void SetUniform1i(const std::string& name, int value) const;
    void SetUniform1f(const std::string& name, float value) const;
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) const;
    void SetUniformMat4f(const std::string& name, const glm::mat4& matrix) const;

private:
    int GetUniformLocation(const std::string& name) const;
};