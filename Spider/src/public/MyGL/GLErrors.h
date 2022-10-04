#pragma once

#include <string>
#include <stdexcept>
#include <iostream>

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

#ifdef _DEBUG
#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) {GLClearError(); (x); ASSERT(GLLogCall(#x, __FILE__, __LINE__));}
#else
#define ASSERT(x) 
#define GLCall(x) x
#endif


template<typename ... Args>
void LOG_ERROR(const std::string & format, Args ... args)
{
    int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
    if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
    auto size = static_cast<size_t>(size_s);
    auto buf = std::make_unique<char[]>(size);
    std::snprintf(buf.get(), size, format.c_str(), args ...);
    std::string message = std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
    std::cout << message << std::endl;
}

#define LOG_INFO LOG_ERROR

#ifdef GLCORE_ENABLE_ASSERTS
#define GLCORE_ASSERT(x, ...) { if(!(x)) { LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define GLCORE_ASSERT(x, ...)
#endif
