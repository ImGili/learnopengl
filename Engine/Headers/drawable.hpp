/*
 * @Author: ImGili
 * @Description: 
 */
#pragma once
#include "Engine.h"
enum VertexLayout
{
    Vertex,
    VertexNormal,
    VertexTexcoord,
    VertexNormalTexcoord
};
class Drawable
{
public:
    virtual void Draw() = 0;

    Drawable() {}

    virtual void Update()
    {
        CameraInstance *camera = CameraInstance::GetCamera();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera->GetViewMatrix();
        glm::mat4 projection = camera->getPerspective();
        shader->use();
        shader->setMat4("model", model);
        shader->setMat4("view", view);
        shader->setMat4("projection", projection);
        glUseProgram(0);
    }

    ~Drawable()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        delete shader;
    }

protected:
    unsigned int VAO, VBO, TextureID;
    Shader *shader;
    void updateVAOAndVBO(void *v, VertexLayout vt, int vsize)
    {
        switch (vt)
        {
        case VertexTexcoord:
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
        case Vertex:
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vsize, v, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
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
        updateVAOAndVBO(&cubeVertices, VertexTexcoord, sizeof(cubeVertices));
        shader = new Shader("./EngineShaders/VertexTexcoord/ObjectVertex.vert", "./EngineShaders/VertexTexcoord/ObjectFragment.frag");
        TextureID = loadTexture("imgs/woodPicture.jpeg");
        shader->use();
        shader->setInt("texture1", 0);
        glUseProgram(0);
    }
    inline void Draw() override
    {
        Update();
        shader->use();
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureID);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glUseProgram(0);
    }
};

class Plane : public Drawable
{
public:
    Plane()
    {
        float planeVertices[] = {
            // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
            5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
            -5.0f, -0.5f, 5.0f, 0.0f, 0.0f,
            -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,

            5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
            -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
            5.0f, -0.5f, -5.0f, 2.0f, 2.0f};
        updateVAOAndVBO(&planeVertices, VertexTexcoord, sizeof(planeVertices));
        shader = new Shader("./EngineShaders/VertexTexcoord/ObjectVertex.vert", "./EngineShaders/VertexTexcoord/ObjectFragment.frag");
        TextureID = loadTexture("imgs/metal.png");
        shader->use();
        shader->setInt("texture1", 0);
        glUseProgram(0);
    }

    inline void Draw() override
    {
        Update();
        shader->use();
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureID);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glUseProgram(0);
    }
};

class SkyBox : public Drawable
{
public:
    SkyBox()
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
        updateVAOAndVBO(&skyboxVertices, Vertex, sizeof(skyboxVertices));
        vector<std::string> faces{
            "imgs/skybox/right.jpg",
            "imgs/skybox/left.jpg",
            "imgs/skybox/top.jpg",
            "imgs/skybox/bottom.jpg",
            "imgs/skybox/front.jpg",
            "imgs/skybox/back.jpg"};
        cubemapTextureID = loadCubemap(faces);
        shader = new Shader("./20/l2/ObjectVertex.vert", "./20/l2/ObjectFragment.frag");
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

    void Draw() override
    {
        Update();
        glDepthFunc(GL_LEQUAL);
        shader->use();
        glBindVertexArray(VAO);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTextureID);
        glActiveTexture(GL_TEXTURE0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
    }

private:
    unsigned int cubemapTextureID;
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