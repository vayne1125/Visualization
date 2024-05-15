add_executable(Visualization
"main.cpp"
"Streamline.cpp"
"Shader.cpp"
"Volume.cpp"
"constants.cpp"
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