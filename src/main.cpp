#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "./header/Shader.hpp"
#include "./header/Camera.hpp"
#include "./header/ModelManager.hpp"

#include <bits/stdc++.h>
#define   PI   3.1415927
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int enableCliped = 0;
glm::vec4 clipNormal = glm::vec4(0,1,0,-150);
int addIsoValue = 128;
Shader *shader;
Camera *camera;
ModelManager *modelManager;

void draw_iso_surface_gui(){
    ImGui::SetNextWindowBgAlpha(0.35f);
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoMove;
    ImGui::Begin("00957116 C. Y. Wang", 0, window_flags);

    ImGui::Text("Add Iso Surface");
    ImGui::Text("Iso Value");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(162);
    if(ImGui::InputInt("##Iso Value",&addIsoValue)){
        addIsoValue = min(addIsoValue,255);
        addIsoValue = max(addIsoValue,1);
    }
    ImGui::SameLine();
    
    int btnSz = 130;
    // ImGui::SetNextItemWidth(text_len);
    if(ImGui::Button("Add",ImVec2(btnSz, 20))){
        modelManager->add_volume(addIsoValue);
    }

    ImGui::NewLine();
    
    ImGui::Text("Rotate:");
    ImGui::Text("X:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::SliderFloat("##x",&modelManager->rotate.x,0,360); // ax + by + cz + d = 0

    ImGui::SameLine();
    ImGui::Text("Y:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::SliderFloat("##y",&modelManager->rotate.y,0,360); // ax + by + cz + d = 0

    ImGui::SameLine();
    ImGui::Text("Z:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::SliderFloat("##z",&modelManager->rotate.z,0,360); // ax + by + cz + d = 0

    ImGui::SameLine();
    if(ImGui::Button("Auto rotate Y", ImVec2(btnSz, 20))){
        modelManager->autoRY ^= 1;
    }

    ImGui::NewLine();
    ImGui::Text("Clipped plane:");
    ImGui::SetNextItemWidth(50);
    ImGui::SliderFloat("x + ",&clipNormal.x,-1.0f,1.0f); // ax + by + cz + d = 0
    ImGui::SameLine();

    ImGui::SetNextItemWidth(50);
    ImGui::SliderFloat("y + ",&clipNormal.y,-1.0f,1.0f);
    ImGui::SameLine();

    ImGui::SetNextItemWidth(50);
    ImGui::SliderFloat("z + ",&clipNormal.z,-1.0f,1.0f);
    ImGui::SameLine();

    ImGui::SetNextItemWidth(100);
    ImGui::SliderFloat(" = 0",&clipNormal.w,-150,150);

    // if(ImGui::RadioButton("clipped section", enableCliped)){
    //     enableCliped ^= 1;
    // }

    if(ImGui::Button("clipped section", ImVec2(btnSz, 20))){
        enableCliped ^= 1;
    }
    if(ImGui::IsItemHovered()){
        if(enableCliped){
            ImGui::SetTooltip("click to disable clipped section");
        }else{
            ImGui::SetTooltip("click to enable clipped section");
        }
    }
    ImGui::SameLine();
    if(ImGui::Button("Reset", ImVec2(btnSz, 20))){
        clipNormal = glm::vec4(0,1,0,-150);
        modelManager->rotate = glm::vec3(0,0,0);
        enableCliped = 0;
    }
    // ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_DefaultOpen;
    // if (ImGui::TreeNodeEx("Histogram", flag))
    // {
    //     ImGui::PlotHistogram("##iso_value", volume->data.data(), 256, 0,  NULL, FLT_MAX, FLT_MAX, ImVec2(200, 130));
    //     int text_iso_value_len = ImGui::CalcTextSize("Iso-value").x;
    //     ImGui::SetCursorPosX(100-text_iso_value_len/2.0);
    //     ImGui::Text("Iso-value");
    //     ImGui::TreePop();  // This is required at the end of the if block
    // }  
    ImGui::End();

    //------histogram--------
    ImGui::SetNextWindowBgAlpha(0.35f);
    ImGui::Begin("Histogram");
    ImGui::PlotHistogram("##iso_value", modelManager->isoValueDistributed.data(), 256, 0,  NULL, FLT_MAX, FLT_MAX, ImVec2(200, 130));
    int text_len = ImGui::CalcTextSize("Iso-value").x;
    ImGui::SetCursorPosX(100-text_len/2.0);
    ImGui::Text("Iso-value");
    ImGui::End();
    ImGui::ShowDemoWindow(); // Show demo window! :)
}

int main(){
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST); 
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    string v = "D:\\school\\Visualization\\src\\shaders\\IsoSurface.vert";
    string f = "D:\\school\\Visualization\\src\\shaders\\IsoSurface.frag";
    shader = new Shader(v,f);

    string inf = "D:\\school\\Visualization\\src\\asset\\Carp.inf";
    string raw = "D:\\school\\Visualization\\src\\asset\\Carp.raw";

    camera = new Camera(glm::vec3(0,0,-200),glm::vec3(0,0,0),glm::vec3(0,1,0),100);
    modelManager = new ModelManager(inf,raw);
    modelManager->add_volume(30);
    //------------------
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
    //------------------


    // render loop
    // -----------
    while(!glfwWindowShouldClose(window)){

        //---------------------
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        draw_iso_surface_gui();

        // input
        processInput(window);

        // render
        // clear the colorbuffer
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader->use();
    
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(camera->zoom),(float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 2000.0f);
        shader->set_uniform("projection",projection);

        shader->set_uniform("model",modelManager->GetModelMatrix());        
        if(modelManager->autoRY) modelManager->updateFixedRY();
        shader->set_uniform("fixedRY",modelManager->GetFixedRYMatrix());

        shader->set_uniform("clipNormal",clipNormal);
        shader->set_uniform("enableCliped",enableCliped);
        
        // camera/view transformation
        glm::mat4 view = camera->GetViewMatrix();//glm::lookAt(glm::vec3(0,0,-200),glm::vec3(0,0,0),glm::vec3(0,1,0));
        shader->set_uniform("view", view);
        shader->set_uniform("viewPos", camera->position);

        glm::vec3 lightPos = glm::vec3(0,0,-300);
        shader->set_uniform("lightPos",lightPos);
        
        int sz = modelManager->volumeArray.size();
        for(int i=0;i<sz;i++){
            float isV = modelManager->volumeArray[i].isoValue;
            shader->set_uniform("objectHSVColor",glm::vec3(255-isV, 1.0f, 0.5f));
            modelManager->volumeArray[i].draw();
        }
        // shader->set_uniform("objectColor",glm::vec4(0.5f, 0.5f, 0.5f,0.8f));
        // modelManager->volumeArray[1].draw();
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteProgram(shader->ID);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}


void processInput(GLFWwindow *window){
    // if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS);   
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}


void mouse_callback(GLFWwindow* window, double xposIn, double yposIn){

}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    camera->ProcessMouseScroll(static_cast<float>(yoffset));
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    // if (button == GLFW_MOUSE_BUTTON_RIGHT){
    //     enableCliped ^= 1;
    // }
}