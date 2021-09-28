#pragma once
#include "Engine.h"

namespace glx0211
{

    int main()
    {
        Window *window = Window::getWindow();
        CameraInstance *camera = CameraInstance::GetCamera();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        Drawable *cube = new Cube();
        Drawable *mmodel = new mModel();

        Plane plane;
        SkyBox skybox;
        FrameBufferObject fbo;

        Object *obj = new Object(glm::vec3(1, 1, 1));

        mGUI::Init();
        // render loop
        // -----------
        while (!window->WindowShouldClose())
        {
            window->UpdateDeltaTime();
            // input
            // -----
            window->processInput();

            // render
            // ------
            fbo.SetFrameBuffer();
            window->Clear();

            mGUI::NewFrame();
            {
                ImGui::Begin("myGUI");
                ImGui::InputFloat3("Objet position", (float *)&(obj->Position));
                ImGui::End();
            }

            mGUI::RenderGUI();

            skybox.Draw();
            mmodel->Draw();
            // cubes
            // cube->Draw();
            cube->DrawObject(obj);
            

            // floor
            plane.Draw();

            fbo.SetMainFrameBuffer();
            fbo.Draw();

            mGUI::DrawRenderData();
            window->SwapBufferAndPollEvents();
        }

        delete obj, cube, mmodel;
        mGUI::DestroyGUI();
        Window::DestoryWindow();
        CameraInstance::DestoryCamera();
        return 0;
    }

}