add_executable(Visualization
"main.cpp"
"Shader.cpp"
"Volume.cpp"
"Const.cpp"
"Camera.cpp"
"ModelManager.cpp"
) #列所有的cpp
target_link_libraries(Visualization
# OpenGL::GL
glfw
glm::glm
glad
imgui
)