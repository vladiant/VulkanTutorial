## Video
* [Specular Lighting - Vulkan Game Engine Tutorial 26](https://www.youtube.com/watch?v=8CTr0SKQ21U)

## Code
```cpp
  inverseViewMatrix = glm::mat4{1.f};
  inverseViewMatrix[0][0] = u.x;
  inverseViewMatrix[0][1] = u.y;
  inverseViewMatrix[0][2] = u.z;
  inverseViewMatrix[1][0] = v.x;
  inverseViewMatrix[1][1] = v.y;
  inverseViewMatrix[1][2] = v.z;
  inverseViewMatrix[2][0] = w.x;
  inverseViewMatrix[2][1] = w.y;
  inverseViewMatrix[2][2] = w.z;
  inverseViewMatrix[3][0] = position.x;
  inverseViewMatrix[3][1] = position.y;
  inverseViewMatrix[3][2] = position.z;
```

## Links
* [Chapter 11. Shinies](https://paroj.github.io/gltut/Illumination/Tutorial%2011.html)
