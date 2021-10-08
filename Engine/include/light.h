#pragma once
#define GU_POINT_LIGHT 0
#define GU_SPOT_LIGHT 1
#include "Engine.h"
class Light
{
public:
    Light()
        :Position(glm::vec3(0)), lightType(GU_POINT_LIGHT)
    {
        // 创建共享uniform布局块的缓冲对象
        glGenBuffers(1, &uboLight);

        glBindBuffer(GL_UNIFORM_BUFFER, uboLight);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboLight, 0, 2 * sizeof(glm::mat4));
        UpdateUniform();
    }
    void Draw()
    {
        drawable->Draw();
    }

    void UpdateUniform()
    {
        glBindBuffer(GL_UNIFORM_BUFFER, uboLight);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3), glm::value_ptr(Position));
    }


private:
    Drawable* drawable;
    glm::vec3 Position;

    unsigned int lightType, uboLight;
};