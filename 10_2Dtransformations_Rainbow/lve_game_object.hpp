#pragma once

#include <memory>

#include "lve_model.hpp"

namespace lve {

struct Trnsform2dComponent {
  glm::vec2 translation{};  // position offset
  glm::vec2 scale{1.0f, 1.0f};
  float rotation;

  glm::mat2 mat2() {
    const float s = glm::sin(rotation);
    const float c = glm::cos(rotation);
    glm::mat2 rotMatrix{{c, s}, {-s, c}};

    glm::mat2 scaleMat{{scale.x, 0.0f}, {0.0f, scale.y}};

    return rotMatrix * scaleMat;
  }
};

class LveGameObject {
 public:
  using id_t = unsigned int;

  static LveGameObject createGameObject() {
    static id_t curentId = 0;
    return LveGameObject{curentId++};
  }

  LveGameObject(const LveGameObject&) = delete;
  LveGameObject& operator=(LveGameObject&) = delete;
  LveGameObject(LveGameObject&&) = default;
  LveGameObject& operator=(LveGameObject&&) = default;

  id_t getId() { return id; }

  std::shared_ptr<LveModel> model{};
  glm::vec3 color;
  Trnsform2dComponent transform2d{};

 private:
  LveGameObject(id_t objId) : id{objId} {}
  id_t id;
};

}  // namespace lve
