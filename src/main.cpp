#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "./header/Shader.hpp"
#include "./header/Camera.hpp"
#include "./header/ModelManager.hpp"

// #include <glm/glm.hpp>
// #include <glm/mat4x4.hpp>
// #include <glm/mat3x3.hpp>
// #include <glm/vec3.hpp>
// #include "./header/constants.hpp"
// #include "./header/Volume.hpp"

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
static pair<int,int> modelFileIndex = {3,3};
const char* modelFileList[] = { "carp", "engine","golfball", "teddybear"};

static pair<int,int> renderModeIndex = {1,1};
const char* renderModeList[] = { "iso-surface method", "slice method"};
int sliceNum = 512;
// int CUR = 240;
METHODS method;
PROJECTION_METHODS projectMethod;

float testa1 = 100.0f,testa2 = 100;
int iso1 = 100, iso2 = 100;


void draw_iso_surface_gui(){
    int btnSz = 130;
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
}
vector<float> alpha,R,G,B;
static int rgba = 0;
void draw_volume_rendering_gui(){
    int btnSz = 130;
    ImGui::NewLine();
    
    {
        ImGui::Text("Slice num");
        if(ImGui::RadioButton("256", &sliceNum, 256)) modelManager->volumeArray[0].cal_slice(sliceNum);
        ImGui::SameLine();
        if(ImGui::RadioButton("512", &sliceNum, 512)) modelManager->volumeArray[0].cal_slice(sliceNum);
        ImGui::SameLine();
        if(ImGui::RadioButton("1024", &sliceNum, 1024)) modelManager->volumeArray[0].cal_slice(sliceNum);
        ImGui::SameLine();
        if(ImGui::RadioButton("2048", &sliceNum, 2048)) modelManager->volumeArray[0].cal_slice(sliceNum);
        ImGui::SameLine();
        if(ImGui::RadioButton("4096", &sliceNum, 4096)) modelManager->volumeArray[0].cal_slice(sliceNum);
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
    // ImGui::End();
    
    int text_len = ImGui::CalcTextSize("Iso-value").x;
    ImGui::SetCursorPosX(100-text_len/2.0);
    ImGui::Text("Iso-value");
    // ImGui::MyPlotHistogram("##iso_value", modelManager->isoValueDistributed.data(), 256, 0,  NULL, FLT_MAX, FLT_MAX, ImVec2(200, 130));
    ImGui::PlotHistogram("##iso_value", modelManager->isoValueDistributed.data(), 256, 0,  NULL, FLT_MAX, FLT_MAX, ImVec2(200, 130));
    ImGui::PlotLines("##a", alpha.data(), 256, 0,  NULL, 0, 255, ImVec2(200, 50));
    ImGui::PlotLines("##R", R.data(), 256, 0,  NULL, 0, 255, ImVec2(200, 50));
    ImGui::PlotLines("##G", G.data(), 256, 0,  NULL, 0, 255, ImVec2(200, 50));
    ImGui::PlotLines("##B", B.data(), 256, 0,  NULL, 0, 255, ImVec2(200, 50));
    
    ImGui::SetNextItemWidth(200);
    ImGui::SliderInt("iso1",&iso1,0,255); 
    
    ImGui::SetNextItemWidth(200);
    ImGui::SliderFloat("a1",&testa1,0,255); 
    
    ImGui::SetNextItemWidth(200);
    ImGui::SliderInt("iso2",&iso2,0,255); 
    
    ImGui::SetNextItemWidth(200);
    ImGui::SliderFloat("a2",&testa2,0,255); 
    
    // {
    //     ImGui::RadioButton("r", &rgba, 0);ImGui::SameLine();
    //     ImGui::RadioButton("g", &rgba, 1);ImGui::SameLine();
    //     ImGui::RadioButton("b", &rgba, 2);ImGui::SameLine();
    //     ImGui::RadioButton("a", &rgba, 3);
    // }
    if(ImGui::Button("go", ImVec2(btnSz, 20))){
        if(rgba == 3) {
            alpha[iso1] = testa1;
            alpha[iso2] = testa2;
        }else if(rgba == 0){
            R[iso1] = testa1;
            R[iso2] = testa2;
        }else if(rgba == 1){
            G[iso1] = testa1;
            G[iso2] = testa2;
        }else if(rgba == 2){
            B[iso1] = testa1;
            B[iso2] = testa2;
        }
        float delta = (testa2 - testa1)/(iso2-iso1);
        for(int i=iso1+1,j=0;i<iso2;i++,j++){
            if(rgba == 3) alpha[i] = testa1 + delta*j;
            if(rgba == 0) R[i] = testa1 + delta*j;
            if(rgba == 1) G[i] = testa1 + delta*j;
            if(rgba == 2) B[i] = testa1 + delta*j;
        }
        modelManager->volumeArray[0].create_1dtexture(alpha,R,G,B);
    }   

    {
        static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
        ImGuiColorEditFlags flags = 0;
        flags |= ImGuiColorEditFlags_AlphaPreviewHalf;
        flags |= ImGuiColorEditFlags_AlphaBar;
        flags |= ImGuiColorEditFlags_PickerHueWheel;
        flags |= ImGuiColorEditFlags_NoInputs;       // Disable all RGB/HSV/Hex displays
        float w = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.y) * 0.40f;
        ImGui::SetNextItemWidth(w);
        ImGui::ColorPicker4("MyColor##4", (float*)&color, flags, NULL);
    }
}
void my_init(){

    for(int i=0;i<256;i++) {
        alpha.push_back(0.1);
        R.push_back(0);
        G.push_back(0);
        B.push_back(0);
    }

    method = METHODS::VOLUME_RENDERING;
    projectMethod = PROJECTION_METHODS::ORTHO;
    
    string v,f;
    #ifdef __linux__
        // v = "/home/yu/Desktop/school/Visualization/src/shaders/IsoSurface.vert";
        // f = "/home/yu/Desktop/school/Visualization/src/shaders/IsoSurface.frag";
        v = "/home/yu/Desktop/school/Visualization/src/shaders/volumeRendering.vert";
        f = "/home/yu/Desktop/school/Visualization/src/shaders/volumeRendering.frag";
    #else
        // v = "D:\\school\\Visualization\\src\\shaders\\IsoSurface.vert";
        // f = "D:\\school\\Visualization\\src\\shaders\\IsoSurface.frag";
        v = "D:\\school\\Visualization\\src\\shaders\\volumeRendering.vert";
        f = "D:\\school\\Visualization\\src\\shaders\\volumeRendering.frag";
    #endif

    shader = new Shader(v,f);

    camera = new Camera(glm::vec3(0,0,-200),glm::vec3(0,0,0),glm::vec3(0,1,0),100);
    camera->set_projection_method(projectMethod);
    
    if(method == METHODS::ISO_SURFACE)
        modelManager = new ModelManager(METHODS::ISO_SURFACE, modelFileList[modelFileIndex.first],200);
    else if(method == METHODS::VOLUME_RENDERING)
        modelManager = new ModelManager(METHODS::VOLUME_RENDERING, modelFileList[modelFileIndex.first]);
    else cout << "ERROR: main.cpp modelManager cant find mrthod.\n";

    // string dir = "D:\\school\\Visualization\\src\\asset\\";    
    // string infFile = dir + modelFileList[modelFileIndex.first] + ".inf";
    // string rawFile = dir + modelFileList[modelFileIndex.first] + ".raw";
    // Volume* mv = new Volume(METHODS::VOLUME_RENDERING, infFile, rawFile);
}
void draw_gui(){
    int btnSz = 130;
    ImGui::SetNextWindowBgAlpha(0.35f);
    ImGuiWindowFlags window_flags = 0;
    // window_flags |= ImGuiWindowFlags_NoMove;
    // ImGui::Begin("00957116 C. Y. Wang", 0, window_flags);
    ImGui::Begin("00957116 C. Y. Wang");
    // render method
    {
        ImGui::Text("Render Mode");
        ImGui::SetNextItemWidth(232);
        if(ImGui::Combo("##loadrendermode", &renderModeIndex.second, renderModeList, IM_ARRAYSIZE(renderModeList)));
        ImGui::SameLine();
        ImGui::SetCursorPosX(250);
        if(ImGui::Button("GoGo",ImVec2(btnSz, 20))){
            if(renderModeIndex.second != renderModeIndex.first){
                renderModeIndex.first = renderModeIndex.second;

                string v,f;
                #ifdef __linux__
                    v = "/home/yu/Desktop/school/Visualization/src/shaders/";
                    f = "/home/yu/Desktop/school/Visualization/src/shaders/";
                #else
                    v = "D:\\school\\Visualization\\src\\shaders\\";
                    f = "D:\\school\\Visualization\\src\\shaders\\";
                #endif

                
                
                if(renderModeIndex.first == METHODS::ISO_SURFACE){
                    method =  METHODS::ISO_SURFACE;
                    v += "IsoSurface.vert";
                    f += "IsoSurface.frag";
                    modelManager->init(METHODS::ISO_SURFACE, modelFileList[modelFileIndex.first], 200);
                }else if(renderModeIndex.first == METHODS::VOLUME_RENDERING){
                    sliceNum = 512;
                    method =  METHODS::VOLUME_RENDERING;
                    v += "volumeRendering.vert";
                    f += "volumeRendering.frag";
                    modelManager->init(METHODS::VOLUME_RENDERING, modelFileList[modelFileIndex.first]);
                }else{
                    cout << "ERROR: main.cpp draw_gui error!\n";
                }
                
                shader = new Shader(v,f);
            }
        }
    }
    ImGui::NewLine();
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
                modelManager->init(method, modelFileList[modelFileIndex.first], 200);
            }
        }
    }

    if(method == METHODS::ISO_SURFACE)
        draw_iso_surface_gui();
    else if(method == METHODS::VOLUME_RENDERING) 
        draw_volume_rendering_gui();
    
    ImGui::End();
    
    if(method == METHODS::ISO_SURFACE){
        //------histogram--------
        ImGui::SetNextWindowBgAlpha(0.35f);
        ImGui::Begin("Histogram");
        ImGui::PlotHistogram("##iso_value", modelManager->isoValueDistributed.data(), 256, 0,  NULL, FLT_MAX, FLT_MAX, ImVec2(200, 130));
        int text_len = ImGui::CalcTextSize("Iso-value").x;
        ImGui::SetCursorPosX(100-text_len/2.0);
        ImGui::Text("Iso-value");
        ImGui::End();
    }

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
    
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    
    my_init();
    // render loop
    // -----------
    while(!glfwWindowShouldClose(window)){

        //---------------------
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        draw_gui();

        // input
        processInput(window);

        // render
        // clear the colorbuffer
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader->use();

        // camera/projection/view transformation
        glm::mat4 projection = camera->get_projection_matrix();
        shader->set_uniform("projection",projection);
        
        glm::mat4 view = camera->get_view_matrix();//glm::lookAt(glm::vec3(0,0,-200),glm::vec3(0,0,0),glm::vec3(0,1,0));
        shader->set_uniform("view", view);
        shader->set_uniform("viewPos", camera->position);
        
        glm::vec3 lightPos = glm::vec3(0,0,-300);
        shader->set_uniform("lightPos",lightPos);

        shader->set_uniform("model",modelManager->get_model_matrix());    

        if(modelManager->autoRY) modelManager->updateFixedRY();
        shader->set_uniform("fixedRY",modelManager->get_fixedRY_matrix());
        
        shader->set_uniform("clipNormal",clipNormal);
        shader->set_uniform("enableCliped",enableCliped);

        if(method == METHODS::ISO_SURFACE){
            int sz = modelManager->volumeArray.size();
            for(int i=0;i<sz;i++){
                float isV = modelManager->volumeArray[i].isoValue;
                shader->set_uniform("objectHSVColor",glm::vec3(255-isV, 1.0f, 0.5f));
                modelManager->volumeArray[i].draw();
            }
        }else if(method == METHODS::VOLUME_RENDERING){
            shader->set_uniform("texture3d", 0);
            shader->set_uniform("texture1d", 1);
            shader->set_uniform("maxMag",modelManager->volumeArray[0].maxMag);
            shader->set_uniform("minMag",modelManager->volumeArray[0].minMag);

            glm::mat3 m3(1.0f);
            glm::vec3 v3(0.0f,0.0f,-100.0f);
            glm::vec3 v4 = m3 * v3;
            
            glm::vec4 xyplane = modelManager->get_model_matrix() * glm::vec4(0.0f,0.0f,-100.0f,0.0f);
            glm::vec4 _xyplane = modelManager->get_model_matrix() * glm::vec4(0.0f,0.0f,100.0f,0.0f);
            glm::vec4 xzplane = modelManager->get_model_matrix() * glm::vec4(0.0f,-100.0f,0.0f,0.0f);
            glm::vec4 _xzplane = modelManager->get_model_matrix() * glm::vec4(0.0f,100.0f,0.0f,0.0f);
            glm::vec4 yzplane = modelManager->get_model_matrix() * glm::vec4(-100.0f,0.0f,0.0f,0.0f);
            glm::vec4 _yzplane = modelManager->get_model_matrix() * glm::vec4(100.0f,0.0f,0.0f,0.0f);

            xyplane = (modelManager->get_fixedRY_matrix() * xyplane) - glm::vec4(0.0f,0.0f,-200.0f,0.0f);
            _xyplane = (modelManager->get_fixedRY_matrix() * _xyplane) - glm::vec4(0.0f,0.0f,-200.0f,0.0f);
            xzplane = (modelManager->get_fixedRY_matrix() * xzplane) - glm::vec4(0.0f,0.0f,-200.0f,0.0f);
            _xzplane = (modelManager->get_fixedRY_matrix() * _xzplane) - glm::vec4(0.0f,0.0f,-200.0f,0.0f);
            yzplane = (modelManager->get_fixedRY_matrix() * yzplane) - glm::vec4(0.0f,0.0f,-200.0f,0.0f);
            _yzplane = (modelManager->get_fixedRY_matrix() * _yzplane) - glm::vec4(0.0f,0.0f,-200.0f,0.0f);

            vector<pair<float,int>> tpv;
            tpv.push_back({glm::l2Norm(glm::vec3(xyplane.x,xyplane.y,xyplane.z)),0});
            tpv.push_back({glm::l2Norm(glm::vec3(_xyplane.x,_xyplane.y,_xyplane.z)),1});
            tpv.push_back({glm::l2Norm(glm::vec3(xzplane.x,xzplane.y,xzplane.z)),4});
            tpv.push_back({glm::l2Norm(glm::vec3(_xzplane.x,_xzplane.y,_xzplane.z)),5});
            tpv.push_back({glm::l2Norm(glm::vec3(yzplane.x,yzplane.y,yzplane.z)),2});
            tpv.push_back({glm::l2Norm(glm::vec3(_yzplane.x,_yzplane.y,_yzplane.z)),3});

            sort(tpv.begin(),tpv.end());

            modelManager->volumeArray[0].draw(tpv[0].second);
        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    ImGui::DestroyContext();
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
    camera -> set_screen_wh(width,height);
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