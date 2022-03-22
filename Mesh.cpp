#include "Mesh.h"

Mesh::Mesh(std::vector<float>& vertices, std::vector<unsigned int>& indices)
{

    std::cout << "2";
    indices = indices;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), &vertices[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices.size(), &indices[0], GL_DYNAMIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
};

void Mesh::swap_buffer(std::vector<float> &active_vertices){
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*active_vertices.size(), &active_vertices[0], GL_DYNAMIC_DRAW);
};

void Mesh::draw(){
    glBindVertexArray(VAO);

    if (!points){
        for(unsigned int strip=0; strip < NUM_STRIPS; ++strip)
        {
            glDrawElements(GL_TRIANGLE_STRIP, NUM_VERTS_PER_STRIP, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int)*NUM_VERTS_PER_STRIP*strip));
        }
    } else 
    {
        glDrawElements(GL_POINTS, indices.size(), GL_UNSIGNED_INT, 0);

    }
}
