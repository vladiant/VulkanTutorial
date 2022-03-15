#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "first_app.hpp"

int main() {
  lve::FirstApp app{};

  try {
    app.run();
  } catch (std::exception& e) {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}