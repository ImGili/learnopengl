# OpenGL学习
克隆项目
```bash
git clone git@github.com:ImGili/learnopengl.git --recursive
```

## 编译项目
### macos及linux平台
```bash
cd build 
cmake ..
make
cd Engine
./Engine
```
### Windows平台
请先下载安装好mingw，并配置好环境变量
```bash
cd build 
cmake .. -G"MinGW Makefile"
mingw32-make
cd Engine
./Engine
```