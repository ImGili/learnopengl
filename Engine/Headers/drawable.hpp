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
    NeedSkyBoxTexture = BIT(3),
    InstanceDrawlayout = BIT(4)
};



class SpecialShaders
{
public:
    static SpecialShaders *getInstance()
    {
        if (Instance == NULL)
        {
            Instance = new SpecialShaders();
        }

        return Instance;
    }

    Shader *exshader = new Shader("./22/l3/ObjectVertex.vert", "./22/l3/ObjectFragment.frag", "./22/l3/ObjectGeometry.gs");
    Shader *normalshader = new Shader("./22/l4/ObjectVertex.vert", "./22/l4/ObjectFragment.frag", "./22/l4/ObjectGeometry.gs");

private:
    SpecialShaders()
    {
        exshader->use();
        exshader->setMat4("model", glm::mat4(1));
        normalshader->use();
        normalshader->setMat4("model", glm::mat4(1));
        glUseProgram(0);
    }
    static SpecialShaders *Instance;
};

#define GU_NORAML_SHADER SpecialShaders::getInstance()->normalshader
#define GU_EXPLOR_SHADER SpecialShaders::getInstance()->exshader

class Drawable
{
public:
    virtual void Draw(Shader *specialShader = nullptr)
    {
        UpdateCamera();
        if (specialShader != nullptr)
        {
            specialShader->use();
        }
        else
        {
            shader->use();
        }

        glBindVertexArray(VAO);
        if (drawlayout & (DrawLayout::TextureDrawlayout))
        {
            for (int i = 0; i < TextureID.size(); i++)
            {
                glActiveTexture(GL_TEXTURE0 + i);
                glBindTexture(GL_TEXTURE_2D, TextureID[i]);
            }
        }
        if (drawlayout & (DrawLayout::NeedSkyBoxTexture))
        {
            glBindTexture(GL_TEXTURE_CUBE_MAP, SkyboxTextureID);
            glDepthFunc(GL_LEQUAL);
        }

        if (drawlayout&(DrawLayout::InstanceDrawlayout))
        {
            glDrawArraysInstanced(drawTypes, 0, vn, instanceNum);
        }
        else
        {
            glDrawArrays(drawTypes, 0, vn);
        }
            
        
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
    virtual void Init()
    {
    }

    Drawable()
    {
    }

    Drawable *SetDrawInstance(unsigned int n)
    {
        drawlayout |= DrawLayout::InstanceDrawlayout;
        instanceNum = n;
        return this;
    }
    Drawable *SetDrawlayout(unsigned int dl)
    {
        drawlayout = dl;
        return this;
    }
    Drawable *SetDrawTypes(unsigned int dt)
    {
        drawTypes = dt;
        return this;
    }

    Shader* GetShader()
    {
        return shader;
    }
    // 设置着色器
    Drawable *SetShader(const char *vertexPath, const char *fragmentPath, const char *geometryPath = nullptr)
    {
        if (shader != nullptr)
        {
            delete shader;
        }

        shader = new Shader(vertexPath, fragmentPath, geometryPath);
        return this;
    }

    Drawable *SetModle(glm::mat4 model)
    {
        shader->use();
        shader->setMat4("model", model);
        glUseProgram(0);
        return this;
    }

    // 设置贴图
    Drawable *SetTextureId(const char *texturePath, const char *uniformName = nullptr)
    {
        drawlayout |= TextureDrawlayout;
        unsigned int tmp;
        tmp = loadTexture(texturePath);
        TextureID.push_back(tmp);
        shader->use();
        if (uniformName != nullptr)
        {
            shader->setInt(uniformName, TextureID.size() - 1);
        }
        glUseProgram(0);

        return this;
    }

    Drawable *SetSkyTextureId(vector<std::string> faces)
    {
        drawlayout |= NeedSkyBoxTexture;
        SkyboxTextureID = loadCubemap(faces);
        return this;
    }
    unsigned int GetSkyboxTextureID()
    {
        return SkyboxTextureID;
    }
    Drawable *SetSkyTextureId(unsigned int skyboxid)
    {
        SkyboxTextureID = skyboxid;
        return this;
    }

    virtual void UpdateCamera()
    {
        if (glGetUniformBlockIndex(shader->ID, "CameraMatrices") == GL_INVALID_INDEX)
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
    }

    virtual ~Drawable()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        // glDeleteTextures(1, &TextureID);
        for (int i = 0; i < TextureID.size(); i++)
        {
            glDeleteTextures(1, &TextureID[i]);
        }

        delete shader;
    }
    Drawable *SetVertex(void *v, VertexLayout vertexlayout, int vsize)
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
            vn = vsize / (sizeof(float) * 5);
            break;
        case Vertexlayout:
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
            vn = vsize / (sizeof(float) * 3);
            break;
        case D2VertexTexcoordlayout:
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
            vn = vsize / (sizeof(float) * 4);

            break;
        default:
            break;
        }
        glBindVertexArray(0);
        return this;
    }
    Drawable *SetVertexLayout(VertexLayout _vt)
    {
        vt = _vt;
        return this;
    }

    Drawable *SetVertexNum(int _vn)
    {
        vn = _vn;
        return this;
    }
    Drawable *SetVertexFromData(std::string dataPath, unsigned int iv=0)
    {
        float positions[256] = {0};
        float normals[256] = {0};
        float texcoord[256] = {0};
        float instance[32768] = {0};
        std::string positionsPath, normalsPath, texcoordPath, instancePath;
        positionsPath = dataPath + "positions.txt";
        normalsPath = dataPath + "normals.txt";
        texcoordPath = dataPath + "texcoord.txt";
        instancePath = dataPath + "instance.txt";

        std::ifstream positionsIn(positionsPath.c_str());
        std::ifstream normalsIn(normalsPath.c_str());
        std::ifstream texcoordIn(texcoordPath.c_str());
        std::ifstream instanceIn(instancePath.c_str());
        unsigned int i = 0;
        float tmp = 0.0f;
        while (positionsIn.is_open() && positionsIn >> tmp)
        {
            positions[i] = tmp;
            i++;
        }

        i = 0;
        tmp = 0.0f;
        while (normalsIn.is_open() && normalsIn >> tmp)
        {
            normals[i] = tmp;
            i++;
        }

        i = 0;
        tmp = 0.0f;
        while (texcoordIn.is_open() && texcoordIn >> tmp)
        {
            texcoord[i] = tmp;
            i++;
        }
        i = 0;
        tmp = 0.0f;
        while (drawlayout&(DrawLayout::InstanceDrawlayout) && instanceIn.is_open() && instanceIn >> tmp)
        {
            instance[i] = tmp;
            i++;
        }

        positionsIn.close();
        normalsIn.close();
        texcoordIn.close();
        instanceIn.close();

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        unsigned long subOffset = 0;
        switch (vt)
        {
        case Vertexlayout:
            subOffset = vn * sizeof(float) * 3;
            glBufferData(GL_ARRAY_BUFFER, vn * sizeof(float) * 3 + iv * sizeof(float) * instanceNum , nullptr, GL_STATIC_DRAW);
            glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(float) * vn, positions);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
            break;
        case VertexNormallayout:
            subOffset = vn * sizeof(float) * 3 + vn * sizeof(float) * 3;
            glBufferData(GL_ARRAY_BUFFER, vn * sizeof(float) * 3 + vn * sizeof(float) * 3+ iv * sizeof(float) * instanceNum, nullptr, GL_STATIC_DRAW);
            glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(float) * vn, positions);
            glBufferSubData(GL_ARRAY_BUFFER, 3 * sizeof(float) * vn, 3 * sizeof(float) * vn, normals);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(3 * sizeof(float) * vn));
            break;
        case VertexTexcoordlayout:
            subOffset = vn * sizeof(float) * 3 + vn * sizeof(float) * 2;
            glBufferData(GL_ARRAY_BUFFER, vn * sizeof(float) * 3 + vn * sizeof(float) * 2+ iv * sizeof(float) * instanceNum, nullptr, GL_STATIC_DRAW);
            glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(float) * vn, positions);
            glBufferSubData(GL_ARRAY_BUFFER, 3 * sizeof(float) * vn, 2 * sizeof(float) * vn, texcoord);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)(3 * sizeof(float) * vn));
            break;
        case VertexNormalTexcoordlayout:
            subOffset = vn * sizeof(float) * 3 + vn * sizeof(float) * 3 + vn * sizeof(float) * 2;
            glBufferData(GL_ARRAY_BUFFER, vn * sizeof(float) * 3 + vn * sizeof(float) * 3 + vn * sizeof(float) * 2+ iv * sizeof(float) * instanceNum, nullptr, GL_STATIC_DRAW);
            glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(float) * vn, positions);
            glBufferSubData(GL_ARRAY_BUFFER, 3 * sizeof(float) * vn, 3 * sizeof(float) * vn, normals);
            glBufferSubData(GL_ARRAY_BUFFER, 3 * sizeof(float) * vn + 3 * sizeof(float) * vn, 2 * sizeof(float) * vn, texcoord);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(3 * sizeof(float) * vn));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(
                2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)(3 * sizeof(float) * vn + 3 * sizeof(float) * vn));
            break;
        default:
            break;
        }
        if (drawlayout&(DrawLayout::InstanceDrawlayout) && iv)
        {
            glBufferSubData(GL_ARRAY_BUFFER, subOffset, iv * sizeof(float) * instanceNum, instance);
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(
                3, iv, GL_FLOAT, GL_FALSE, iv * sizeof(float), (void *)(subOffset));
            glVertexAttribDivisor(3, 1);
        }
        
        glBindVertexArray(0);
        return this;
    }



protected:
    // 可读可写数据
    // opengl 缓冲id
    unsigned int SkyboxTextureID;
    std::vector<unsigned int> TextureID;

    // 组合成员
    Shader *shader = nullptr;

    // 绘制布局
    unsigned int drawlayout = DrawLayout::None;
    unsigned int drawTypes = GL_TRIANGLES;

    // 自身只读数据
    VertexLayout vt = Vertexlayout;
    unsigned int vn;
    unsigned int instanceNum;
    unsigned int VAO, VBO;

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
        SetVertex(&cubeVertices, VertexTexcoordlayout, sizeof(cubeVertices));
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
        SetVertex(&planeVertices, VertexTexcoordlayout, sizeof(planeVertices));
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
        SetVertex(&skyboxVertices, Vertexlayout, sizeof(skyboxVertices));
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
        SetVertex(&quadVertices, D2VertexTexcoordlayout, sizeof(quadVertices));
        SetShader("./19/l1/ObjectVertex.vert", "./19/l1/ObjectFragment.frag");
    }
    void Draw(Shader *specialShader = nullptr) override
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
    Drawable *SetModel(const char *modelPath)
    {
        if (_model != nullptr)
        {
            delete _model;
        }
        _model = new Model(modelPath);
        return this;
    }

    void Draw(Shader *specialShader = nullptr) override
    {
        UpdateCamera();
        if (specialShader != nullptr)
        {
            specialShader->use();
        }
        else
        {
            shader->use();
        }
        if (drawlayout & (DrawLayout::NeedSkyBoxTexture))
        {
            glBindTexture(GL_TEXTURE_CUBE_MAP, SkyboxTextureID);
        }

        if (specialShader != nullptr)
        {
            _model->Draw(*specialShader);
        }
        else
        {
            _model->Draw(*shader);
        }
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

private:
    Model *_model = nullptr;
};
