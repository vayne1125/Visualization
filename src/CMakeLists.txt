add_executable(TEST
"main.cpp"
"Shader.cpp"
"Volume.cpp"
"MarchingCubes.cpp"
"Camera.cpp"
) #列所有的cpp
target_link_libraries(TEST
# OpenGL::GL
glfw
glm::glm
glad
)