/*
 * @Author: ImGili
 * @Description: 
 */

#pragma once
#include "Engine.h"
enum VertexLayout
{
    Vertexlayout,
    VertexNormallayout,
    VertexTexcoordlayout,
    VertexNormalTexcoordlayout,
    D2VertexTexcoordlayout
};

enum DrawLayout
{
    None = 0,
    TextureDrawlayout = BIT(0),
    CamerPositionInside = BIT(2),
    NeedSkyBoxTexture = BIT(3)

};

class Drawable
{
public:
    virtual void Draw()
    {
        UpdateCamera();
        shader->use();
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        if (drawlayout & (DrawLayout::TextureDrawlayout))
        {
            glBindTexture(GL_TEXTURE_2D, TextureID);
        }
        if (drawlayout & (DrawLayout::NeedSkyBoxTexture))
        {
            glBindTexture(GL_TEXTURE_CUBE_MAP, SkyboxTextureID);
            glDepthFunc(GL_LEQUAL);
        }

        glDrawArrays(GL_TRIANGLES, 0, vn);
        glDepthFunc(GL_LESS);
        glUseProgram(0);
    }

    void DrawObject(Object *obj)
    {
        shader->use();
        glm::mat4 model = glm::mat4(1);
        model = glm::translate(model, obj->Position);
        model = glm::scale(model, obj->Scale);
        shader->setMat4("model", model);
        glUseProgram(0);
        Draw();
        shader->use();
        model = glm::mat4(1);
        shader->setMat4("model", model);
        glUseProgram(0);
    }
    virtual void Init() = 0;

    Drawable() {
    }
    Drawable* SetDrawlayout(unsigned int dl)
    {
        drawlayout = dl;
    }
    // 设置着色器
    Drawable *SetShader(const char *vertexPath, const char *fragmentPath)
    {
        shader = new Shader(vertexPath, fragmentPath);
        return this;
    }

    // 设置贴图
    Drawable *SetTextureId(const char *texturePath)
    {
        if (drawlayout & (DrawLayout::TextureDrawlayout))
        {
            if (TextureID != 0)
            {
                glDeleteTextures(1, &TextureID);
            }
            TextureID = loadTexture(texturePath);
        }

        return this;
    }

    Drawable *SetSkyTextureId(vector<std::string> faces) {
        if (drawlayout&(DrawLayout::NeedSkyBoxTexture))
        {
            SkyboxTextureID  = loadCubemap(faces);
        }
        return this;
    }
    unsigned int GetSkyboxTextureID()
    {
        return SkyboxTextureID;
    }
    Drawable *SetSkyTextureId(unsigned int skyboxid) {
        SkyboxTextureID  = skyboxid;
        return this;
    }


    virtual void UpdateCamera()
    {
        CameraInstance *camera = CameraInstance::GetCamera();
        glm::mat4 view = camera->GetViewMatrix();
        glm::mat4 projection = camera->getPerspective();
        shader->use();
        if (drawlayout & (DrawLayout::CamerPositionInside))
        {
            shader->setVec3("cameraPos", camera->GetPosition());
        }

        shader->setMat4("view", view);
        shader->setMat4("projection", projection);
        glUseProgram(0);
    }

    virtual ~Drawable()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteTextures(1, &TextureID);
        delete shader;
    }

protected:
    // 可读可写数据
    // opengl 缓冲id
    unsigned int TextureID, SkyboxTextureID;

    // 组合成员
    Shader *shader;

    // 绘制布局
    unsigned int drawlayout = DrawLayout::None;

    // 自身只读数据
    VertexLayout vt;
    unsigned int vn;
    unsigned int VAO, VBO;

    void InitVertex(void *v, VertexLayout vertexlayout, int vsize)
    {
        vt = vertexlayout;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vsize, v, GL_STATIC_DRAW);
        switch (vt)
        {
        case VertexTexcoordlayout:
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
            glBindVertexArray(0);
            vn = vsize/(sizeof(float)*5);
            break;
        case Vertexlayout:
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
            glBindVertexArray(0);
            vn = vsize/(sizeof(float)*3);
            break;
        case D2VertexTexcoordlayout:
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
            vn = vsize/(sizeof(float)*4);
            glBindVertexArray(0);
            break;
        default:
            break;
        }
    }

    unsigned int loadTexture(char const *path)
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << path << std::endl;
            stbi_image_free(data);
        }

        return textureID;
    }

    unsigned int loadCubemap(vector<std::string> faces)
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width, height, nrChannels;
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                             0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
                stbi_image_free(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return textureID;
    }
};

class Cube : public Drawable
{
public:
    Cube()
    {
        Init();
    }

    void Init() override
    {
        drawlayout = DrawLayout::TextureDrawlayout;
        float cubeVertices[] = {
            // positions          // texture Coords
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
        InitVertex(&cubeVertices, VertexTexcoordlayout, sizeof(cubeVertices));
        SetShader("./EngineShaders/VertexTexcoord/ObjectVertex.vert", "./EngineShaders/VertexTexcoord/ObjectFragment.frag");
        SetTextureId("imgs/woodPicture.jpeg");
        shader->use();
        glm::mat4 model = glm::mat4(1.0f);
        shader->setMat4("model", model);
        shader->setInt("texture1", 0);
        glUseProgram(0);
    }

    ~Cube()
    {
    }
};

class Plane : public Drawable
{
public:
    Plane()
    {
        Init();
    }
    void Init() override
    {
        drawlayout = DrawLayout::TextureDrawlayout;
        float planeVertices[] = {
            // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
            5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
            -5.0f, -0.5f, 5.0f, 0.0f, 0.0f,
            -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,

            5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
            -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
            5.0f, -0.5f, -5.0f, 2.0f, 2.0f};
        InitVertex(&planeVertices, VertexTexcoordlayout, sizeof(planeVertices));
        SetShader("./EngineShaders/VertexTexcoord/ObjectVertex.vert", "./EngineShaders/VertexTexcoord/ObjectFragment.frag");
        SetTextureId("imgs/metal.png");
        shader->use();
        glm::mat4 model = glm::mat4(1.0f);
        shader->setMat4("model", model);
        shader->setInt("texture1", 0);
        glUseProgram(0);
    }
};

class SkyBox : public Drawable
{
public:
    SkyBox()
    {
        Init();
    }
    void Init() override
    {
        drawlayout = DrawLayout::NeedSkyBoxTexture;
        float skyboxVertices[] = {
            // positions
            -1.0f, 1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,

            -1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,

            -1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 1.0f};
        InitVertex(&skyboxVertices, Vertexlayout, sizeof(skyboxVertices));
        vector<std::string> faces{
            "imgs/skybox/right.jpg",
            "imgs/skybox/left.jpg",
            "imgs/skybox/top.jpg",
            "imgs/skybox/bottom.jpg",
            "imgs/skybox/front.jpg",
            "imgs/skybox/back.jpg"};
        SetSkyTextureId(faces);

        SetShader("./20/l2/ObjectVertex.vert", "./20/l2/ObjectFragment.frag");
    }

    void UpdateCamera() override
    {
        CameraInstance *camera = CameraInstance::GetCamera();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(glm::mat3(camera->GetViewMatrix()));
        glm::mat4 projection = camera->getPerspective();
        shader->use();
        shader->setMat4("view", view);
        shader->setMat4("projection", projection);
        glUseProgram(0);
    }

private:
};

class FrameBufferObject : public Drawable
{
public:
    FrameBufferObject()
    {
        Init();
    }

    void Init() override
    {
        Window *window = Window::getWindow();
        // 帧缓冲创建

        glGenFramebuffers(1, &framebufferID);
        glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
        // 帧缓冲纹理创建
        glGenTextures(1, &texColorBufferID);
        glBindTexture(GL_TEXTURE_2D, texColorBufferID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window->SCR_WIDTH, window->SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        // 将它附加到当前绑定的帧缓冲对象
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBufferID, 0);

        // 创建渲染缓冲对象
        glGenRenderbuffers(1, &rboID);
        glBindRenderbuffer(GL_RENDERBUFFER, rboID);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboID);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        float quadVertices[] = {// vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
                                // positions   // texCoords
                                -1.0f, 1.0f, 0.0f, 1.0f,
                                -1.0f, -1.0f, 0.0f, 0.0f,
                                1.0f, -1.0f, 1.0f, 0.0f,

                                -1.0f, 1.0f, 0.0f, 1.0f,
                                1.0f, -1.0f, 1.0f, 0.0f,
                                1.0f, 1.0f, 1.0f, 1.0f};
        InitVertex(&quadVertices, D2VertexTexcoordlayout, sizeof(quadVertices));
        SetShader("./19/l1/ObjectVertex.vert", "./19/l1/ObjectFragment.frag");
    }
    void Draw() override
    {
        Window::Clear();
        shader->use();
        glBindVertexArray(VAO);
        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, texColorBufferID);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
    }

    void SetFrameBuffer()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
    }
    void SetMainFrameBuffer()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0); // 返回默认
    }

private:
    unsigned int framebufferID, rboID, texColorBufferID;
};
class mModel : public Drawable
{
public:
    mModel()
    {
        Init();
        _model = new Model("./models/nanosuit/nanosuit.obj");
    }
    
    ~mModel()
    {
        delete _model;
    }
    void Init() override
    {
        SetShader("./14/l1/ObjectVertex.vert", "./14/l1/ObjectFragment.frag");
        shader->use();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        shader->setMat4("model", model);
        glUseProgram(0);
    }
    Drawable* SetModel(const char* modelPath)
    {
        if (_model!=nullptr)
        {
            delete _model;
        }
        _model = new Model(modelPath);
        return this;
    }

    void Draw() override
    {
        UpdateCamera();
        shader->use();
        if (drawlayout & (DrawLayout::NeedSkyBoxTexture))
        {
            glBindTexture(GL_TEXTURE_CUBE_MAP, SkyboxTextureID);
        }

        _model->Draw(*shader);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

private:
    Model *_model = nullptr;
};
