## Video
* [Camera (View) Transform - Vulkan Game Engine Tutorial 14](https://www.youtube.com/watch?v=rvJHkYnAR3w)

## Code
```cpp
void LveCamera::setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up) {
  const glm::vec3 w{glm::normalize(direction)};
  const glm::vec3 u{glm::normalize(glm::cross(w, up))};
  const glm::vec3 v{glm::cross(w, u)};

  viewMatrix = glm::mat4{1.f};
  viewMatrix[0][0] = u.x;
  viewMatrix[1][0] = u.y;
  viewMatrix[2][0] = u.z;
  viewMatrix[0][1] = v.x;
  viewMatrix[1][1] = v.y;
  viewMatrix[2][1] = v.z;
  viewMatrix[0][2] = w.x;
  viewMatrix[1][2] = w.y;
  viewMatrix[2][2] = w.z;
  viewMatrix[3][0] = -glm::dot(u, position);
  viewMatrix[3][1] = -glm::dot(v, position);
  viewMatrix[3][2] = -glm::dot(w, position);
}

void LveCamera::setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up) {
  setViewDirection(position, target - position, up);
}

void LveCamera::setViewYXZ(glm::vec3 position, glm::vec3 rotation) {
  const float c3 = glm::cos(rotation.z);
  const float s3 = glm::sin(rotation.z);
  const float c2 = glm::cos(rotation.x);
  const float s2 = glm::sin(rotation.x);
  const float c1 = glm::cos(rotation.y);
  const float s1 = glm::sin(rotation.y);
  const glm::vec3 u{(c1 * c3 + s1 * s2 * s3), (c2 * s3), (c1 * s2 * s3 - c3 * s1)};
  const glm::vec3 v{(c3 * s1 * s2 - c1 * s3), (c2 * c3), (c1 * c3 * s2 + s1 * s3)};
  const glm::vec3 w{(c2 * s1), (-s2), (c1 * c2)};
  viewMatrix = glm::mat4{1.f};
  viewMatrix[0][0] = u.x;
  viewMatrix[1][0] = u.y;
  viewMatrix[2][0] = u.z;
  viewMatrix[0][1] = v.x;
  viewMatrix[1][1] = v.y;
  viewMatrix[2][1] = v.z;
  viewMatrix[0][2] = w.x;
  viewMatrix[1][2] = w.y;
  viewMatrix[2][2] = w.z;
  viewMatrix[3][0] = -glm::dot(u, position);
  viewMatrix[3][1] = -glm::dot(v, position);
  viewMatrix[3][2] = -glm::dot(w, position);
}
```

## Links
* [The Math behind (most) 3D games - Perspective Projection](https://www.youtube.com/watch?v=U0_ONQQ5ZNM)
* [OpenGL Projection Matrix](http://www.songho.ca/opengl/gl_projectionmatrix.html)
* [The new Vulkan Coordinate System](https://matthewwellings.com/blog/the-new-vulkan-coordinate-system/)
* [How Pixar's animation has evolved over 24 years, from 'Toy Story' to 'Toy Story 4'](https://www.insider.com/pixars-animation-evolved-toy-story-2019-6)
* [‘Battlefield V’ ray tracing tested: Can a patch save RTX?](https://www.digitaltrends.com/gaming/battlefield-v-dxr-ray-tracing-tested/)
* [Line–plane intersection](https://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection)
