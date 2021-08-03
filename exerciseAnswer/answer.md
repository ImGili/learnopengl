# 习题解答
## lesson4
### 4e-1
```glsl
FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(-TexCoord.x, TexCoord.y)), 0.2);
```