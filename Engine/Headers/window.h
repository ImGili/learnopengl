/*
 * @Author: ImGili
 * @Description: 窗口类封装
 */
#pragma once
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "camera.hpp"

class Window
{
private:
    GLFWwindow *_window;

    static Window *Instance;

    // 禁止构造 禁止复制
    Window();
    Window &operator=(const Window &);

    bool isMouseEnalbe = true;
    bool firstMouse = true;
    float lastX = (float)SCR_WIDTH / 2.0;
    float lastY = (float)SCR_HEIGHT / 2.0;
    

public:
    ~Window()
    {
        glfwTerminate();
    }

    static void DestoryWindow()
    {
        delete Instance;
        Instance = NULL;
    }
    
    // settings
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;
    // timing
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    static Window *getWindow();

    void processInput();
    inline bool WindowShouldClose()
    {
        return glfwWindowShouldClose(_window);
    }

    inline void SwapBufferAndPollEvents()
    {
        glfwSwapBuffers(_window);
        glfwPollEvents();
    }

    inline void UpdateDeltaTime()
    {
        deltaTime = glfwGetTime() - lastFrame;
        lastFrame = glfwGetTime();
    }
    

private:
    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
    static void mouse_callback(GLFWwindow *window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
};
