#include "Engine.h"
#include "learnopengl/1_hello_window.h"
#include "learnopengl/2_hello_triangle.h"
#include "learnopengl/3_shader.h"
#include "learnopengl/4_texture.h"
#include "learnopengl/5_transformation.h"
#include "learnopengl/6_coordinatte_system.h"
#include "learnopengl/7_camera.h"
#include "learnopengl/8_color.h"
#include "learnopengl/9_basic_light.h"
#include "learnopengl/10_material.h"
#include "learnopengl/11_light_map.h"
#include "learnopengl/12_light_caster.h"
#include "learnopengl/13_mutiple_light.h"
/*
    imgui实验
*/
// demo窗口

namespace glx121
{
    void framebuffer_size_callback(GLFWwindow *window, int width, int height);
    void processInput(GLFWwindow *window);

    // settings
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;

    int main()
    {
        // glfw: initialize and configure
        // ------------------------------
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        // glfw window creation
        // --------------------
        GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return -1;
        }

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
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        // Our state
        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        float vertices[] = {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};

        unsigned int VBO, VAO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);
        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        stbi_set_flip_vertically_on_load(true);
        unsigned int texture1;
        glGenTextures(1, &texture1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        // 为当前绑定的纹理对象设置环绕、过滤方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // 加载纹理图片
        int width, height, nrChannels;
        unsigned char *data = stbi_load("./imgs/woodPicture.jpeg", &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture1" << std::endl;
        }
        stbi_image_free(data);

        unsigned int texture2;
        glGenTextures(1, &texture2);
        glBindTexture(GL_TEXTURE_2D, texture2);
        // 为当前绑定的纹理对象设置环绕、过滤方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        data = stbi_load("./imgs/awesomeface.png", &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture1" << std::endl;
        }
        stbi_image_free(data);

        Shader myshader("./6/l2/vertex.vert", "./6/l2/fragment.frag");

        myshader.use();
        myshader.setInt("texture1", 0);
        myshader.setInt("texture2", 1);
        glm::mat4 view = glm::mat4(1), model = glm::mat4(1), projection = glm::mat4(1);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);

        glEnable(GL_DEPTH_TEST);

        glm::vec3 cubePositions[] = {
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(2.0f, 5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3(2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f, 3.0f, -7.5f),
            glm::vec3(1.3f, -2.0f, -2.5f),
            glm::vec3(1.5f, 2.0f, -2.5f),
            glm::vec3(1.5f, 0.2f, -1.5f),
            glm::vec3(-1.3f, 1.0f, -1.5f)};

        // render loop
        // -----------
        while (!glfwWindowShouldClose(window))
        {
            // input
            // -----
            processInput(window);

            // imgui
            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
            if (show_demo_window)
                ImGui::ShowDemoWindow(&show_demo_window);

            // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
            {
                static float f = 0.0f;
                static int counter = 0;

                ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.
                ImGui::Text("你好");
                ImGui::Text("This is some useful text.");          // Display some text (you can use a format strings too)
                ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
                ImGui::Checkbox("Another Window", &show_another_window);

                ImGui::SliderFloat("float", &f, 0.0f, 1.0f);             // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::ColorEdit3("clear color", (float *)&clear_color); // Edit 3 floats representing a color

                if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
                    counter++;
                ImGui::SameLine();
                ImGui::Text("counter = %d", counter);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }

            // 3. Show another simple window.
            if (show_another_window)
            {
                ImGui::Begin("Another Window", &show_another_window); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
                ImGui::Text("Hello from another window!");
                if (ImGui::Button("Close Me"))
                    show_another_window = false;
                ImGui::End();
            }

            // Rendering
            ImGui::Render();

            // render
            // ------
            glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture2);
            glBindVertexArray(VAO);
            myshader.use();
            myshader.setMat4("view", view);
            myshader.setMat4("projection", projection);
            for (int i = 0; i < 10; i++)
            {
                model = glm::mat4(1);
                model = glm::translate(model, cubePositions[i]);
                model = glm::rotate(model, (float)glfwGetTime() * 50.0f, glm::vec3(1.0f, i, 0.0f));
                myshader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);

        // glfw: terminate, clearing all previously allocated GLFW resources.
        // ------------------------------------------------------------------
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwTerminate();
        return 0;
    }

    // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
    // ---------------------------------------------------------------------------------------------------------
    void processInput(GLFWwindow *window)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }

    // glfw: whenever the window size changed (by OS or user resize) this callback function executes
    // ---------------------------------------------------------------------------------------------
    void framebuffer_size_callback(GLFWwindow *window, int width, int height)
    {
        // make sure the viewport matches the new window dimensions; note that width and
        // height will be significantly larger than specified on retina displays.
        glViewport(0, 0, width, height);
    }
}

int main()
{
    int no = 1;
    while (no)
    {
        __CLEAR_TERMINAL;
        std::cout << "1、你好，窗口；\n"
                     "21、三角形实验1（你好、三角形）；\n"
                     "22、三角形实验2（用顶点组索引绘制正方形）；\n"
                     "201、三角形习题1（绘制两个三角形）；\n"
                     "202、三角形习题1（用不同的VAO和VBO，绘制两个三角形）；\n"
                     "31、着色器实验1（简单着色器程序）；\n"
                     "32、着色器实验2（绿色随时间变化）；\n"
                     "301、着色器习题1（三角形颠倒）；\n"
                     "302、着色器习题2（设置Uniform变量将三角形移动距离）；\n"
                     "303、着色器习题3（将顶点位置设置为颜色输出）；\n"
                     "41、纹理实验1（木箱纹理实验）；\n"
                     "42、纹理实验2（木箱纹理混合颜色）；\n"
                     "43、纹理实验3（两种纹理混合）；\n"
                     "401、纹理习题1（只修改片段着色器，使笑脸颠倒）；\n"
                     "402、纹理习题2（尝试修改纹理环绕方式）；\n"
                     "403、纹理习题3（只显示一个像素）；\n"
                     "404、纹理习题4（键盘控制mix参数）；\n"
                     "51、变换实验1（正方形先缩小再旋转）；\n"
                     "52、变换实验2（正方形不断旋转）；\n"
                     "501、变换习题1（正方形先旋转再位移）；\n"
                     "502、变换习题2（两个正方形不停旋转）；\n"
                     "61、坐标系统实验1（初步进入3D世界）；\n"
                     "62、坐标系统实验1（更多3D）；\n"
                     "63、坐标系统实验1（更多3D箱子）；\n"
                     "601、坐标系统习题1（对projection函数各个参数进行实验）；\n"
                     "602、坐标系统习题2（观察矩阵在各个方向上进行位移）；\n"
                     "603、坐标系统习题3（让3对倍数对箱子运动（包括第一个），其他箱子静止）；\n"
                     "71、摄像机实验1（look at矩阵实验）；\n"
                     "72、摄像机实验2（鼠标输入）；\n"
                     "73、摄像机实验3（自由移动）；\n"
                     "74、摄像机实验4（使用摄像机类）；\n"
                     "701、摄像机习题1（创建FPS相机）；\n"
                     "81、颜色实验1；\n"
                     "91、光照实验1；\n"
                     "901、光照习题1（光源移动）；\n"
                     "902、光照习题2（观察坐标系光照）；\n"
                     "903、光照习题3（Gouraud）；\n"
                     "101、材质实验1；\n"
                     "1001、材质习题1（青色塑料）；\n"
                     "111、光照贴图实验1；\n"
                     "112、光照贴图实验2（镜面贴图）；\n"
                     "1101、光照贴图习题1（反转镜面贴图）；\n"
                     "1102、光照贴图习题2（彩色镜面贴图）；\n"
                     "1103、光照贴图习题3（自发光贴图）；\n"
                     "121、投光物实验1（平行光）；\n"
                     "122、投光物实验2（衰减平行光）；\n"
                     "123、投光物实验3（聚光）；\n"
                     "124、投光物实验4（聚光平滑/软化边缘）；\n"
                     "131、多光源实验1；\n";
        std::cout << "请输入编号（输入0退出程序）：";
        std::cin >> no;
        switch (no)
        {
        case 1:
            glx001::main();
            break;
        case 21:
            glx021::main();
            break;
        case 22:
            glx022::main();
            break;
        case 201:
            glxe021::main();
            break;
        case 202:
            glxe022::main();
            break;
        case 31:
            glx031::main();
            break;
        case 32:
            glx032::main();
            break;
        case 301:
            glxe031::main();
            break;
        case 302:
            glxe032::main();
            break;
        case 303:
            glxe033::main();
            break;
        case 41:
            glx041::main();
            break;
        case 42:
            glx042::main();
            break;
        case 43:
            glx043::main();
            break;
        case 401:
            glxe041::main();
            break;
        case 402:
            glxe042::main();
            break;
        case 403:
            glxe043::main();
            break;
        case 404:
            glxe044::main();
            break;
        case 51:
            glx051::main();
            break;
        case 52:
            glx052::main();
            break;
        case 501:
            glxe051::main();
            break;
        case 502:
            glxe052::main();
            break;
        case 61:
            glx061::main();
            break;
        case 62:
            glx062::main();
            break;
        case 63:
            glx063::main();
            break;
        case 601:
            glxe061::main();
            break;
        case 602:
            glxe062::main();
        case 603:
            glxe063::main();
            break;
        case 71:
            glx071::main();
            break;
        case 72:
            glx072::main();
            break;
        case 73:
            glx073::main();
            break;
        case 74:
            glx074::main();
            break;
        case 701:
            glxe071::main();
            break;
        case 81:
            glx081::main();
            break;
        case 91:
            glx091::main();
            break;
        case 901:
            glxe091::main();
            break;
        case 902:
            glxe092::main();
            break;
        case 903:
            glxe093::main();
            break;
        case 101:
            glx0101::main();
            break;
        case 1001:
            glxe0101::main();
            break;
        case 111:
            glx0111::main();
            break;
        case 112:
            glx0112::main();
            break;
        case 1101:
            glxe0111::main();
            break;
        case 1102:
            glxe0112::main();
            break;
        case 1103:
            glxe0113::main();
            break;
        case 121:
            glx0121::main();
            break;
        case 122:
            glx0122::main();
            break;
        case 123:
            glx0123::main();
            break;
        case 124:
            glx0124::main();
            break;
        case 131:
            glx0131::main();
            break;
        default:
            break;
        }
    }
    __CLEAR_TERMINAL;
}