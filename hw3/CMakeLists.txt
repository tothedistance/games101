add_executable(
  hw3
  main.cpp
  rasterizer.hpp
  rasterizer.cpp
  global.hpp
  Triangle.hpp
  Triangle.cpp
  Texture.hpp
  Texture.cpp
  Shader.hpp
  OBJ_Loader.h)
target_link_libraries(hw3 PRIVATE games_diagnostics games_utils)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/models DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
