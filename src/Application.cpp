#include <GLAD/glad.h>
#include <GLAD/glad.c>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cstdint>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Init.h"
#include "Camera.h"

unsigned const int SCREEN_WIDTH = 1600;
unsigned const int SCREEN_HEIGHT = 900;

int main(void)
{
    GLFWwindow* window = Initialize(SCREEN_WIDTH, SCREEN_HEIGHT);

    Shader ourShader("res/Shaders/shader.vs", "res/Shaders/shader.fs");

    float vertices[] = {
            //Position          //Texture
        //--------------------------------- front
         0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,    1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,     1.0f, 1.0f,

        //--------------------------------- back
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,    1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 1.0f,

         0.5f,  0.5f,  0.5f,    1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,    0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,

        //--------------------------------- left
        -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,

        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,

        //--------------------------------- right
         0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,    1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,    0.0f, 1.0f,

         //-------------------------------- bot
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,    1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,    1.0f, 0.0f,

         0.5f, -0.5f,  0.5f,    1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,

        //--------------------------------- top
         0.5f,  0.5f,  0.5f,    1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,    0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 0.0f,
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
                       //id, size, type,            pos + tex size,   start offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // load and create a texture 
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("res/textures/dirt512.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


    Camera cam(window, SCREEN_WIDTH, SCREEN_HEIGHT);

    ourShader.use(); // don't forget to activate/use the shader before setting uniforms
    ourShader.setInt("texture1", 0);
    ourShader.setMat4("projection", cam.GetProjectionMatrix(SCREEN_WIDTH, SCREEN_HEIGHT));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    const uint16_t x_size = 16, z_size = 16, y_size = 32;

    auto chunk = new bool[x_size][y_size][z_size];

    for (uint16_t x = 0; x < x_size; x++) {
        for (uint16_t y = 0; y < y_size; y++) {
            for (uint16_t z = 0; z < z_size; z++) {
                chunk[x][y][z] = true;
            }
        }
    }

    while (!glfwWindowShouldClose(window)) //game loop
    {
        cam.ProcessInput();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // activate shader
        ourShader.use();
        ourShader.setMat4("view", cam.GetViewMatrix());

        glBindVertexArray(VAO);

        for (uint16_t x = 0; x < x_size; x++) {
            for (uint16_t y = 0; y < y_size; y++) {
                for (uint16_t z = 0; z < z_size; z++) {

                    if (chunk[x][y][z]) {
                        glm::mat4 model = glm::mat4(1.0f);
                        model = glm::translate(model, glm::vec3((float)x, (float)y, (float)z));
                        ourShader.setMat4("model", model);
                    }

                    if (z == 0)
                        glDrawArrays(GL_TRIANGLES, 0, 6); //front
                    if (z == z_size - 1)
                        glDrawArrays(GL_TRIANGLES, 6, 6); //back
                    if (x == 0)
                        glDrawArrays(GL_TRIANGLES, 12, 6); //left
                    if (x == x_size - 1)
                        glDrawArrays(GL_TRIANGLES, 18, 6); //right
                    if (y == 0)
                        glDrawArrays(GL_TRIANGLES, 24, 6); //bot
                    if (y == y_size - 1)
                        glDrawArrays(GL_TRIANGLES, 30, 6); //top
                }
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}