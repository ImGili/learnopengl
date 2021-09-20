/*
 * @Author: ImGili
 * @Description: 
 */
#pragma once
#include "Engine.h"
namespace mGUI
{
    void Init()
    {
        Window *window = Window::getWindow();
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        ImFont *font = io.Fonts->AddFontFromFileTTF("./fonts/楷体_GB2312.ttf", 15.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());
        io.Fonts->GetGlyphRangesChineseSimplifiedCommon();
        // io.Fonts->GetGlyphRangesChineseFull();

        // Setup Dear ImGui style
        ImGui::StyleColorsLight();
        //ImGui::StyleColorsClassic();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window->getGLFWwindow(), true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }

    void NewFrame()
    {
        // imgui
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    

    void RenderGUI()
    {
        ImGui::Render();
    }

    void DrawText(const char* c)
    {
        ImGui::Begin("test");
        ImGui::Text(c);
        ImGui::End();
    }

    void DrawRenderData()
    {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void DestroyGUI()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}