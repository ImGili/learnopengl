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

## 实验案例

### 实验2005
折射天空盒子
![折射天空盒子](./docs/imgs/2005.jpg)

### 实验2305
加载100万个小行星模型
![小心星实验](./docs/imgs/2305.jpg)