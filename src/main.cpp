#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <implot.h>

#include <bits/stdc++.h>

#include "./header/Shader.hpp"
#include "./header/Camera.hpp"
#include "./header/ModelManager.hpp"
#include "./header/Streamline.hpp"


#define   PI   3.1415927
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int enableCliped = 0;
glm::vec4 clipNormal = glm::vec4(0,1,0,-150);
int addIsoValue = 128;
Shader *shader;
Camera *camera;
ModelManager *modelManager;
Streamline *streamline;
static pair<int,int> modelFileIndex = {1,1};
const char* modelFileList[] = { "carp", "engine","golfball", "teddybear"};

static pair<int,int> vecFileIndex = {0,0};
const char* vecFileList[] = { "1.vec", "2.vec","3.vec", "4.vec", "5.vec", "6.vec", "7.vec", "8.vec", "9.vec", "10.vec", "11.vec", "12.vec", "13.vec", "14.vec", "15.vec", "16.vec", "17.vec", "18.vec", "19.vec", "20.vec", "21.vec", "22.vec", "23.vec", "rect1.vec", "rect2.vec", "step5_velocity.vec", "test_not_unit.vec", "test_unit.vec"};

// todo
// vec 陣列
static pair<int,int> renderModeIndex = {0,0};
const char* renderModeList[] = { "iso-surface method", "slice method","ray casting method","streamline(RK2 method)"};
METHODS method;
PROJECTION_METHODS projectMethod;

// gui
vector<float> xs, bar;
float val1 = 0.0f,val2 = 0.0f;
int iso1 = 0, iso2 = 0;
int sliceNum = 512;
float rayCastingGap = 0.3;
vector<vector<float>> RGBA;
static int rgba = 0;
static float streamline_h, streamline_density, streamline_gap;
static int streamline_pointsthreshold1, streamline_pointsthreshold2;

void draw_iso_surface_gui(){
    int btnSz = 130;
    ImGui::Spacing();
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
    ImGui::Spacing();
    
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

    ImGui::Spacing();
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
void reset_RGBA(){
    cout << "reset RGBA\n";
    for(int i=0;i<256;i++) {
        // HSV to RGB
        float h =  256-i;
        float s =  1.0;
        float l =  0.5;

        float q = l + s - (l*s);
        float p = 2 * l - q;
        float hk = h/360.0;
        // float tpR = hk + 1/3.0, tpG = hk, tpB = hk-1/3.0;
        glm::vec3 tpRGB = glm::vec3(hk + 1/3.0, hk, hk-1/3.0);
        glm::vec3 RGB;

        for(int i=0;i<3;i++){
            if(tpRGB[i] < 0) tpRGB[i] += 1.0;
            else if(tpRGB[i] > 1) tpRGB[i] -= 1.0; 
        }
        for(int i=0;i<3;i++){
            if(tpRGB[i] < 1/6.0) RGB[i] = p+(q-p)*6*tpRGB[i];
            else if(1/6.0 <= tpRGB[i] && tpRGB[i] < 1/2.0) RGB[i] = q;
            else if(1/2.0 <= tpRGB[i] && tpRGB[i] < 2/3.0) RGB[i] = p + (q-p)*6*(2/3.0 - tpRGB[i]);
            else RGB[i] = p;
        }
        RGBA[0][i] = RGB.r;
        RGBA[1][i] = RGB.g;
        RGBA[2][i] = RGB.b;
        RGBA[3][i] = 0.05;
    }
}
void draw_volume_rendering_gui(){
    int btnSz = 130;
    ImGui::Spacing();
    {
        ImGui::Text("Use Phong Shadding");ImGui::SameLine();
        ImGui::RadioButton("Yes", &modelManager->openPhong, 1);ImGui::SameLine();
        ImGui::RadioButton("No", &modelManager->openPhong, 0);
    }
    if(method == METHODS::SLICE_METHOD){
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
        
        ImGui::Spacing();
        ImGui::Text("Rotate:");
        ImGui::Text("X:");ImGui::SameLine();
        ImGui::SetNextItemWidth(50);
        ImGui::SliderFloat("##x",&modelManager->rotate.x,0,360); // ax + by + cz + d = 0
        ImGui::SameLine();
        ImGui::Text("Y:");ImGui::SameLine();
        ImGui::SetNextItemWidth(50);
        ImGui::SliderFloat("##y",&modelManager->rotate.y,0,360); // ax + by + cz + d = 0

        ImGui::SameLine();
        ImGui::Text("Z:");ImGui::SameLine();
        ImGui::SetNextItemWidth(50);
        ImGui::SliderFloat("##z",&modelManager->rotate.z,0,360); // ax + by + cz + d = 0

        ImGui::SameLine();
        ImGui::SetCursorPosX(250);
        if(ImGui::Button("Auto rotate Y", ImVec2(btnSz, 20))){
            modelManager->autoRY ^= 1;
        }
    } //END IF 
    else if(method == METHODS::RAY_CASTING){
        ImGui::Text("Gap"); ImGui::SameLine();
        ImGui::SetNextItemWidth(btnSz);
        ImGui::SliderFloat("##gap",&rayCastingGap,0.5f,0.01f);
    }
    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::SeparatorText("Color Editor");
    {
        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
        if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags)){
            if (ImGui::BeginTabItem("editor1"))
            {
                ImGui::RadioButton("r", &rgba, 0);ImGui::SameLine();
                ImGui::RadioButton("g", &rgba, 1);ImGui::SameLine();
                ImGui::RadioButton("b", &rgba, 2);ImGui::SameLine();
                ImGui::RadioButton("a", &rgba, 3);

                ImGui::Text("iso1");ImGui::SameLine();
                ImGui::SetNextItemWidth(146); ImGui::SliderInt("##iso1",&iso1,0,255); ImGui::SameLine();
                ImGui::Text("val1");ImGui::SameLine(); 
                ImGui::SetNextItemWidth(146);ImGui::SliderFloat("##a1",&val1,0,1); 
            
                ImGui::Text("iso2");ImGui::SameLine();
                ImGui::SetNextItemWidth(146); ImGui::SliderInt("##iso2",&iso2,0,255); ImGui::SameLine();
                ImGui::Text("val2");ImGui::SameLine(); 
                ImGui::SetNextItemWidth(146);ImGui::SliderFloat("##a2",&val2,0,1);

                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                if(ImGui::Button("reset", ImVec2(btnSz/1.5, 18))){
                    reset_RGBA();
                    modelManager->volumeArray[0].create_1dtexture(RGBA); 
                }
                ImGui::SameLine();
                if(ImGui::Button("reset to 0", ImVec2(btnSz/1.5, 18))){
                    for(int i=0;i<256;i++){
                        RGBA[0][i] = 0;
                        RGBA[1][i] = 0;
                        RGBA[2][i] = 0;
                        RGBA[3][i] = 0;
                    }
                    modelManager->volumeArray[0].create_1dtexture(RGBA); 
                }
                ImGui::SameLine();
                ImGui::SetCursorPosX(250);
                if(ImGui::Button("go", ImVec2(btnSz, 18))){
                    RGBA[rgba][iso1] = val1;
                    RGBA[rgba][iso2] = val2;
                    float delta = (val2 - val1)/(iso2-iso1);
                    for(int i=iso1+1,j=0;i<iso2;i++,j++){
                        RGBA[rgba][i] = val1 + delta*j;
                    }
                    modelManager->volumeArray[0].create_1dtexture(RGBA); 
                }
                ImGui::EndTabItem();
                
            }
            if (ImGui::BeginTabItem("editor2"))
            {
                static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
                ImGuiColorEditFlags flags = 0;
                flags |= ImGuiColorEditFlags_AlphaPreviewHalf;
                flags |= ImGuiColorEditFlags_AlphaBar;
                flags |= ImGuiColorEditFlags_PickerHueWheel;
                flags |= ImGuiColorEditFlags_NoInputs;       // Disable all RGB/HSV/Hex displays
                // float w = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.y) * 0.40f;
                ImGui::SetNextItemWidth(165);
                ImGui::ColorPicker4("MyColor##4", (float*)&color, flags, NULL);

                ImGui::SameLine(250);
                ImGui::SetNextItemWidth(50);
                ImGui::BeginGroup();
                ImGui::Text("iso1");
                ImGui::SetNextItemWidth(btnSz);ImGui::SliderInt("##iso3",&iso1,0,255);
                ImGui::Text("iso2");
                ImGui::SetNextItemWidth(btnSz);ImGui::SliderInt("##iso4",&iso2,0,255);
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                if(ImGui::Button("go", ImVec2(btnSz, 18))){
                    for(int i=iso1+1,j=0;i<iso2;i++,j++){ // for each iso
                        RGBA[0][i] = color.x;
                        RGBA[1][i] = color.y;
                        RGBA[2][i] = color.z;
                        RGBA[3][i] = color.w;
                    }
                    modelManager->volumeArray[0].create_1dtexture(RGBA); 
                } 
                ImGui::EndGroup();
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
    }

    if(method == METHODS::SLICE_METHOD)ImGui::SetCursorPosY(400);
    else if(method == METHODS::RAY_CASTING) ImGui::SetCursorPosY(350);
    {
        if (ImPlot::BeginPlot("Palette")) {
            ImPlot::SetupLegend(ImPlotLocation_NorthWest,ImPlotLegendFlags_Horizontal);
            ImPlot::SetupAxis(ImAxis_X1, "intensity");
            ImPlot::SetupAxisLimits(ImAxis_X1, 0, 256);
            ImPlot::SetupAxis(ImAxis_Y1, "count");
            ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 25);
            ImPlot::SetupAxis(ImAxis_Y2, "RGBA value", ImPlotAxisFlags_AuxDefault);
            ImPlot::SetupAxisLimits(ImAxis_Y2, -0.01, 1.01);

            ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 0, 256);
            ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, 0, 25);
            ImPlot::SetupAxisLimitsConstraints(ImAxis_Y2, -0.01, 1.01);

            ImPlot::PlotBars("log2(nums)", xs.data(), bar.data(), 256, 1.0f);

            ImPlot::SetAxes(ImAxis_X1, ImAxis_Y2);
            ImPlot::SetNextLineStyle(ImVec4(1,153/255.0,153/255.0,1), 2);
            ImPlot::PlotLine("R", xs.data(), RGBA[0].data(), 256);

            ImPlot::SetNextLineStyle(ImVec4(153/255.0,1,153/255.0,1), 2);
            ImPlot::PlotLine("G", xs.data(), RGBA[1].data(), 256);
            
            ImPlot::SetNextLineStyle(ImVec4(153/255.0,153/255.0,1,1), 2);
            ImPlot::PlotLine("B", xs.data(), RGBA[2].data(), 256);
            
            ImPlot::SetNextLineStyle(ImVec4(1,1,1,1), 2);
            ImPlot::PlotLine("A", xs.data(), RGBA[3].data(), 256);

            ImPlot::EndPlot();
        }
    }
}
void draw_streamline_gui(){
    int btnSz = 130;
    ImGui::Text("Load File");
    ImGui::SetNextItemWidth(232);
    if(ImGui::Combo("##loadvecfile", &vecFileIndex.second, vecFileList, IM_ARRAYSIZE(vecFileList)));
    ImGui::SameLine();
    ImGui::SetCursorPosX(250);
    if(ImGui::Button("Load",ImVec2(btnSz, 20))){
        if(vecFileIndex.second != vecFileIndex.first){
            vecFileIndex.first = vecFileIndex.second;
            delete streamline;
            streamline = new Streamline(vecFileList[vecFileIndex.first],streamline_h,streamline_density,streamline_gap,streamline_pointsthreshold1,streamline_pointsthreshold2);
            // todo
            // 設置 rgba
        }
    }
    ImGui::SeparatorText("Set Parameter");
    // ImGui::SameLine();

    int text_len = ImGui::CalcTextSize("Forward Distance").x; 
    
    ImGui::Text("Forward Distance");
    ImGui::SetNextItemWidth(232);
    ImGui::SliderFloat("##streamline_h",&streamline_h,0.1,1);
    ImGui::Spacing();

    ImGui::Text("Line Density ");
    ImGui::SetNextItemWidth(232);
    ImGui::SliderFloat("##streamline_density",&streamline_density,0.1,5);
    ImGui::Spacing();

    ImGui::Text("Gap between two line");
    ImGui::SetNextItemWidth(232);
    ImGui::SliderFloat("##streamline_gap",&streamline_gap,0.1,10);
    ImGui::Spacing();

    ImGui::Text("Minimum sampling points (per line)");
    ImGui::SetNextItemWidth(232);
    ImGui::SliderInt("##streamline_pointsthreshold1",&streamline_pointsthreshold1,10,100);
    ImGui::Spacing();

    ImGui::Text("Maximum sampling points (per line)");
    ImGui::SetNextItemWidth(232);
    ImGui::SliderInt("##streamline_pointsthreshold2",&streamline_pointsthreshold2,100,2000);
    
    ImGui::SameLine();
    ImGui::SetCursorPosX(250);
    if(ImGui::Button("GOGO",ImVec2(btnSz, 20))){
        cout << "load\n";
        delete streamline;
        streamline = new Streamline(vecFileList[vecFileIndex.first],streamline_h,streamline_density,streamline_gap,streamline_pointsthreshold1,streamline_pointsthreshold2);
        // todo
        // 設置 rgba
    }
}
void my_init(){
    RGBA.assign(4,vector<float>(256,0));
    reset_RGBA();

    // method = METHODS::ISO_SURFACE;
    method = METHODS::STREAMLINE;
    projectMethod = PROJECTION_METHODS::ORTHO;
    
    string v,f,dir;
    #ifdef __linux__
        dir = "/home/yu/Desktop/school/Visualization/src/shaders/";
    #else
        dir = "D:\\school\\Visualization\\src\\shaders\\";
    #endif

    streamline_h = 0.1;
    streamline_density = 1;
    streamline_gap = 1;
    streamline_pointsthreshold1 = 50;
    streamline_pointsthreshold2 = 1000;

    if(method == METHODS::ISO_SURFACE){
        renderModeIndex = {0,0};
        modelManager = new ModelManager(METHODS::ISO_SURFACE, modelFileList[modelFileIndex.first],200);
        v = dir + "IsoSurface.vert"; 
        f = dir + "IsoSurface.frag"; 
    }else if(method == METHODS::SLICE_METHOD){
        renderModeIndex = {1,1};
        modelManager = new ModelManager(METHODS::SLICE_METHOD, modelFileList[modelFileIndex.first]);
        v = dir + "SliceMethod.vert"; 
        f = dir + "SliceMethod.frag"; 
    }else if(method == METHODS::RAY_CASTING){
        renderModeIndex = {2,2};
        modelManager = new ModelManager(METHODS::RAY_CASTING, modelFileList[modelFileIndex.first]);
        v = dir + "RayCasting.vert"; 
        f = dir + "RayCasting.frag"; 
    }else if(method == METHODS::STREAMLINE){
        renderModeIndex = {3,3};
        v = dir + "Streamline.vert"; 
        f = dir + "Streamline.frag"; 
        streamline = new Streamline("1.vec",streamline_h,streamline_density,streamline_gap,streamline_pointsthreshold1,streamline_pointsthreshold2);
    }
    else cout << "ERROR: main.cpp modelManager cant find mrthod.\n";

    shader = new Shader(v,f);

    camera = new Camera(glm::vec3(0,0,-200),glm::vec3(0,0,0),glm::vec3(0,1,0),100);
    camera->set_projection_method(projectMethod);
    
    

    xs.clear();
    bar.clear();
    if(method == METHODS::SLICE_METHOD || method == METHODS::RAY_CASTING){
        for(int i=0;i<=256;i++){
            xs.push_back(i);
            bar.push_back(log2(modelManager->isoValueDistributed[i]));
        }
    }
    // string dir = "D:\\school\\Visualization\\src\\asset\\";    
    // string infFile = dir + modelFileList[modelFileIndex.first] + ".inf";
    // string rawFile = dir + modelFileList[modelFileIndex.first] + ".raw";
    // Volume* mv = new Volume(METHODS::VOLUME_RENDERING, infFile, rawFile);
}
void draw_gui(){
    static int btnSz = 130;
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
        if(ImGui::Button("OK",ImVec2(btnSz, 20))){
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
                    delete modelManager;
                    modelManager = new ModelManager(METHODS::ISO_SURFACE, modelFileList[modelFileIndex.first],200);
                    // modelManager->init(METHODS::ISO_SURFACE, modelFileList[modelFileIndex.first], 200);
                }else if(renderModeIndex.first == METHODS::SLICE_METHOD){
                    method =  METHODS::SLICE_METHOD;
                    v += "SliceMethod.vert";
                    f += "SliceMethod.frag";
                    delete modelManager;
                    modelManager = new ModelManager(METHODS::SLICE_METHOD, modelFileList[modelFileIndex.first]);
                    // modelManager->init(METHODS::SLICE_METHOD, modelFileList[modelFileIndex.first]);
                    reset_RGBA();

                    xs.clear();
                    bar.clear();
                    if(method == METHODS::SLICE_METHOD || method == METHODS::RAY_CASTING){
                        for(int i=0;i<=256;i++){
                            xs.push_back(i);
                            bar.push_back(log2(modelManager->isoValueDistributed[i]));
                        }
                    }

                }else if(renderModeIndex.first == METHODS::RAY_CASTING){
                    method =  METHODS::RAY_CASTING;
                    v += "RayCasting.vert";
                    f += "RayCasting.frag";
                    // modelManager->init(METHODS::RAY_CASTING, modelFileList[modelFileIndex.first]);
                    delete modelManager;
                    modelManager = new ModelManager(METHODS::RAY_CASTING, modelFileList[modelFileIndex.first]);
                    
                    reset_RGBA();

                    xs.clear();
                    bar.clear();
                    if(method == METHODS::SLICE_METHOD || method == METHODS::RAY_CASTING){
                        for(int i=0;i<=256;i++){
                            xs.push_back(i);
                            bar.push_back(log2(modelManager->isoValueDistributed[i]));
                        }
                    }

                }else if(renderModeIndex.first == METHODS::STREAMLINE){
                    method =  METHODS::STREAMLINE;
                    v += "Streamline.vert";
                    f += "Streamline.frag";
                }else{
                    cout << "ERROR: main.cpp draw_gui error!\n";
                }
                delete shader;
                shader = new Shader(v,f);
            }
        }
    }
    ImGui::Spacing();
    // Load Model
    if(method == METHODS::ISO_SURFACE || method == METHODS::RAY_CASTING || method == METHODS::SLICE_METHOD){
        ImGui::Text("Load Model");
        ImGui::SetNextItemWidth(232);
        if(ImGui::Combo("##loadfile", &modelFileIndex.second, modelFileList, IM_ARRAYSIZE(modelFileList)));
        ImGui::SameLine();
        ImGui::SetCursorPosX(250);
        if(ImGui::Button("Load",ImVec2(btnSz, 20))){
            if(modelFileIndex.second != modelFileIndex.first){
                modelFileIndex.first = modelFileIndex.second;
                modelManager->init(method, modelFileList[modelFileIndex.first], 200);

                if(method == METHODS::SLICE_METHOD || method == METHODS::RAY_CASTING){
                    reset_RGBA();
                    bar.clear();
                    xs.clear();
                    for(int i=0;i<=256;i++){
                        xs.push_back(i);
                        bar.push_back(log2(modelManager->isoValueDistributed[i]));
                    }
                }
            }
        }
    }
    
    if(method == METHODS::ISO_SURFACE)
        draw_iso_surface_gui();
    else if(method == METHODS::SLICE_METHOD || method == METHODS::RAY_CASTING) 
        draw_volume_rendering_gui();
    else if(method == METHODS::STREAMLINE)
        draw_streamline_gui();

    // 排版
    if(method == METHODS::ISO_SURFACE || method == METHODS::RAY_CASTING || method == METHODS::SLICE_METHOD){
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::SetCursorPosX(250);
        
        if(ImGui::Button("Reset Camera",ImVec2(btnSz, 20))){
            camera -> reset();
        }
    }
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
    ImPlot::ShowDemoWindow();
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
    glfwSetKeyCallback(window,keyboard_callback);

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
    ImPlot::CreateContext();

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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        shader->use();

        if(method == METHODS::ISO_SURFACE){

            shader->set_uniform("projection", camera->get_projection_matrix());
            shader->set_uniform("view", camera->get_view_matrix());
            shader->set_uniform("viewPos", camera->position);
            shader->set_uniform("lightPos",camera->position);

            shader->set_uniform("model",modelManager->get_model_matrix());    

            if(modelManager->autoRY) modelManager->updateFixedRY();
            shader->set_uniform("fixedRY",modelManager->get_fixedRY_matrix());

            shader->set_uniform("clipNormal",clipNormal);
            shader->set_uniform("enableCliped",enableCliped);
            int sz = modelManager->volumeArray.size();
            for(int i=0;i<sz;i++){
                float isV = modelManager->volumeArray[i].isoValue;
                shader->set_uniform("objectHSVColor",glm::vec3(255-isV, 1.0f, 0.5f));
                modelManager->volumeArray[i].draw();
            }
        }else if(method == METHODS::SLICE_METHOD){
            shader->set_uniform("projection", camera->get_projection_matrix());
            shader->set_uniform("view", camera->get_view_matrix());
            shader->set_uniform("viewPos", camera->position);
            shader->set_uniform("lightPos",camera->position);

            shader->set_uniform("model",modelManager->get_model_matrix());    

            if(modelManager->autoRY) modelManager->updateFixedRY();
            shader->set_uniform("fixedRY",modelManager->get_fixedRY_matrix());
            
            shader->set_uniform("texture3d", 0);
            shader->set_uniform("texture1d", 1);
            shader->set_uniform("maxMag",modelManager->volumeArray[0].maxMag);
            shader->set_uniform("minMag",modelManager->volumeArray[0].minMag);
            shader->set_uniform("openPhong",modelManager->openPhong);
            // shader->set_uniform("sliceNum",modelManager->volumeArray[0].sliceNum);
            glm::mat3 model_3x3 = glm::mat3(modelManager->get_fixedRY_matrix()) * glm::mat3(modelManager->get_model_matrix());
            
            glm::vec3 xyplane = (model_3x3 * glm::vec3(0.0f,0.0f,-100.0f)) - camera->position;
            glm::vec3 _xyplane = (model_3x3 * glm::vec3(0.0f,0.0f,100.0f)) - camera->position;
            glm::vec3 xzplane = (model_3x3 * glm::vec3(0.0f,-100.0f,0.0f)) - camera->position;
            glm::vec3 _xzplane = (model_3x3 * glm::vec3(0.0f,100.0f,0.0f)) - camera->position;
            glm::vec3 yzplane = (model_3x3 * glm::vec3(-100.0f,0.0f,0.0f)) - camera->position;
            glm::vec3 _yzplane = (model_3x3 * glm::vec3(100.0f,0.0f,0.0f)) - camera->position;
            
            vector<pair<float,int>> tpv;
            tpv.push_back({glm::l2Norm(xyplane),0});
            tpv.push_back({glm::l2Norm(_xyplane),1});
            tpv.push_back({glm::l2Norm(yzplane),2});
            tpv.push_back({glm::l2Norm(_yzplane),3});
            tpv.push_back({glm::l2Norm(xzplane),4});
            tpv.push_back({glm::l2Norm(_xzplane),5});

            sort(tpv.begin(),tpv.end());
            modelManager->volumeArray[0].draw(tpv[0].second);
        }else if(method == METHODS::RAY_CASTING){

            // camera/projection/view transformation
            shader->set_uniform("projection", camera->get_projection_matrix());
            shader->set_uniform("view", camera->get_view_matrix());
            shader->set_uniform("viewPos", camera->position);
            shader->set_uniform("lightPos",camera->position);

            shader->set_uniform("texture3d", 0);
            shader->set_uniform("texture1d", 1);
            shader->set_uniform("gap", rayCastingGap);
            shader->set_uniform("openPhong",modelManager->openPhong);
            modelManager->volumeArray[0].draw();
        }else if(method == METHODS::STREAMLINE){
 
            shader->set_uniform("projection", camera->get_projection_matrix());
            shader->set_uniform("view", camera->get_view_matrix());
            shader->set_uniform("maxMagnitude", streamline->maxMagnitude);
            shader->set_uniform("minMagnitude", streamline->minMagnitude);
            shader->set_uniform("texture1d", 0);

            streamline -> draw();

        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    ImGui::DestroyContext();
    ImPlot::DestroyContext();
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

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    // cout << action << "\n";
    if(action == 2)
        if(method == METHODS::ISO_SURFACE || method == METHODS::RAY_CASTING || method == METHODS::SLICE_METHOD)
            camera->ProcessKeyDown(key);
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