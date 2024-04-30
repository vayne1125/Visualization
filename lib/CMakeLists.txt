cmake_minimum_required(VERSION 3.22.1)

project(imgui)

# Add the library that is built on this subdirectory.
add_library(imgui 
imconfig.h
imgui.h
imgui_internal.h
imstb_rectpack.h
imstb_textedit.h
imstb_truetype.h
imgui.cpp
imgui_demo.cpp
imgui_draw.cpp
imgui_tables.cpp
imgui_widgets.cpp
backends/imgui_impl_glfw.h
backends/imgui_impl_opengl3.h
backends/imgui_impl_opengl3_loader.h
backends/imgui_impl_glfw.cpp
backends/imgui_impl_opengl3.cpp

implot_internal.h
implot.h
implot_items.cpp
implot.cpp
implot_demo.cpp
)

target_include_directories(imgui PUBLIC include)
target_link_libraries(imgui
glfw
)
