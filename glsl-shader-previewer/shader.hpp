#ifndef SHADER_HPP
#define SHADER_HPP

#include "include.h"

string ReadFile(filesystem::path path)
{
    ifstream in(path, ios::binary);
    const uintmax_t size = filesystem::file_size(path);

    string result(size, '\0');
    in.read(result.data(), size);

    return result;
}

uint32_t CompileShader(uint32_t type, string& source)
{
    uint32_t id = glCreateShader(type);
    const char* raw = source.c_str();

    glShaderSource(id, 1, &raw, nullptr);
    glCompileShader(id);

    char log[1080];

    int success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(id, 512, nullptr, log);

        cout << "shader compilation error: \n" << log << endl;
    }

    return id;
}

uint32_t CreateShader(string& vertex, string& fragment)
{
    uint32_t vertexs = CompileShader(GL_VERTEX_SHADER, vertex);
    uint32_t fragments = CompileShader(GL_FRAGMENT_SHADER, fragment);

    uint32_t program = glCreateProgram();

    glAttachShader(program, vertexs);
    glAttachShader(program, fragments);
    glLinkProgram(program);
    glValidateProgram(program);

    char log[1089];

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(program, 512, nullptr, log);

        cout << "shader program linking error: \n" << log << endl;
    }

    glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 512, nullptr, log);

        cout << "shader program validation error: \n" << log << endl;
    }

    glDeleteShader(vertexs);
    glDeleteShader(fragments);

    return program;
}

#endif 