#include "MyGL/GLErrors.h"

#include <GL/glew.h>

#include <iostream>


void GLClearError()
{
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
        ;
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "OpenGl ERROR:(0x" << std::hex << error << "): " << function <<
            " File:" << file << "(" << std::dec << line << ")" << std::endl;
        return false;
    }
    return true;
}
