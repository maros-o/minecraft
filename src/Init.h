#pragma once

const float block_vertices[] = {
    //Position          //Texture
//--------------------------------- front
 0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
 0.5f, -0.5f, -0.5f,    1.0f, 0.0f,
-0.5f, -0.5f, -0.5f,    0.0f, 0.0f,
-0.5f, -0.5f, -0.5f,    0.0f, 0.0f,
-0.5f,  0.5f, -0.5f,    0.0f, 1.0f,
 0.5f,  0.5f, -0.5f,    1.0f, 1.0f,

 //--------------------------------- back
 -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
  0.5f, -0.5f,  0.5f,    1.0f, 0.0f,
  0.5f,  0.5f,  0.5f,    1.0f, 1.0f,
  0.5f,  0.5f,  0.5f,    1.0f, 1.0f,
 -0.5f,  0.5f,  0.5f,    0.0f, 1.0f,
 -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,

 //--------------------------------- left
 -0.5f,  0.5f,  0.5f,    0.0f, 1.0f,
 -0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
 -0.5f, -0.5f, -0.5f,    1.0f, 0.0f,
 -0.5f, -0.5f, -0.5f,    1.0f, 0.0f,
 -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
 -0.5f,  0.5f,  0.5f,    0.0f, 1.0f,

 //--------------------------------- right
  0.5f, -0.5f, -0.5f,    1.0f, 0.0f,
  0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
  0.5f,  0.5f,  0.5f,    0.0f, 1.0f,
  0.5f,  0.5f,  0.5f,    0.0f, 1.0f,
  0.5f, -0.5f,  0.5f,    0.0f, 0.0f,
  0.5f, -0.5f, -0.5f,    1.0f, 0.0f,

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

GLFWwindow* Initialize(unsigned int const SCREEN_WIDTH, unsigned int const SCREEN_HEIGHT)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
    std::cout << glGetString(GL_VERSION) << std::endl;
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CW);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    return window;
}

void InitVAOVBO(unsigned int& VAO, unsigned int& VBO) {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(block_vertices), block_vertices, GL_STATIC_DRAW);

    // position atr      id, size, type,            pos + tex size,   start offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture coord atr
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}