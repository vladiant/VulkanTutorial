#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <memory>

#include "lve_model.hpp"

namespace lve {

struct TransformComponent {
  glm::vec3 translation{};  // position offset
  glm::vec3 scale{1.0f, 1.0f, 1.0f};
  glm::vec3 rotation{};

  // Matrix corresponds to translate * Ry * Rx * Rz * scale transformation
  // Rotation conversion uses Tait-Bryan angles with axis order Y(1), X(2), Z(3)
  // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
  glm::mat4 mat4();

  glm::mat3 normalMatrix();
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
  TransformComponent transform{};

 private:
  LveGameObject(id_t objId) : id{objId} {}
  id_t id;
};

}  // namespace lve
