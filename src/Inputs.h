#pragma once

struct Mouse {
    float xpos;
    float ypos;
    bool left_button;
};

struct Keyboard {
    bool W;
    bool S;
    bool A;
    bool D;
    bool SPACE;
};

void processInput(GLFWwindow* window, Mouse& mouse, Keyboard& keyboard)
{
    bool logInput = false;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) keyboard.W = true;
    else keyboard.W = false;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) keyboard.S = true;
    else keyboard.S = false;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) keyboard.A = true;
    else keyboard.A = false;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) keyboard.D = true;
    else keyboard.D = false;

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) keyboard.SPACE = true;
    else keyboard.SPACE = false;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) mouse.left_button = true;
    else mouse.left_button = false;

    double tempX, tempY;
    glfwGetCursorPos(window, &tempX, &tempY);
    mouse.xpos = (float)tempX;
    mouse.ypos = (float)tempY;

    if (logInput) {
        system("CLS");
        std::cout << "mouse X:" << mouse.xpos << ", Y:" << mouse.ypos << std::endl;
        std::cout << "mouse left button: " << mouse.left_button << std::endl;
        std::cout << "keyboard W: " << keyboard.W << std::endl;
        std::cout << "keyboard S: " << keyboard.S << std::endl;
        std::cout << "keyboard A: " << keyboard.A << std::endl;
        std::cout << "keyboard D: " << keyboard.D << std::endl;
        std::cout << "keyboard SPACE: " << keyboard.SPACE << std::endl;
        std::cout << "-----------------------------" << std::endl;
    }
}