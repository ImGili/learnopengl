# 习题解答
## lesson4
### 4e-1
```glsl
FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(-TexCoord.x, TexCoord.y)), 0.2);
```
### 4e-2
纹理贴图坐标始终都是从(0,0)到（1，1）的，但是范围拉到（2，2）时，就会根据延伸方式，对超出部分进行拓展，如果时GL_REPEAT的话，就会重复成4张图片。
