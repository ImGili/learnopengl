/*
 * @Author: ImGili
 * @Description: 
 */
#pragma once
#include"Engine.h"
enum class DrawableType
{
    CubeWithTexture = 0 ,
    Plane               ,
    SkyBox              ,
    Other               
};


class Object
{
public:
    Object(int sid, int did, glm::vec3 position)
        : SceneId(sid), DrawableID(did), Position(position)
    {}

    // TODO:规划Object属性
    int SecneID, DrawableID;
    glm::vec3 Position;
};
class mScene
{
public:
    ~mScene(){
        /*
         TODO:
            销毁vector中的指针
        */
    }

    Object* CreateObject(DrawableType dt, glm::vec3 position = glm::vec3(0, 0, 0) , Drawable* d = nullptr)
    {
        int sceneid = ss.size(), drawaleid = 0;
        if(dt == DrawableType::Other)
        {
            ds.push_back(d);
            drawaleid = ds.size() -1 ;
            return new Object(sceneid, drawaleid, position);
        }
        return new Object(scenid, (int)dt, position);
    }

    void Draw()
    {
        // TODO: 绘制场景中的Object
    }

    mScene* getInstance()
    {
        if (instance == NULL)
        {
            instance = new mScene();
        }
        return instance;
    }
private:
    std::vector<Drawable*> ds = {new CubeWithTexture(), new Plane(), new SkyBox()};
    std::vector<Object*>   ss;
    static mScene* instance;
    // 禁止构造
    mScene(){}
    // 静止拷贝
    mScene& operator=(const mScene&){}
};


