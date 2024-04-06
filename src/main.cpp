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
static pair<int,int> modelFileIndex = {0,0};
const char* modelFileList[] = { "carp", "engine"};
// int CUR = 240;
void draw_iso_surface_gui(){
    int btnSz = 130;
    ImGui::SetNextWindowBgAlpha(0.35f);
    ImGuiWindowFlags window_flags = 0;
    // window_flags |= ImGuiWindowFlags_NoMove;
    // ImGui::Begin("00957116 C. Y. Wang", 0, window_flags);
    ImGui::Begin("00957116 C. Y. Wang");
    // Load Model
    {
        ImGui::Text("Load Model");
        ImGui::SetNextItemWidth(232);
        if(ImGui::Combo("##loadfile", &modelFileIndex.second, modelFileList, IM_ARRAYSIZE(modelFileList)));
        ImGui::SameLine();
        ImGui::SetCursorPosX(250);
        if(ImGui::Button("Load",ImVec2(btnSz, 20))){
            if(modelFileIndex.second != modelFileIndex.first){
                modelFileIndex.first = modelFileIndex.second;
                modelManager->init(modelFileList[modelFileIndex.first], 200);
            }
        }
    }
    ImGui::NewLine();
    // Add Iso Surface
    {
        ImGui::Text("Add Iso Surface");
        ImGui::Text("Iso Value");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(162);
        if(ImGui::InputInt("##Iso Value",&addIsoValue)){
            addIsoValue = min(addIsoValue,255);
            addIsoValue = max(addIsoValue,1);
            // CUR--;
            // cout << CUR << " ";
        }
        ImGui::SameLine();
        ImGui::SetCursorPosX(250);
        if(ImGui::Button("Add",ImVec2(btnSz, 20))){
            modelManager->add_volume(addIsoValue);
        }
    }
    // ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_DefaultOpen;
    if (ImGui::TreeNodeEx("iso value list")){
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
        int sz = (min(modelManager->volumnCnt,4) + 0.75) * 20;
        ImGui::BeginChild("ChildL", ImVec2(ImGui::GetContentRegionAvail().x, sz), ImGuiChildFlags_None, window_flags);
        for(int i = 1; i<=255; i++){
            if(modelManager->volumeIsoValueArray[i]){
                int tp = i;
                string stp;
                while(tp){
                    stp += tp%10 + '0';
                    tp/=10;
                }
                reverse(stp.begin(),stp.end());
                {
                    ImGui::PushID(i);
                    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV((255-i)/360.0, 1.0f, 0.5f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV((255-i)/360.0,  1.0f, 0.5f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV((255-i)/360.0,  1.0f, 0.5f));
                    ImGui::Button("", ImVec2(20, 20));
                    ImGui::PopStyleColor(3);
                    ImGui::PopID();
                    ImGui::SameLine();
                }
                
                ImGui::Text(stp.c_str());
                ImGui::SameLine();
                ImGui::SetCursorPosX(220);
                string btns = "delete##" + stp;
                if(ImGui::Button(btns.c_str(), ImVec2(btnSz, 20))){
                    modelManager->delete_volume(i);
                }
            }
        }
        ImGui::EndChild();
        
        ImGui::TreePop();  // This is required at the end of the if block
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
    ImGui::SetCursorPosX(250);
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
    
    if(ImGui::Button("Cross section", ImVec2(btnSz, 20))){
        enableCliped ^= 1;
    }
    if(ImGui::IsItemHovered()){
        if(enableCliped){
            ImGui::SetTooltip("click to disable cross section");
        }else{
            ImGui::SetTooltip("click to enable cross section");
        }
    }
    ImGui::SameLine();
    if(ImGui::Button("Reset", ImVec2(btnSz, 20))){
        clipNormal = glm::vec4(0,1,0,-150);
        modelManager->rotate = glm::vec3(0,0,0);
        enableCliped = 0;
    }
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
    string v,f;
    #ifdef __linux__
        v = "/home/yu/Desktop/school/Visualization/src/shaders/IsoSurface.vert";
        f = "/home/yu/Desktop/school/Visualization/src/shaders/IsoSurface.frag";
    #else
        v = "D:\\school\\Visualization\\src\\shaders\\IsoSurface.vert";
        f = "D:\\school\\Visualization\\src\\shaders\\IsoSurface.frag";
    #endif

    shader = new Shader(v,f);

    camera = new Camera(glm::vec3(0,0,-200),glm::vec3(0,0,0),glm::vec3(0,1,0),100);
    modelManager = new ModelManager("carp",200);
    // modelManager->add_volume(30);
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