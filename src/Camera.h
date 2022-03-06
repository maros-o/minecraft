#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <iomanip>

const float SPEED = 0.1f;
const float SENSITIVITY = 0.1f;

const float POV = 70.0f;
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float MAX_DRAW_DISTANCE = 200.0f;
const float MIN_DRAW_DISTANCE = 0.1f;

//fps
double prevTime = 0.0;
double currTime = 0.0;
double timeDiff = 0.0;
double delayer = 0.0;
unsigned int counter = 0;

unsigned int culling_cooldown = 0;

class Camera
{
public:
    Camera(GLFWwindow* window, const int SCREEN_WIDTH, const int SCREEN_HEIGHT)
    {
        m_position = glm::vec3(0.0f, 0.0f, 0.0f);
        m_worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        m_yaw = YAW;
        m_pitch = PITCH;
        m_front = glm::vec3(0.0f, 0.0f, -1.0f);
        m_movementSpeed = SPEED;
        m_mouseSensitivity = SENSITIVITY;
        m_window = window;
        m_lastX = (float)SCREEN_WIDTH / 2;
        m_lastY = (float)SCREEN_HEIGHT / 2;
        m_deltaTime = 0.0f;
        m_lastFrame = 0.0f;
        m_culling = true;

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        updateCameraVectors();
    }

    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(m_position, m_position + m_front, m_up);
    }

    glm::mat4 GetProjectionMatrix(const int SCREEN_WIDTH, const int SCREEN_HEIGHT)
    {
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(POV), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, MIN_DRAW_DISTANCE, MAX_DRAW_DISTANCE);
        return projection;
    }

    void ProcessKeyboard()
    {
        float currentFrame = (float)glfwGetTime();
        m_deltaTime = currentFrame - m_lastFrame;
        m_lastFrame = currentFrame;

        float velocity = m_movementSpeed * m_deltaTime;

        if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) m_position -= glm::normalize(glm::cross(m_front, m_up)) * SPEED;
        if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) m_position += glm::normalize(glm::cross(m_front, m_up)) * SPEED;
        if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) m_position += m_front * SPEED;
        if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) m_position -= m_front * SPEED;

        if (glfwGetKey(m_window, GLFW_KEY_C) == GLFW_PRESS && culling_cooldown > 20) {
            culling_cooldown = 0;
            if (m_culling) {
                glDisable(GL_CULL_FACE);
                m_culling = false;
            }
            else {
                glEnable(GL_CULL_FACE);
                m_culling = true;
            }
        }
        else culling_cooldown++;
    }

    void PrintFPS() {
        currTime = glfwGetTime();
        timeDiff = currTime - prevTime;
        counter++;
        if (timeDiff > 1.0 / 30.0) {
            if (delayer < currTime) {
                delayer = currTime + 0.2;
                std::stringstream ss;
                ss << std::fixed << int(1.0 / timeDiff * counter) << " FPS  |  " << std::setprecision(0) << timeDiff / counter * 1000 << " ms";
                if (m_culling) ss << "  |  Face Culling: ON";
                else ss << "  |  Face Culling: OFF";
                glfwSetWindowTitle(m_window, ss.str().c_str());
            }
            prevTime = currTime;
            counter = 0;
        }
    }

    void ProcessMouseMovement()
    {
        double tempX, tempY;
        glfwGetCursorPos(m_window, &tempX, &tempY);
        float mouseXpos = (float)tempX;
        float mouseYpos = (float)tempY;

        float xoffset = mouseXpos - m_lastX;
        float yoffset = m_lastY - mouseYpos;
        m_lastX = mouseXpos;
        m_lastY = mouseYpos;

        xoffset *= m_mouseSensitivity;
        yoffset *= m_mouseSensitivity;

        m_yaw += xoffset;
        m_pitch += yoffset;

        if (m_pitch > 89.0f)
            m_pitch = 89.0f;
        if (m_pitch < -89.0f)
            m_pitch = -89.0f;

        updateCameraVectors();
    }

    void ProcessInput()
    {
        ProcessKeyboard();
        ProcessMouseMovement();
        PrintFPS();
    }

private:
    GLFWwindow* m_window;
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp;

    float m_yaw;
    float m_pitch;

    float m_movementSpeed;
    float m_mouseSensitivity;

    float m_lastX;
    float m_lastY;
    float m_deltaTime;
    float m_lastFrame;
    bool m_culling;
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new m_front vector
        glm::vec3 front = glm::vec3();
        front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        front.y = sin(glm::radians(m_pitch));
        front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        m_front = glm::normalize(front);
        // also re-calculate the m_right and m_up vector
        m_right = glm::normalize(glm::cross(m_front, m_worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        m_up = glm::normalize(glm::cross(m_right, m_front));
    }
};