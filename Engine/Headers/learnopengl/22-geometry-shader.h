#include"Engine.h"

namespace glx0221
{
   
    int main()
    {
        Window *window = Window::getWindow();
        CameraInstance *camera = CameraInstance::GetCamera();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glPointSize(200);

        Drawable* drawable =    (new Drawable())\
                                ->SetVertexLayout(Vertexlayout)\
                                ->SetDrawTypes(DrawTypes::POINTS)\
                                ->SetVertexNum(4)\
                                ->SetVertexFromData("./data/22/l1/")\
                                ->SetShader("./22/l1/ObjectVertex.vert", "./22/l1/ObjectFragment.frag", "./22/l1/ObjectGeometry.gs")\
                                ->SetModle(glm::mat4(1));
        // Drawable *drawable = (new Drawable())\
        // ->SetDrawTypes(DrawTypes::POINTS)\
        // ->SetVertexLayout(Vertexlayout)->\
        // SetVertexNum(4)\
        // ->SetVertexFromData("./data/22/l1/")->\
        // SetShader("./21/l1/ObjectVertex.vert", "./21/l1/ObjectFragment.frag")\
        // ->SetModle(glm::mat4(1));

        glPointSize(200);
        
        mGUI::Init();
        // render loop
        // -----------
        while (!window->WindowShouldClose())
        {
            window->UpdateDeltaTime();
            camera->UpdateUniform();
            // input
            // -----
            window->processInput();

            
            window->Clear();

            mGUI::NewFrame();
            {
                ImGui::Begin("myGUI");
                ImGui::End();
            }

            mGUI::RenderGUI();

            
            drawable->Draw();
            

            mGUI::DrawRenderData();
            window->SwapBufferAndPollEvents();
        }

        mGUI::DestroyGUI();
        Window::DestoryWindow();
        CameraInstance::DestoryCamera();
        return 0;
    }

}