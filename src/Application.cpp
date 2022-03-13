#include <GLAD/glad.h>
#include <GLAD/glad.c>
#include <GLFW/glfw3.h>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Init.h"
#include "Camera.h"
#include "Chunk.h"
#include "Block.h"

const unsigned int SCREEN_WIDTH = 1600, SCREEN_HEIGHT = 900;
const unsigned int chunk_x_size = 32, chunk_y_size = 64, chunk_z_size = 32;

int main()
{
    GLFWwindow* window = Initialize(SCREEN_WIDTH, SCREEN_HEIGHT);

    unsigned int VAO, VBO;
    InitVAOVBO(VAO, VBO);

    Shader ourShader("res/Shaders/shader.vs", "res/Shaders/shader.fs");

    Chunk* chunk = new Chunk(chunk_x_size, chunk_y_size, chunk_z_size);
    chunk->Generate();
    chunk->CalculateSides();

    Camera cam(window, SCREEN_WIDTH, SCREEN_HEIGHT);
    ourShader.setMat4("projection", cam.GetProjectionMatrix());

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cam.Update();
        ourShader.setMat4("view", cam.GetViewMatrix());

        chunk->Render(ourShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}