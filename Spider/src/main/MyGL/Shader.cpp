#include "MyGL/Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "MyGL/GLErrors.h"
#include "GL/glew.h"


namespace
{
    struct ShaderProgramSource
    {
        std::string VertexSource;
        std::string FragmentSource;
    };

    ShaderProgramSource ParseShader(const std::string& filepath)
    {
        std::ifstream stream(filepath);
        if (!stream.good())
        {
            std::cout << "Shader not found at: " << filepath << std::endl;
            throw std::exception("File Not Found");
        }
        std::string line;
        std::stringstream ss[2];
        enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };
        ShaderType currentType = ShaderType::NONE;

        while (getline(stream, line))
        {
            if (line.find("#shader") != std::string::npos)
            {
                if (line.find("vertex") != std::string::npos)
                    currentType = ShaderType::VERTEX;
                else if (line.find("fragment") != std::string::npos)
                    currentType = ShaderType::FRAGMENT;
            }
            else
            {
                ss[(int)currentType] << line << '\n';
            }
        }
        return { ss[0].str(), ss[1].str() };
    }

    GLuint CompileShader(GLenum type, const std::string& source)
    {
        GLuint id = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        // error checking
        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE)
        {
            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            char* error_msg = (char*)malloc((unsigned long)length * sizeof(char));
            std::unique_ptr<char> error_holder(error_msg);
            glGetShaderInfoLog(id, length, nullptr, error_msg);

            std::string typeString = (type == GL_VERTEX_SHADER) ? "vertex" : "fragment";
            std::cout << "ERROR compiling " << typeString << " shader: " << error_msg << std::endl;
            glDeleteShader(id);
            return 0;
        }

        return id;
    }

    GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
    {
        GLuint program = glCreateProgram();
        GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
        GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        glValidateProgram(program);

        glDeleteShader(vs);
        glDeleteShader(fs);
        return program;
    }
}



Shader::Shader(const std::string& filepath)
    :m_filePath(filepath)
    , m_rendererId(0)
{
    ShaderProgramSource source = ParseShader(filepath);
    m_rendererId = CreateShader(source.VertexSource, source.FragmentSource);
}




Shader::~Shader()
{
    GLCall(glDeleteProgram(m_rendererId));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_rendererId));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value) const
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value) const
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) const
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) const
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& name) const
{
    auto cacheHit = m_uniformLocationCache.find(name);
    if (cacheHit != m_uniformLocationCache.end())
        return cacheHit->second;
    int location = glGetUniformLocation(m_rendererId, name.c_str());
    if (location == -1)
        std::cout << "Warning: uniform '" << name << "' not found: " << std::endl;
    else
        m_uniformLocationCache[name] = location;
    return location;
}
