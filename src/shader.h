#ifndef SHADER_H
#define SHADER_H


#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>



class Shader
{
public:
    // Shader programID from opengl
    unsigned int ID;

    // Requires onevertex and one fragment
    Shader(const char* vertexPath, const char* fragmentPath);

    // activate
    void use();

    // I don't really see the need
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

    unsigned int getID();

private:

};
#endif


