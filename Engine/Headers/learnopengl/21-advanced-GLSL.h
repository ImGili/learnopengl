#pragma once
#include "Engine.h"

#include <fstream>
namespace glx0211
{
   
    int main()
    {
        Window *window = Window::getWindow();
        CameraInstance *camera = CameraInstance::GetCamera();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        // Drawable* drawable =    (new Drawable())\
        //                         ->SetDrawlayout(TextureDrawlayout)\
        //                         ->SetVertexLayout(VertexTexcoordlayout)\
        //                         ->SetVertexNum(36)\
        //                         ->SetVertexFromData("./data/21/l1/")\
        //                         ->SetShader("./EngineShaders/VertexTexcoord/ObjectVertex.vert", "./EngineShaders/VertexTexcoord/ObjectFragment.frag")\
        //                         ->SetTextureId("imgs/woodPicture.jpeg");
        Drawable *cube = new Cube();
        Drawable *mmodel = new mModel();
        // float vertices[] = {
        //     -0.5f, -0.5f, 0.0f, // left
        //     0.5f, -0.5f, 0.0f,  // right
        //     0.0f, 0.5f, 0.0f    // top
        // };
        // Drawable *drawable = (new Drawable())->SetDrawTypes(DrawTypes::POINTS)->SetVertexNum(3)->SetVertex(&vertices, Vertexlayout, sizeof(vertices))->SetShader("./21/l1/ObjectVertex.vert", "./21/l1/ObjectFragment.frag")->SetModle(glm::mat4(1));
        // Drawable *drawable = (new Drawable())->SetDrawTypes(DrawTypes::POINTS)->SetVertexNum(4)->SetVertexFromData("./data/21/l1/")->SetShader("./21/l1/ObjectVertex.vert", "./21/l1/ObjectFragment.frag")->SetModle(glm::mat4(1));
        Drawable *drawable = (new Drawable())->SetDrawTypes(DrawTypes::POINTS)->SetVertexLayout(VertexNormallayout)->SetVertexNum(4)->SetVertexFromData("./data/21/l1/")->SetShader("./21/l1/ObjectVertex.vert", "./21/l1/ObjectFragment.frag")->SetModle(glm::mat4(1));

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
            // fbo.SetFrameBuffer();
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
            drawable->Draw();
            // cubes
            // cube->Draw();
            cube->DrawObject(obj);

            // floor
            plane.Draw();

            // fbo.SetMainFrameBuffer();
            // fbo.Draw();

            mGUI::DrawRenderData();
            window->SwapBufferAndPollEvents();
        }

        delete obj;
        delete cube;
        delete mmodel;
        mGUI::DestroyGUI();
        Window::DestoryWindow();
        CameraInstance::DestoryCamera();
        return 0;
    }

}
namespace glx0212
{
   
    int main()
    {
        Window *window = Window::getWindow();
        CameraInstance *camera = CameraInstance::GetCamera();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        Drawable* drawable =    (new Drawable())\
                                ->SetDrawlayout(TextureDrawlayout)\
                                ->SetVertexLayout(VertexTexcoordlayout)\
                                ->SetVertexNum(36)\
                                ->SetVertexFromData("./data/21/l2/")\
                                ->SetShader("./EngineShaders/VertexTexcoord/ObjectVertex.vert", "./EngineShaders/VertexTexcoord/ObjectFragment.frag")\
                                ->SetModle(glm::mat4(1))\
                                ->SetTextureId("imgs/woodPicture.jpeg");
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
            // fbo.SetFrameBuffer();
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
            drawable->Draw();
            // cubes
            // cube->Draw();
            // cube->DrawObject(obj);

            // floor
            plane.Draw();

            // fbo.SetMainFrameBuffer();
            // fbo.Draw();

            mGUI::DrawRenderData();
            window->SwapBufferAndPollEvents();
        }

        delete obj;
        delete cube;
        delete mmodel;
        mGUI::DestroyGUI();
        Window::DestoryWindow();
        CameraInstance::DestoryCamera();
        return 0;
    }

}

        
namespace glx0213
{
   
    int main()
    {
        Window *window = Window::getWindow();
        CameraInstance *camera = CameraInstance::GetCamera();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        Drawable* drawable =    (new Drawable())\
                                ->SetVertexLayout(Vertexlayout)\
                                ->SetVertexNum(36)\
                                ->SetVertexFromData("./data/21/l2/")\
                                ->SetShader("./21/l2/ObjectVertex.vert", "./21/l2/ObjectFragment.frag")\
                                ->SetModle(glm::mat4(1));
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
            // fbo.SetFrameBuffer();
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
            drawable->Draw();
            // cubes
            // cube->Draw();
            // cube->DrawObject(obj);

            // floor
            plane.Draw();

            // fbo.SetMainFrameBuffer();
            // fbo.Draw();

            mGUI::DrawRenderData();
            window->SwapBufferAndPollEvents();
        }

        delete obj;
        delete cube;
        delete mmodel;
        mGUI::DestroyGUI();
        Window::DestoryWindow();
        CameraInstance::DestoryCamera();
        return 0;
    }

}


namespace glx0214
{
   
    int main()
    {
        Window *window = Window::getWindow();
        CameraInstance *camera = CameraInstance::GetCamera();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        Drawable* drawable =    (new Drawable())\
                                ->SetVertexLayout(VertexTexcoordlayout)\
                                ->SetVertexNum(36)\
                                ->SetVertexFromData("./data/21/l2/")\
                                ->SetShader("./21/l2/ObjectVertex.vert", "./21/l2/ObjectFragment.frag")\
                                ->SetModle(glm::mat4(1));
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
            // fbo.SetFrameBuffer();
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
            drawable->Draw();
            // cubes
            // cube->Draw();
            // cube->DrawObject(obj);

            // floor
            plane.Draw();

            // fbo.SetMainFrameBuffer();
            // fbo.Draw();

            mGUI::DrawRenderData();
            window->SwapBufferAndPollEvents();
        }

        delete obj;
        delete cube;
        delete mmodel;
        mGUI::DestroyGUI();
        Window::DestoryWindow();
        CameraInstance::DestoryCamera();
        return 0;
    }

}