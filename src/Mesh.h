#ifndef MESH_H
#define MESH_H


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "config.h"
#include  <vector>


class Mesh
{
public:
    std::vector<float> indices;

    unsigned int VBO, VAO, EBO;

    Mesh(std::vector<float>& vertices, std::vector<unsigned int>& indices);

    void swap_buffer(std::vector<float>& active_vertices);
    void draw();

private:

};
#endif


