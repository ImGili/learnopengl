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

namespace glx0222
{
   
    int main()
    {
        Window *window = Window::getWindow();
        CameraInstance *camera = CameraInstance::GetCamera();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glPointSize(200);

        Drawable* drawable =    (new Drawable())\
                                ->SetVertexLayout(VertexNormallayout)\
                                ->SetDrawTypes(DrawTypes::POINTS)\
                                ->SetVertexNum(4)\
                                ->SetVertexFromData("./data/22/l2/")\
                                ->SetShader("./22/l2/ObjectVertex.vert", "./22/l2/ObjectFragment.frag", "./22/l2/ObjectGeometry.gs")\
                                ->SetModle(glm::mat4(1));

        // glPointSize(200);
        
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

namespace glx0223
{
    #define PI 3.14
    int main()
    {
        Window *window = Window::getWindow();
        CameraInstance *camera = CameraInstance::GetCamera();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glPointSize(200);
        
        // Drawable* drawable =  (new mModel())->SetShader("./22/l3/ObjectVertex.vert", "./22/l3/ObjectFragment.frag", "./22/l3/ObjectGeometry.gs")->SetModle(glm::mat4(1));
        Drawable* drawable =  (new mModel());
        Shader* ss = GU_EXPLOR_SHADER;
        
        

        float ex = 0.0f;

        // glPointSize(200);
        
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
                ImGui::Begin("爆破");
                ImGui::DragFloat("爆破", &ex, PI/180, -PI, PI);
                ImGui::End();
            }

            mGUI::RenderGUI();
            ss->setFloat("time", ex);
            drawable->Draw(ss);
            
            

            mGUI::DrawRenderData();
            window->SwapBufferAndPollEvents();
        }

        mGUI::DestroyGUI();
        Window::DestoryWindow();
        CameraInstance::DestoryCamera();
        return 0;
    }

}