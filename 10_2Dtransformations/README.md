## Video
* [2D Transformations - Vulkan Game Engine Tutorial 10](https://www.youtube.com/watch?v=gxUcgc88tD4)

## Code
```cpp
void FirstApp::loadGameObjects() {
  std::vector<LveModel::Vertex> vertices{
      {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
      {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
      {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};
  auto lveModel = std::make_shared<LveModel>(lveDevice, vertices);
    
  // https://www.color-hex.com/color-palette/5361
  std::vector<glm::vec3> colors{
      {1.f, .7f, .73f},
      {1.f, .87f, .73f},
      {1.f, 1.f, .73f},
      {.73f, 1.f, .8f},
      {.73, .88f, 1.f}  //
  };
  for (auto& color : colors) {
    color = glm::pow(color, glm::vec3{2.2f});
  }
  for (int i = 0; i < 40; i++) {
    auto triangle = LveGameObject::createGameObject();
    triangle.model = lveModel;
    triangle.transform2d.scale = glm::vec2(.5f) + i * 0.025f;
    triangle.transform2d.rotation = i * glm::pi<float>() * .025f;
    triangle.color = colors[i % colors.size()];
    gameObjects.push_back(std::move(triangle));
  }
}
 
void FirstApp::renderGameObjects(VkCommandBuffer commandBuffer) {
  // update
  int i = 0;
  for (auto& obj : gameObjects) {
    i += 1;
    obj.transform2d.rotation =
        glm::mod<float>(obj.transform2d.rotation + 0.001f * i, 2.f * glm::pi<float>());
  }
 
  // render
  lvePipeline->bind(commandBuffer);
  for (auto& obj : gameObjects) {
    SimplePushConstantData push{};
    push.offset = obj.transform2d.translation;
    push.color = obj.color;
    push.transform = obj.transform2d.mat2();
 
    vkCmdPushConstants(
        commandBuffer,
        pipelineLayout,
        VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
        0,
        sizeof(SimplePushConstantData),
        &push);
    obj.model->bind(commandBuffer);
    obj.model->draw(commandBuffer);
  }
}
```

## Notes
* ECS (Data Oriented)
* Object Oriented (Inheritance, etc.)

## Links
* [Essence of linear algebra](https://www.youtube.com/playlist?list=PLZHQObOWTQDPD3MizzM2xVFitgF8hE_ab)
* [GameplayKit Tutorial: Entity-Component System, Agents, Goals, and Behaviors](https://www.raywenderlich.com/706-gameplaykit-tutorial-entry-component-system-agents-goals-and-behaviors)
* [A SIMPLE ENTITY COMPONENT SYSTEM (ECS) C++](https://austinmorlan.com/posts/entity_component_system/)
* [Geometry](https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/geometry/row-major-vs-column-major-vector)
