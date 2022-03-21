#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

#include  <vector>
#include <iostream>
#include <math.h>

#define OBJ_WIDTH 800
#define OBJ_HEIGHT 800
#define WIND_WIDTH 1200
#define WIND_HEIGHT 900
#define DRAW_DISTANCE 5000.0f

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void clearColor(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

float yaw = -90.0f;
float pitch = 0.0f;
float fov = 45.0f;

glm::vec3 direction;
//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
//glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraPos = glm::vec3(0.0f, 50.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 50.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = 400, lastY = 300;
bool firstMouse = true;
bool fill = false;
bool animated = true;
bool points = false;


int main(int argc, char* argv[]){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIND_WIDTH, WIND_HEIGHT, "Playground", NULL, NULL);
    if (window == NULL)
    {
            std::cout << "Failed to create GLFW window" << std::endl;
                glfwTerminate();
                    return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
            std::cout << "Failed to initialize GLAD" << std::endl;
                return -1;
    }    

    glViewport(0, 0, WIND_WIDTH, WIND_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 


    // BEGIN VERTEX SHADER
    //float vertices[] = {
    //    // positions        // colors
    //    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
    //    0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
    //    0.0f,  0.5f, -0.5f,   0.0f, 1.0f, 1.0f,
    //    0.5f, 0.5f, 0.5f,    1.0f, 0.0f, 1.0f,
    //};  

    //unsigned int indices[] = {
    //    0, 1, 3,
    //    1, 2, 3
    //};

    float xwavelength = 10.0f;
    float xamplitude = 50.0f;

    float ywavelength = 30.0f;
    float yamplitude = 40.0f;
    std::vector<float> vertices;
    for(unsigned int i = 0; i<OBJ_HEIGHT; i++)
    {
        for(unsigned int j = 0; j<OBJ_WIDTH; j++)
        {
            vertices.push_back(-OBJ_HEIGHT/2.0f + i);
            vertices.push_back(xamplitude*cos(j/xwavelength) + yamplitude*cos(i/ywavelength));
            vertices.push_back(-OBJ_WIDTH/2.0f + j);

            // RGB
            vertices.push_back(cos(i/xwavelength));
            vertices.push_back(sin(j/ywavelength));
            vertices.push_back(0.5f*(cos(i/OBJ_HEIGHT) + sin(i/OBJ_WIDTH)));
        }
    }

    // Top triangles
    std::vector<unsigned int> indices;
    for(unsigned int i = 0; i<OBJ_HEIGHT-1; i++)
    {
        for(unsigned int j = 0; j<OBJ_WIDTH-2; j++)
        {
            indices.push_back(j + i*(OBJ_WIDTH));
            indices.push_back(j + i*(OBJ_WIDTH) + 1);

            indices.push_back(j + (i+1)*OBJ_WIDTH);
            indices.push_back(j + 1 + (i+1)*(OBJ_WIDTH));
        }
    }

    // * DELEER HERE WHEN MESH READY
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices.size(), &indices[0], GL_STATIC_DRAW);

    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    //DELETE HERE WHEN MESH END

    if(!fill){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    //Shader ourShader("shaders/vertex.vs", "shaders/color.fs");
    Shader ourShader("shaders/vertex_camera.vs", "shaders/color.fs");

    // perspective looking matrix
    glm::mat4 projection;
    glm::mat4 view;

    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    const unsigned int NUM_STRIPS = OBJ_HEIGHT - 1;
    const unsigned int NUM_VERTS_PER_STRIP = 4*(OBJ_WIDTH - 2);

    while(!glfwWindowShouldClose(window))
    {
        projection = glm::perspective(glm::radians(fov),  static_cast<float>(WIND_WIDTH) / static_cast<float>(WIND_HEIGHT), 0.1f, DRAW_DISTANCE);
        float currentFrame = glfwGetTime();
        float timeOffset = currentFrame/5.0f;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);

        clearColor(window);

        ourShader.use();


        glm::mat4 model = glm::mat4(1.0f);

        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(direction);

        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        unsigned int viewLoc = glGetUniformLocation(ourShader.getID(), "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        unsigned int projectionLoc = glGetUniformLocation(ourShader.getID(), "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));


        if(animated){
            for(unsigned int i = 0; i<OBJ_HEIGHT; i++)
            {
                for(unsigned int j = 0; j<OBJ_WIDTH; j++)
                {
                    // TODO - MESH.update(6*(OBJ_WIDTH*i + j) + 3, xamplitude*cos(timeOffset + j/xwavelength) + yamplitude*cos(timeOffset+i/ywavelength);
                    // TODO - MESH.update(6*(OBJ_WIDTH*i + j) + 3, cos(timeOffset + j/40.0f)
                    vertices[6*(OBJ_WIDTH*i + j) + 1] = xamplitude*cos(timeOffset + j/xwavelength) + yamplitude*cos(timeOffset+i/ywavelength);
                    vertices[6*(OBJ_WIDTH*i + j) + 3] = cos(timeOffset + j/40.0f);
                    //vertices[6*(OBJ_WIDTH*i + j) + 4] = cos(timeOffset + j/20.0f);
                    //vertices[6*(OBJ_WIDTH*i + j) + 5] = cos(timeOffset + j/7.0f);
                }
            }


        //TODO
        mesh.buffer();
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), &vertices[0], GL_STATIC_DRAW);
        }

        unsigned int modelLoc = glGetUniformLocation(ourShader.getID(), "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(VAO);

        //TODO
        mesh.draw();

        if (!points){
            for(unsigned int strip=0; strip < NUM_STRIPS; ++strip)
            {
                glDrawElements(GL_TRIANGLE_STRIP, NUM_VERTS_PER_STRIP, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int)*NUM_VERTS_PER_STRIP*strip));
            }
        } else 
        {
            glDrawElements(GL_POINTS, indices.size(), GL_UNSIGNED_INT, 0);

        }

        glfwSwapBuffers(window);
        glfwPollEvents();    
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(ourShader.getID());

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 20.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraSpeed *= 10.0;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void scroll_callback(GLFWwindow*, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
}

void clearColor(GLFWwindow *window)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
