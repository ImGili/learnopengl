#include"Engine.h"

// 未使用实例化
namespace glx0231
{
    int main()
    {
        Window *window = Window::getWindow();
        CameraInstance *camera = CameraInstance::GetCamera();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glPointSize(200);
        
        Drawable* drawable =    (new Drawable())\
                                ->SetVertexNum(6)\
                                ->SetVertexLayout(VertexNormallayout)\
                                ->SetVertexFromData("./data/23/l1/")\
                                ->SetShader("./23/l1/ObjectVertex.vert", "./23/l1/ObjectFragment.frag")\
                                ->SetModle(glm::mat4(1));
        glm::vec3 translations[100];
        int index = 0;
        float offset = 0.1f;
        for(int y = -10; y < 10; y += 2)
        {
            for(int x = -10; x < 10; x += 2)
            {
                glm::vec3 translation;
                translation.x = (float)x / 10.0f + offset;
                translation.y = (float)y / 10.0f + offset;
                translation.z = 0;
                translations[index++] = translation;
            }
        }
        
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
                ImGui::Begin("实例化");
                ImGui::End();
            }

            mGUI::RenderGUI();
            for (int i = 0; i < 100; i++)
            {
                drawable->GetShader()->setVec3("offset", translations[i]);
                drawable->Draw();
            }
            

            mGUI::DrawRenderData();
            window->SwapBufferAndPollEvents();
        }

        mGUI::DestroyGUI();
        Window::DestoryWindow();
        CameraInstance::DestoryCamera();
        return 0;
    }

}
// 使用实例化
namespace glx0232
{
    int main()
    {
        Window *window = Window::getWindow();
        CameraInstance *camera = CameraInstance::GetCamera();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glPointSize(200);
        
        Drawable* drawable =    (new Drawable())\
                                ->SetVertexNum(6)\
                                ->SetDrawInstance(100)\
                                ->SetVertexLayout(VertexNormallayout)\
                                ->SetVertexFromData("./data/23/l1/")\
                                ->SetShader("./23/l2/ObjectVertex.vert", "./23/l2/ObjectFragment.frag")\
                                ->SetModle(glm::mat4(1));
        glm::vec3 translations[100];
        int index = 0;
        float offset = 0.1f;
        for(int y = -10; y < 10; y += 2)
        {
            for(int x = -10; x < 10; x += 2)
            {
                glm::vec3 translation;
                translation.x = (float)x / 10.0f + offset;
                translation.y = (float)y / 10.0f + offset;
                translation.z = 0;
                translations[index++] = translation;
            }
        }
        for(unsigned int i = 0; i < 100; i++)
        {
            stringstream ss;
            string index;
            ss << i; 
            index = ss.str(); 
            drawable->GetShader()->setVec3(("offsets[" + index + "]").c_str(), translations[i]);
        }
        
        
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
                ImGui::Begin("实例化");
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