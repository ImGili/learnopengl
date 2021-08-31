# 错误记录

## lesson 4
### 4l-2
```c++
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float))); // 这里第二个参数是2
```
### 4L_3
```c++
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
```
> 这里的是GL_RGBA！！

### 4e_2
> switch 中不能有变量声明
```c++
switch (select)
        {
        case 1:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            break;
        case 2:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
            break;
        case 3:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture wrapping to GL_REPEAT (default wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            break;
        case 4:
            float borderColor[] = {1.0f, 1.0f, 0.0f, 1.0f}; //这里错了
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
            break;
        default:
            break;
        }
```

### 5l_1
> 最新版本的glm不需要将度数转化为弧度了
```c++
trans = glm::rotate(trans, 90.0f, glm::vec3(0.0, 0.0, 1.0));
```

### assimp编译错误
```zsh
ld: library not found for -lminizip
clang: error: linker command failed with exit code 1 (use -v to see invocation)
make[2]: *** [bshot] Error 1
make[1]: *** [CMakeFiles/bshot.dir/all] Error 2
make: *** [all] Error 2
```

前提：已安装minizip。可以使用``brew install minizip``安装
解决办法：
第一步：找到minizip库文件的位置
这里为``/usr/local/Cellar/minizip/1.2.11/lib``
第二步：添加路径
系统级：修改``/etc/profile``或者``/etc/bashrc``
用户级：修改``~/.bashrc``或者``~/.bash_profile``
将下面的复制粘贴到上述文件即可，注意修改路径
```zsh
#添加到gcc头文件
export C_INCLUDE_PATH=$C_INCLUDE_PATH:/usr/local/Cellar/minizip/1.2.11/lib

#添加到g++头文件路径
export CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:/usr/local/Cellar/minizip/1.2.11/lib

#添加到动态库
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/Cellar/minizip/1.2.11/lib

#添加到静态库
export LIBRARY_PATH=$LIBRARY_PATH:/usr/local/Cellar/minizip/1.2.11/lib
```

### model报错提醒
```c++
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
```
> 这里要把c++标准设置为c++11
> 总header文件，Engine.h文件最好不要用pragam once

### 20课立方体贴图-优化
>把``glDepthFunc(GL_LESS);``写成``glDepthMask(GL_LESS);``