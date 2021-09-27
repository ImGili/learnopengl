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
    SkyboxDrawlayout = BIT(1),
    CamerPositionInside = BIT(2),
    NeedSkyBoxTexture = BIT(3)

};

class Drawable
{
public:
    virtual void Draw() 
    {
        Update();
        shader->use();
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        if(drawlayout&(DrawLayout::TextureDrawlayout))
        {
            glBindTexture(GL_TEXTURE_2D, TextureID);
        }
        if (drawlayout&(DrawLayout::SkyboxDrawlayout))
        {
            glBindTexture(GL_TEXTURE_CUBE_MAP, TextureID);
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
        shader->setMat4("model", model);
        glUseProgram(0);
        Draw();
        shader->use();
        model = glm::mat4(1);
        shader->setMat4("model", model);
        glUseProgram(0);
    }
    virtual void Init(const char *vertexPath, const char *fragmentPath) = 0;

    Drawable() {}

    virtual void Update()
    {
        CameraInstance *camera = CameraInstance::GetCamera();
        glm::mat4 view = camera->GetViewMatrix();
        glm::mat4 projection = camera->getPerspective();
        shader->use();
        if (drawlayout&(DrawLayout::CamerPositionInside))
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
        std::cout << "~Drawable" << std::endl;
    }

protected:
    unsigned int VAO, VBO, TextureID, vn, SkyboxTextureID;
    unsigned int drawlayout;
    Shader *shader;
    void updateVAOAndVBO(void *v, VertexLayout vt, int vsize)
    {
        switch (vt)
        {
        case VertexTexcoordlayout:
            // cube VAO
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vsize, v, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
            glBindVertexArray(0);
            break;
        case Vertexlayout:
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vsize, v, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
            glBindVertexArray(0);
            break;
        case D2VertexTexcoordlayout:
            // cube VAO
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vsize, v, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
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
};

class CubeWithTexture : public Drawable
{
public:
    CubeWithTexture()
    {
        Init("./EngineShaders/VertexTexcoord/ObjectVertex.vert", "./EngineShaders/VertexTexcoord/ObjectFragment.frag");
    }

    CubeWithTexture(const char *vertexPath, const char *fragmentPath)
    {
        Init(vertexPath, fragmentPath);
    }

    void Init(const char *vertexPath, const char *fragmentPath) override
    {
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
        updateVAOAndVBO(&cubeVertices, VertexTexcoordlayout, sizeof(cubeVertices));
        shader = new Shader(vertexPath, fragmentPath);
        TextureID = loadTexture("imgs/woodPicture.jpeg");
        shader->use();
        glm::mat4 model = glm::mat4(1.0f);
        shader->setMat4("model", model);
        shader->setInt("texture1", 0);
        glUseProgram(0);
        drawlayout = DrawLayout::TextureDrawlayout;
        vn = 36;
    }
    
    ~CubeWithTexture()
    {
        std::cout << "~CubeWithTexture" << std::endl;
    }
};

class Plane : public Drawable
{
public:
    Plane()
    {
        Init("./EngineShaders/VertexTexcoord/ObjectVertex.vert", "./EngineShaders/VertexTexcoord/ObjectFragment.frag");
    }
    Plane(const char *vertexPath, const char *fragmentPath)
    {
        Init(vertexPath, fragmentPath);
    }
    void Init(const char *vertexPath, const char *fragmentPath) override
    {
        float planeVertices[] = {
            // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
            5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
            -5.0f, -0.5f, 5.0f, 0.0f, 0.0f,
            -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,

            5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
            -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
            5.0f, -0.5f, -5.0f, 2.0f, 2.0f};
        updateVAOAndVBO(&planeVertices, VertexTexcoordlayout, sizeof(planeVertices));
        shader = new Shader(vertexPath, fragmentPath);
        TextureID = loadTexture("imgs/metal.png");
        shader->use();
        glm::mat4 model = glm::mat4(1.0f);
        shader->setMat4("model", model);
        shader->setInt("texture1", 0);
        glUseProgram(0);
        vn = 6;
        drawlayout = DrawLayout::TextureDrawlayout;
    }

    
};

class SkyBox : public Drawable
{
public:
    SkyBox()
    {
        Init("./20/l2/ObjectVertex.vert", "./20/l2/ObjectFragment.frag");
    }
    SkyBox(const char *vertexPath, const char *fragmentPath)
    {
        Init(vertexPath, fragmentPath);
    }

    void Init(const char *vertexPath, const char *fragmentPath) override
    {
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
        updateVAOAndVBO(&skyboxVertices, Vertexlayout, sizeof(skyboxVertices));
        vector<std::string> faces{
            "imgs/skybox/right.jpg",
            "imgs/skybox/left.jpg",
            "imgs/skybox/top.jpg",
            "imgs/skybox/bottom.jpg",
            "imgs/skybox/front.jpg",
            "imgs/skybox/back.jpg"};
        TextureID = loadCubemap(faces);
        shader = new Shader(vertexPath, fragmentPath);
        drawlayout = DrawLayout::SkyboxDrawlayout;
        vn = 36;
    }

    void Update() override
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

    unsigned int GetSkyBoxTextureID()
    {
        return TextureID;
    }


private:
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

class FrameBufferObject : public Drawable
{
public:
    FrameBufferObject()
    {
        Init("./19/l1/ObjectVertex.vert", "./19/l1/ObjectFragment.frag");
    }
    FrameBufferObject(const char *vertexPath, const char *fragmentPath)
    {
        Init(vertexPath, fragmentPath);
    }

    void Init(const char *vertexPath, const char *fragmentPath) override
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
        updateVAOAndVBO(&quadVertices, D2VertexTexcoordlayout, sizeof(quadVertices));
        shader = new Shader(vertexPath, fragmentPath);
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
        Init("./14/l1/ObjectVertex.vert", "./14/l1/ObjectFragment.frag");
        _model = new Model("./models/nanosuit/nanosuit.obj");
    }
    mModel(const char *vertexPath, const char *fragmentPath, const char *modelPath = "./models/nanosuit/nanosuit.obj", unsigned int _drawlayout=DrawLayout::None, SkyBox* skybox=nullptr)
    {
        Init(vertexPath, fragmentPath);
        drawlayout = _drawlayout;
        _model = new Model(modelPath);
        if (drawlayout&(DrawLayout::NeedSkyBoxTexture)&& skybox!=nullptr)
        {
            SkyboxTextureID = skybox->GetSkyBoxTextureID();
        }
    }
    ~mModel()
    {
        delete _model;
    }
    void Init(const char *vertexPath, const char *fragmentPath) override
    {
        shader = new Shader(vertexPath, fragmentPath);
        shader->use();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        shader->setMat4("model", model);
        glUseProgram(0);
    }

    void Draw() override
    {
        Update();
        shader->use();
        if (drawlayout&(DrawLayout::NeedSkyBoxTexture))
        {
            glBindTexture(GL_TEXTURE_CUBE_MAP, SkyboxTextureID);
        }
        
        _model->Draw(*shader);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

private:
    Model *_model;
};
