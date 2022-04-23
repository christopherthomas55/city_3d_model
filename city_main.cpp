#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <vector>
#include <iostream>
#include <math.h>

#include "src/shader.h"
#include "src/config.h"

#include "src/BuildingHandler.h"

// Boiler plate here
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
// End boiler plate


int main(int argc, char* argv[]){
    if(!glfwInit()){
        std::cout << "Failed glfw init";
        return -1;
    }

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
    gladLoadGL();

    glViewport(0, 0, WIND_WIDTH, WIND_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    if(!fill){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    glm::mat4 projection;
    glm::mat4 view;
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Non boiler plate
    BuildingHandler handler = BuildingHandler(buildings_path);
    std::vector<float> vertices = handler.getBuildingVector();
    std::vector<unsigned int> indices  = handler.getElementVector();
    std::vector<unsigned int> vertexCounter = handler.getCounterVector();
    std::vector<unsigned int> cumsumIndices = handler.getIndicesTracker();
    // Then boilerplate open gl again


    // Building sides
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices.size(), &indices[0], GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // Horribly inefficient start
    // Building tops
    unsigned int VBO2, VAO2, EBO2;
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    //glGenBuffers(1, &EBO2);

    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*handler.roof_vertices.size(), &handler.roof_vertices[0], GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices.size(), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // Building floor aka ground
    //unsigned int VBO3, VAO3;
    //glGenVertexArrays(1, &VAO3);
    //glGenBuffers(1, &VBO3);

    //glBindVertexArray(VAO3);
    //glBindBuffer(GL_ARRAY_BUFFER, VBO3);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(float)*handler.ground.size(), &handler.ground[0], GL_STATIC_DRAW);

    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);
    // End inefficient


    Shader ourShader("shaders/vertex_camera_no_color.vs", "shaders/red_color.fs");

    while(!glfwWindowShouldClose(window))
    {
        // Camera crap, should maybe be class
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

        unsigned int modelLoc = glGetUniformLocation(ourShader.getID(), "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        // End camera crap

        // Building sides are triangle strip for each side
        // Ground is just gl triangles
        glBindVertexArray(VAO);
        for(unsigned int buildingID=0; buildingID < vertexCounter.size(); buildingID++)
        {
            glDrawElements(GL_TRIANGLE_STRIP, vertexCounter[buildingID], GL_UNSIGNED_INT, (void*)(sizeof(unsigned int)*cumsumIndices[buildingID]));
        }

        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLES, 0, handler.roof_vertices.size()/3);

        //glBindVertexArray(VAO3);
        //glDrawArrays(GL_TRIANGLES, 0, handler.roof_vertices.size()/3);

        glfwSwapBuffers(window);
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

    }
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

    float cameraSpeed = 30.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraSpeed *= 10.0;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cameraSpeed *= 5.0;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cameraPos += cameraUp * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        cameraPos -= cameraUp * cameraSpeed;

    // This doesn't seem to work
    //if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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



