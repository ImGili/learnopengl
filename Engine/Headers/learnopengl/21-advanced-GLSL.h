#pragma once
#include "Engine.h"

#include <fstream>
namespace glx0211
{
    void SetVertexFromData(std::string dataPath, int vn)
    {
        std::vector<float> positions;
        float* normals;
        float* texcoord;
        std::string positionsPath, normalsPath, texcoordPath;
        positionsPath = dataPath + "positions.txt";
        // normalsPath = dataPath + "normal.txt";
        // texcoordPath = dataPath + "texcoord.txt";
        
        std::ifstream positionsIn(positionsPath.c_str());
        // std::ifstream normalsIn(normalsPath.c_str());
        // std::ifstream texcoordIn(texcoordPath.c_str());
        int i = 0;
        float tmp;
        while (positionsIn>>tmp)
        {
            i++;
            positions.push_back(tmp);
            std::cout << tmp << std::endl;
        }
        
        
        // i = 0;
        // while (normalsIn>>normals[i])
        // {
        //     i++;
        //     std::cout << normals[i];
        // }
        // i = 0;
        // while (texcoordIn>>texcoord[i])
        // {
        //     i++;
        //     std::cout << texcoord[i];
        // }
        
        positionsIn.close();
        // normalsIn.close();
        // texcoordIn.close();
    }
    int main()
    {
        Window *window = Window::getWindow();
        CameraInstance *camera = CameraInstance::GetCamera();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        Drawable *cube = new Cube();
        Drawable *mmodel = new mModel();
        // Drawable *drawable = (new Drawable())->SetVertexFromData("./data/21/l1/");
        SetVertexFromData("./data/21/l1/", 3);


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