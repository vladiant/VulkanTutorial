## Video
* [Projection Matrices - Vulkan Game Engine Tutorial 13](https://www.youtube.com/watch?v=YO46x8fALzE)

## Code
```cpp
void LveCamera::setOrthographicProjection(
    float left, float right, float top, float bottom, float near, float far) {
  projectionMatrix = glm::mat4{1.0f};
  projectionMatrix[0][0] = 2.f / (right - left);
  projectionMatrix[1][1] = 2.f / (bottom - top);
  projectionMatrix[2][2] = 1.f / (far - near);
  projectionMatrix[3][0] = -(right + left) / (right - left);
  projectionMatrix[3][1] = -(bottom + top) / (bottom - top);
  projectionMatrix[3][2] = -near / (far - near);
}
 
void LveCamera::setPerspectiveProjection(float fovy, float aspect, float near, float far) {
  assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
  const float tanHalfFovy = tan(fovy / 2.f);
  projectionMatrix = glm::mat4{0.0f};
  projectionMatrix[0][0] = 1.f / (aspect * tanHalfFovy);
  projectionMatrix[1][1] = 1.f / (tanHalfFovy);
  projectionMatrix[2][2] = far / (far - near);
  projectionMatrix[2][3] = 1.f;
  projectionMatrix[3][2] = -(far * near) / (far - near);
}
```

## Links
* [The Math behind (most) 3D games - Perspective Projection](https://www.youtube.com/watch?v=U0_ONQQ5ZNM)
* [OpenGL Projection Matrix](http://www.songho.ca/opengl/gl_projectionmatrix.html)
* [The new Vulkan Coordinate System](https://matthewwellings.com/blog/the-new-vulkan-coordinate-system/)
* [How Pixar's animation has evolved over 24 years, from 'Toy Story' to 'Toy Story 4'](https://www.insider.com/pixars-animation-evolved-toy-story-2019-6)
* [‘Battlefield V’ ray tracing tested: Can a patch save RTX?](https://www.digitaltrends.com/gaming/battlefield-v-dxr-ray-tracing-tested/)
* [Line–plane intersection](https://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection)
