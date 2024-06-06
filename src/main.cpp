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
#include "./header/Sammon.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

METHODS method;
PROJECTION_METHODS projectMethod;
Shader *shader, *ellipseShadder;
Camera *camera;
ModelManager *modelManager;
Streamline *streamline;
Sammon *sammon;

pair<int,int> modelFileIndex = {1,1};
const char* modelFileList[] = { "carp", "engine","golfball", "teddybear"};

pair<int,int> vecFileIndex = {0,0};
const char* vecFileList[] = { "1.vec", "2.vec","3.vec", "4.vec", "5.vec", "6.vec", "7.vec", "8.vec", "9.vec", "10.vec", "11.vec", "12.vec", "13.vec", "14.vec", "15.vec", "16.vec", "19.vec", "20.vec", "21.vec", "22.vec", "23.vec", "rect1.vec", "rect2.vec", "step5_velocity.vec", "test_not_unit.vec", "test_unit.vec"};

pair<int,int> renderModeIndex;
const char* renderModeList[] = { "iso-surface method", "slice method","ray casting method","streamline(RK2 method)", "sammon mapping"};

// iso-surface
int isosurface_enablecliped;
glm::vec4 isosurface_clipnormal;

// ray
float raycasting_gap;

// gui
vector<float> gui_xs, gui_bar;
vector<vector<float>> gui_RGBA;
static int gui_rgba;

// streamline
static float streamline_h, streamline_density, streamline_gap;
static int streamline_points_threshold1, streamline_points_threshold2;
static float streamline_width_ratio_clamp_m, streamline_width_ratio_clamp_M;

// sammon mapping
static int sammon_N;
static float sammon_ball_sz_ratio;
MODE sammon_mode;
static ImVec4 sammon_color_0;
static ImVec4 sammon_color_1;
static ImVec4 sammon_ellipse_color_0;
static ImVec4 sammon_ellipse_color_1;
static float sammon_line_width_ratio;
static bool sammon_show_ellipse;
static bool sammon_nstd[3];
// TODO:
// 1. change marker
void reset_RGBA(){
    cout << "reset gui_RGBA\n";
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
        gui_RGBA[0][i] = RGB.r;
        gui_RGBA[1][i] = RGB.g;
        gui_RGBA[2][i] = RGB.b;
        gui_RGBA[3][i] = 0.05;
    }
}
void reset_RGBA_to_white(){
    cout << "reset gui_RGBA to White\n";
    for(int i=0;i<256;i++){
        gui_RGBA[0][i] = 1;
        gui_RGBA[1][i] = 1;
        gui_RGBA[2][i] = 1;
        gui_RGBA[3][i] = 1;
    }
}
void draw_iso_surface_gui(){
    int btnSz = 130;
    ImGui::Spacing();
    // Add Iso Surface
    {   static int addIsoValue = 128;
        ImGui::Text("Add Iso Surface");
        ImGui::Text("Iso Value");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(162);
        if(ImGui::InputInt("##Iso Value",&addIsoValue)){
            addIsoValue = min(addIsoValue,255);
            addIsoValue = max(addIsoValue,1);
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
    ImGui::SliderFloat("x + ",&isosurface_clipnormal.x,-1.0f,1.0f); // ax + by + cz + d = 0
    ImGui::SameLine();

    ImGui::SetNextItemWidth(50);
    ImGui::SliderFloat("y + ",&isosurface_clipnormal.y,-1.0f,1.0f);
    ImGui::SameLine();

    ImGui::SetNextItemWidth(50);
    ImGui::SliderFloat("z + ",&isosurface_clipnormal.z,-1.0f,1.0f);
    ImGui::SameLine();

    ImGui::SetNextItemWidth(100);
    ImGui::SliderFloat(" = 0",&isosurface_clipnormal.w,-150,150);
    
    if(ImGui::Button("Cross section", ImVec2(btnSz, 20))){
        isosurface_enablecliped ^= 1;
    }
    if(ImGui::IsItemHovered()){
        if(isosurface_enablecliped){
            ImGui::SetTooltip("click to disable cross section");
        }else{
            ImGui::SetTooltip("click to enable cross section");
        }
    }
    ImGui::SameLine();
    if(ImGui::Button("Reset", ImVec2(btnSz, 20))){
        isosurface_clipnormal = glm::vec4(0,1,0,-150);
        modelManager->rotate = glm::vec3(0,0,0);
        isosurface_enablecliped = 0;
    }
}
void draw_volume_rendering_gui(){
    int btnSz = 130;
    ImGui::Spacing();
    {
        ImGui::Text("Use Phong Shadding");ImGui::SameLine();
        ImGui::RadioButton("Yes##Phong", &modelManager->openPhong, 1);ImGui::SameLine();
        ImGui::RadioButton("No##Phong", &modelManager->openPhong, 0);
    }
    if(method == METHODS::SLICE_METHOD){
        static int sliceNum = 512;
        ImGui::Text("Slice num");
        if(ImGui::RadioButton("256##SM", &sliceNum, 256)) modelManager->volumeArray[0].cal_slice(sliceNum);
        ImGui::SameLine();
        if(ImGui::RadioButton("512##SM", &sliceNum, 512)) modelManager->volumeArray[0].cal_slice(sliceNum);
        ImGui::SameLine();
        if(ImGui::RadioButton("1024##SM", &sliceNum, 1024)) modelManager->volumeArray[0].cal_slice(sliceNum);
        ImGui::SameLine();
        if(ImGui::RadioButton("2048##SM", &sliceNum, 2048)) modelManager->volumeArray[0].cal_slice(sliceNum);
        ImGui::SameLine();
        if(ImGui::RadioButton("4096##SM", &sliceNum, 4096)) modelManager->volumeArray[0].cal_slice(sliceNum);
        
        ImGui::Spacing();
        ImGui::Text("Rotate:");
        ImGui::Text("X:");ImGui::SameLine();
        ImGui::SetNextItemWidth(50);
        ImGui::SliderFloat("##x_SM",&modelManager->rotate.x,0,360); // ax + by + cz + d = 0
        ImGui::SameLine();
        ImGui::Text("Y:");ImGui::SameLine();
        ImGui::SetNextItemWidth(50);
        ImGui::SliderFloat("##y_SM",&modelManager->rotate.y,0,360); // ax + by + cz + d = 0

        ImGui::SameLine();
        ImGui::Text("Z:");ImGui::SameLine();
        ImGui::SetNextItemWidth(50);
        ImGui::SliderFloat("##z_SM",&modelManager->rotate.z,0,360); // ax + by + cz + d = 0

        ImGui::SameLine();
        ImGui::SetCursorPosX(250);
        if(ImGui::Button("Auto rotate Y##SM", ImVec2(btnSz, 20))){
            modelManager->autoRY ^= 1;
        }
    } //END IF 
    else if(method == METHODS::RAY_CASTING){
        ImGui::Text("Gap"); ImGui::SameLine();
        ImGui::SetNextItemWidth(btnSz);
        ImGui::SliderFloat("##gap_RC",&raycasting_gap,0.5f,0.01f);
    }
    ImGui::Spacing();
    ImGui::Spacing();

    static int iso1 = 0, iso2 = 0;
    static float val1 = 0.0f,val2 = 0.0f;
    ImGui::SeparatorText("Color Editor");
    {
        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
        if (ImGui::BeginTabBar("MyTabBar##VR", tab_bar_flags)){
            if (ImGui::BeginTabItem("editor1##VR"))
            {   
                ImGui::RadioButton("r##VR", &gui_rgba, 0);ImGui::SameLine();
                ImGui::RadioButton("g##VR", &gui_rgba, 1);ImGui::SameLine();
                ImGui::RadioButton("b##VR", &gui_rgba, 2);ImGui::SameLine();
                ImGui::RadioButton("a##VR", &gui_rgba, 3);

                ImGui::Text("iso1");ImGui::SameLine();
                ImGui::SetNextItemWidth(146); ImGui::SliderInt("##iso1_VR_ed1",&iso1,0,255); ImGui::SameLine();
                ImGui::Text("val1");ImGui::SameLine(); 
                ImGui::SetNextItemWidth(146);ImGui::SliderFloat("##a1_VR_ed1",&val1,0,1); 
            
                ImGui::Text("iso2");ImGui::SameLine();
                ImGui::SetNextItemWidth(146); ImGui::SliderInt("##iso2_VR_ed1",&iso2,0,255); ImGui::SameLine();
                ImGui::Text("val2");ImGui::SameLine(); 
                ImGui::SetNextItemWidth(146);ImGui::SliderFloat("##a2_VR_ed1",&val2,0,1);

                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                if(ImGui::Button("reset##VR_reset_rgba", ImVec2(btnSz/1.5, 18))){
                    reset_RGBA();
                    modelManager->volumeArray[0].create_1dtexture(gui_RGBA); 
                }
                ImGui::SameLine();
                if(ImGui::Button("reset to 0##VR_reset_rgba_to_0", ImVec2(btnSz/1.5, 18))){
                    for(int i=0;i<256;i++){
                        gui_RGBA[0][i] = 0;
                        gui_RGBA[1][i] = 0;
                        gui_RGBA[2][i] = 0;
                        gui_RGBA[3][i] = 0;
                    }
                    modelManager->volumeArray[0].create_1dtexture(gui_RGBA); 
                }
                ImGui::SameLine();
                ImGui::SetCursorPosX(250);
                if(ImGui::Button("Apply##VR_ed1", ImVec2(btnSz, 18))){
                    gui_RGBA[gui_rgba][iso1] = val1;
                    gui_RGBA[gui_rgba][iso2] = val2;
                    float delta = (val2 - val1)/(iso2-iso1);
                    for(int i=iso1+1,j=0;i<iso2;i++,j++){
                        gui_RGBA[gui_rgba][i] = val1 + delta*j;
                    }
                    modelManager->volumeArray[0].create_1dtexture(gui_RGBA); 
                }
                ImGui::EndTabItem();
                
            }
            if (ImGui::BeginTabItem("editor2##VR_ed2"))
            {
                static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
                ImGuiColorEditFlags flags = 0;
                flags |= ImGuiColorEditFlags_AlphaPreviewHalf;
                flags |= ImGuiColorEditFlags_AlphaBar;
                flags |= ImGuiColorEditFlags_PickerHueWheel;
                flags |= ImGuiColorEditFlags_NoInputs;       // Disable all RGB/HSV/Hex displays
                // float w = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.y) * 0.40f;
                ImGui::SetNextItemWidth(165);
                ImGui::ColorPicker4("MyColor##VR_ed2", (float*)&color, flags, NULL);

                ImGui::SameLine(250);
                ImGui::SetNextItemWidth(50);
                ImGui::BeginGroup();
                ImGui::Text("iso1");
                ImGui::SetNextItemWidth(btnSz);ImGui::SliderInt("##iso1_VR_ed2",&iso1,0,255);
                ImGui::Text("iso2");
                ImGui::SetNextItemWidth(btnSz);ImGui::SliderInt("##iso2_VR_ed2",&iso2,0,255);
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                if(ImGui::Button("Apply##VR_ed2", ImVec2(btnSz, 18))){
                    for(int i=iso1;i<=iso2;i++){ // for each iso
                        gui_RGBA[0][i] = color.x;
                        gui_RGBA[1][i] = color.y;
                        gui_RGBA[2][i] = color.z;
                        gui_RGBA[3][i] = color.w;
                    }
                    modelManager->volumeArray[0].create_1dtexture(gui_RGBA); 
                } 
                ImGui::EndGroup();
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
    }

    if(method == METHODS::SLICE_METHOD)ImGui::SetCursorPosY(400);
    else if(method == METHODS::RAY_CASTING) ImGui::SetCursorPosY(350);
    
    if (ImPlot::BeginPlot("Palette##VR")) {
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

        ImPlot::PlotBars("log2(nums)##VR", gui_xs.data(), gui_bar.data(), 256, 1.0f);

        ImPlot::SetAxes(ImAxis_X1, ImAxis_Y2);
        ImPlot::SetNextLineStyle(ImVec4(1,153/255.0,153/255.0,1), 2);
        ImPlot::PlotLine("R##VR", gui_xs.data(), gui_RGBA[0].data(), 256);

        ImPlot::SetNextLineStyle(ImVec4(153/255.0,1,153/255.0,1), 2);
        ImPlot::PlotLine("G##VR", gui_xs.data(), gui_RGBA[1].data(), 256);
        
        ImPlot::SetNextLineStyle(ImVec4(153/255.0,153/255.0,1,1), 2);
        ImPlot::PlotLine("B##VR", gui_xs.data(), gui_RGBA[2].data(), 256);
        
        ImPlot::SetNextLineStyle(ImVec4(1,1,1,1), 2);
        ImPlot::PlotLine("A##VR", gui_xs.data(), gui_RGBA[3].data(), 256);

        ImPlot::EndPlot();
    }
}
void draw_streamline_gui(){
    int btnSz = 130;
    ImGui::Text("Load File");
    ImGui::SetNextItemWidth(232);
    if(ImGui::Combo("##loadvecfile", &vecFileIndex.second, vecFileList, IM_ARRAYSIZE(vecFileList)));
    ImGui::SameLine();
    ImGui::SetCursorPosX(250);
    if(ImGui::Button("Load##SL",ImVec2(btnSz, 20))){
        if(vecFileIndex.second != vecFileIndex.first){
            vecFileIndex.first = vecFileIndex.second;
            delete streamline;
            streamline = new Streamline(vecFileList[vecFileIndex.first],streamline_h,streamline_density,streamline_gap,streamline_points_threshold1,streamline_points_threshold2);
            // 沿用上次的rgba設定
            streamline->create_1dtexture(gui_RGBA);
        }
    }
    
    ImGui::SeparatorText("Streamline Info");
    {
        glm::vec2 res = streamline->get_resolution();
        int res_x = res.x;
        int res_y = res.y;
        string x, y;
        while(res_x) {
            x += res_x % 10 + '0';
            res_x /= 10;
        }
        while(res_y) {
            y += res_y % 10 + '0';
            res_y /= 10;
        }
        reverse(x.begin(),x.end());
        reverse(y.begin(),y.end());
        x = "Resloution:" + x + "x" + y;
        ImGui::Text(x.c_str());
        ImGui::Spacing();

        ImGui::Text("Width Ratio Clamp");
        // ImGui::SameLine();
        ImGui::Text("m");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(166.5);
        ImGui::SliderFloat("##streamline_width_ratio_m",&streamline_width_ratio_clamp_m,0,0.4);
        ImGui::SameLine();
        ImGui::Text("M");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(166.5);
        ImGui::SliderFloat("##streamline_width_ratio_M",&streamline_width_ratio_clamp_M,0.6,1.0);

    }
    ImGui::Spacing();
    ImGui::SeparatorText("Set Parameter");
    {
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

        ImGui::Text("minimum/Maximum sampling points (per line)");
        ImGui::Text("m");ImGui::SameLine();
        ImGui::SetNextItemWidth(166.5);
        ImGui::SliderInt("##streamline_points_threshold1",&streamline_points_threshold1,10,100);
        // ImGui::Spacing();
        
        // ImGui::Text("Maximum sampling points (per line)");
        ImGui::SameLine();
        ImGui::Text("M");ImGui::SameLine();
        ImGui::SetNextItemWidth(166.5);
        ImGui::SliderInt("##streamline_points_threshold2",&streamline_points_threshold2,100,2000);
        
        ImGui::Spacing();
        
       
        if(ImGui::Button("reset##SL_Set_Parameter",ImVec2(btnSz, 20))){
            streamline_h = 0.1;
            streamline_density = 1;
            streamline_gap = 1;
            streamline_points_threshold1 = 50;
            streamline_points_threshold2 = 1000;
            streamline = new Streamline(vecFileList[vecFileIndex.first],streamline_h,streamline_density,streamline_gap,streamline_points_threshold1,streamline_points_threshold2);
            streamline->create_1dtexture(gui_RGBA);
        }
        ImGui::SameLine();
        ImGui::SetCursorPosX(250);

        if(ImGui::Button("Apply##SL_Set_Parameter",ImVec2(btnSz, 20))){
            delete streamline;
            streamline = new Streamline(vecFileList[vecFileIndex.first],streamline_h,streamline_density,streamline_gap,streamline_points_threshold1,streamline_points_threshold2);
            streamline->create_1dtexture(gui_RGBA); 
        }
    }
    ImGui::Spacing();

    static float velocity1 = 0;
    static float velocity2 = 0;
    static float val1 = 0.0f,val2 = 0.0f;
    
    ImGui::SeparatorText("Color Editor");
    {
        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
        if (ImGui::BeginTabBar("MyTabBar##SL_ed1", tab_bar_flags)){
            if (ImGui::BeginTabItem("editor1##SL_ed1"))
            {
                ImGui::RadioButton("r##SL_ed1", &gui_rgba, 0);ImGui::SameLine();
                ImGui::RadioButton("g##SL_ed1", &gui_rgba, 1);ImGui::SameLine();
                ImGui::RadioButton("b##SL_ed1", &gui_rgba, 2);

                
                ImGui::Text("velocity1");ImGui::SameLine();
                ImGui::SetNextItemWidth(128); ImGui::SliderFloat("##v1_SL_ed1",&velocity1,0,1); ImGui::SameLine();
                ImGui::Text("val1");ImGui::SameLine(); 
                ImGui::SetNextItemWidth(128);ImGui::SliderFloat("##a1_SL_ed1",&val1,0,1); 
            
                ImGui::Text("velocity2");ImGui::SameLine();
                ImGui::SetNextItemWidth(128); ImGui::SliderFloat("##v2_SL_ed1",&velocity2,0,1); ImGui::SameLine();
                ImGui::Text("val2");ImGui::SameLine(); 
                ImGui::SetNextItemWidth(128);ImGui::SliderFloat("##a2_SL_ed1",&val2,0,1);

                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                if(ImGui::Button("reset##SL_reset_rgba", ImVec2(btnSz/2, 18))){
                    reset_RGBA();
                    streamline->create_1dtexture(gui_RGBA); 
                }
                ImGui::SameLine();
                if(ImGui::Button("reset to white##SL_reset_rgba_to_white", ImVec2(btnSz, 18))){
                    reset_RGBA_to_white();
                    streamline->create_1dtexture(gui_RGBA); 
                }
                ImGui::SameLine();
                ImGui::SetCursorPosX(250);
                if(ImGui::Button("Apply##SL_ed1", ImVec2(btnSz, 18))){
                    int vel1 = velocity1*256;
                    int vel2 = velocity2*256;
                    gui_RGBA[gui_rgba][vel1] = val1;
                    gui_RGBA[gui_rgba][vel2] = val2;
                    float delta = (val2 - val1)/(vel2-vel1);
                    for(int i=vel1+1,j=0;i<vel2;i++,j++){
                        gui_RGBA[gui_rgba][i] = val1 + delta*j;
                    }
                    streamline -> create_1dtexture(gui_RGBA); 
                }
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("editor2##SL_ed2"))
            {
                static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
                ImGuiColorEditFlags flags = 0;
                flags |= ImGuiColorEditFlags_PickerHueWheel;
                flags |= ImGuiColorEditFlags_NoInputs;       // Disable all RGB/HSV/Hex displays
                // float w = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.y) * 0.40f;
                ImGui::SetNextItemWidth(140);
                ImGui::ColorPicker4("MyColor##SL_ed2", (float*)&color, flags, NULL);

                ImGui::SameLine(250);
                ImGui::SetNextItemWidth(50);
                ImGui::BeginGroup();
                ImGui::Text("velocity1");
                ImGui::SetNextItemWidth(btnSz);ImGui::SliderFloat("##velocity1_SL_ed2",&velocity1,0,1);
                ImGui::Text("velocity2");
                ImGui::SetNextItemWidth(btnSz);ImGui::SliderFloat("##velocity2_SL_ed2",&velocity2,0,1);
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                if(ImGui::Button("Apply##VR_ed2", ImVec2(btnSz, 18))){
                    int vel1 = velocity1*256;
                    int vel2 = velocity2*256;
                    for(int i=vel1;i<=vel2;i++){ // for each iso
                        gui_RGBA[0][i] = color.x;
                        gui_RGBA[1][i] = color.y;
                        gui_RGBA[2][i] = color.z;
                        gui_RGBA[3][i] = color.w;
                    }
                    streamline->create_1dtexture(gui_RGBA); 
                } 
                ImGui::EndGroup();
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
    }

    ImGui::SetCursorPosY(595);
    if (ImPlot::BeginPlot("Palette##SL")) {
        ImPlot::SetupLegend(ImPlotLocation_NorthWest,ImPlotLegendFlags_Horizontal);
        ImPlot::SetupAxis(ImAxis_X1, "velocity ratio");
        ImPlot::SetupAxisLimits(ImAxis_X1, 0, 1);
        ImPlot::SetupAxis(ImAxis_Y1, "", ImPlotAxisFlags_NoLabel|ImPlotAxisFlags_NoTickLabels);
        ImPlot::SetupAxis(ImAxis_Y2, "RGBA value", ImPlotAxisFlags_AuxDefault);

        ImPlot::SetupAxisLimits(ImAxis_Y2, -0.01, 1.01);

        ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 0, 1);
        ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, -0.01, 1.01);
        ImPlot::SetupAxisLimitsConstraints(ImAxis_Y2, -0.01, 1.01);


        ImPlot::SetAxes(ImAxis_X1, ImAxis_Y2);
        ImPlot::SetNextLineStyle(ImVec4(1,153/255.0,153/255.0,1), 2);
        ImPlot::PlotLine("R##SL", gui_xs.data(), gui_RGBA[0].data(), 256);

        ImPlot::SetNextLineStyle(ImVec4(153/255.0,1,153/255.0,1), 2);
        ImPlot::PlotLine("G##SL", gui_xs.data(), gui_RGBA[1].data(), 256);
        
        ImPlot::SetNextLineStyle(ImVec4(153/255.0,153/255.0,1,1), 2);
        ImPlot::PlotLine("B##SL", gui_xs.data(), gui_RGBA[2].data(), 256);

        ImPlot::EndPlot();
    }
}
void draw_sammon_gui(){
    static int tp = 0; 
    int btnSz = 130;
    ImGuiColorEditFlags misc_flags =  ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiTabBarFlags_None;//ImGuiColorEditFlags_NoDragDrop | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);
    
    ImGui::SeparatorText("Class Info");
    {
        ImGui::Text("0 |");
        ImGui::SameLine();
        if(ImGui::ColorEdit4("MyColor##CLASS0", (float*)&sammon_color_0, misc_flags)){
            // cout << sammon_color_0.x << " " << sammon_color_0.y << " " << sammon_color_0.z << "\n";
        }
        ImGui::SameLine();
        ImGui::Text("marker");

        if(sammon_show_ellipse){
            ImGui::SameLine(); 
            ImGui::SetCursorPosX(130);
            ImGui::ColorEdit4("MyColor##CLASS0_ellipse", (float*)&sammon_ellipse_color_0, misc_flags);
            ImGui::SameLine();
            ImGui::Text("ellipse");
        }

        ImGui::Text("1 |");
        ImGui::SameLine();
        if(ImGui::ColorEdit4("MyColor##CLASS1", (float*)&sammon_color_1, misc_flags)){
            // cout << sammon_color_0.x << " " << sammon_color_0.y << " " << sammon_color_0.z << "\n";
        }
        ImGui::SameLine();
        ImGui::Text("marker");

        if(sammon_show_ellipse){
            ImGui::SameLine(); 
            ImGui::SetCursorPosX(130);
            ImGui::ColorEdit4("MyColor##CLASS1_ellipse", (float*)&sammon_ellipse_color_1, misc_flags);
            ImGui::SameLine();
            ImGui::Text("ellipse");
        }
    }
    ImGui::Spacing();

    ImGui::SeparatorText("Set Sammon Parameter");
    {
        if(ImGui::RadioButton("origin##SAMMON", &tp, 0)) {
            sammon_mode = MODE::BEFORE_CALC;
            sammon_show_ellipse = 0;
        }
        ImGui::SameLine();
        if(ImGui::RadioButton("sammon mapping##SAMMON", &tp, 1))  sammon_mode = MODE::AFTER_CALC;
        
        if(sammon_mode == MODE::AFTER_CALC){
            ImGui::SameLine();
            ImGui::SetCursorPosX(250);
            static string sammon_btn = "Show Ellipse##sammon_Show_Ellipse";
            if(sammon_show_ellipse == 0 ){
                sammon_btn = "Show Ellipse##sammon_Show_Ellipse";
            }else {
                sammon_btn = "Hide Ellipse##sammon_Show_Ellipse";
            }
            if(ImGui::Button(sammon_btn.c_str(), ImVec2(btnSz, 20))){
                sammon_show_ellipse ^= 1;
                if(sammon_show_ellipse){
                    if(!sammon_nstd[0] || !sammon_nstd[2]) sammon_nstd[1] = 1;
                }
            }
        }

        ImGui::SetCursorPosY(200);
        ImGui::Text("Point's Num");
        ImGui::SetNextItemWidth(232);
        ImGui::SliderInt("##sammon_N", &sammon_N,40,900);
        ImGui::SameLine();

        ImGui::SetCursorPosX(250);
        if(ImGui::Button("Apply##SAMMON",ImVec2(btnSz, 20))){
            delete sammon;
            sammon = new Sammon("creditcard.dat", sammon_N);
        }
    }    
    
    ImGui::Spacing();

    ImGui::SeparatorText("Set Rander Parameter");
    {
        ImGui::Text("Ball size");
        ImGui::SliderFloat("##sammon_ball_sz_ratio", &sammon_ball_sz_ratio,0.1,1);
        if(sammon_show_ellipse){
            ImGui::Text("Ellipse Line size");
            ImGui::SliderFloat("##sammon_line_width_ratio", &sammon_line_width_ratio,0.2,1);

            ImGui::Spacing();

            ImGui::Text("Ellipse Sigma");
            if(ImGui::IsItemHovered()){
                ImGui::SetTooltip("Ellipses with Sigma (1, 2, 3) Containing (68.27%, 95.45%, 99.73%) of Points");
            }
            ImGui::SameLine();
            ImGui::Checkbox("1##sammon_nstd[0]", &sammon_nstd[0]); ImGui::SameLine();
            ImGui::Checkbox("2##sammon_nstd[1]", &sammon_nstd[1]); ImGui::SameLine();
            ImGui::Checkbox("3##sammon_nstd[2]", &sammon_nstd[2]);
        }
    }
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

                string v,f,g,g2;
                #ifdef __linux__
                    v = "/home/yu/Desktop/school/Visualization/src/shaders/";
                    f = "/home/yu/Desktop/school/Visualization/src/shaders/";
                    g = "/home/yu/Desktop/school/Visualization/src/shaders/";

                #else
                    v = "D:\\school\\Visualization\\src\\shaders\\";
                    f = "D:\\school\\Visualization\\src\\shaders\\";
                    g = "D:\\school\\Visualization\\src\\shaders\\";
                    g2 = "D:\\school\\Visualization\\src\\shaders\\";
                #endif

                if(renderModeIndex.first == METHODS::ISO_SURFACE){
                    method =  METHODS::ISO_SURFACE;
                    v += "IsoSurface.vert";
                    f += "IsoSurface.frag";
                    delete modelManager;
                    modelManager = new ModelManager(METHODS::ISO_SURFACE, modelFileList[modelFileIndex.first],200);
                    delete shader;
                    shader = new Shader(v.c_str(),f.c_str());
                }else if(renderModeIndex.first == METHODS::SLICE_METHOD){
                    method =  METHODS::SLICE_METHOD;
                    v += "SliceMethod.vert";
                    f += "SliceMethod.frag";
                    delete modelManager;
                    modelManager = new ModelManager(METHODS::SLICE_METHOD, modelFileList[modelFileIndex.first]);
                    // modelManager->init(METHODS::SLICE_METHOD, modelFileList[modelFileIndex.first]);
                    reset_RGBA();

                    gui_xs.clear();
                    gui_bar.clear();
                    for(int i=0;i<256;i++){
                        gui_xs.push_back(i);
                        gui_bar.push_back(log2(modelManager->isoValueDistributed[i]));
                    }
                    delete shader;
                    shader = new Shader(v.c_str(),f.c_str());
                }else if(renderModeIndex.first == METHODS::RAY_CASTING){
                    method =  METHODS::RAY_CASTING;
                    v += "RayCasting.vert";
                    f += "RayCasting.frag";
                    // modelManager->init(METHODS::RAY_CASTING, modelFileList[modelFileIndex.first]);
                    delete modelManager;
                    modelManager = new ModelManager(METHODS::RAY_CASTING, modelFileList[modelFileIndex.first]);
                    
                    reset_RGBA();

                    gui_xs.clear();
                    gui_bar.clear();
                    
                    for(int i=0;i<256;i++){
                        gui_xs.push_back(i);
                        gui_bar.push_back(log2(modelManager->isoValueDistributed[i]));
                    }
                    
                    delete shader;
                    shader = new Shader(v.c_str(),f.c_str());

                }else if(renderModeIndex.first == METHODS::STREAMLINE){
                    method =  METHODS::STREAMLINE;
                    v += "Streamline.vert";
                    f += "Streamline.frag";
                    g += "Streamline.geom";
                    camera -> reset();
                    reset_RGBA();
                    gui_xs.clear();
                    for(int i=0;i<256;i++){
                        gui_xs.push_back(i/256.0);
                    }
                    delete shader;
                    shader = new Shader(v.c_str(),f.c_str(),g.c_str());

                    delete streamline;
                    streamline = new Streamline("1.vec",streamline_h,streamline_density,streamline_gap,streamline_points_threshold1,streamline_points_threshold2);
                    streamline->create_1dtexture(gui_RGBA);
                
                }else if(renderModeIndex.first == METHODS::SAMMON_MAPPING){
                    method =  METHODS::SAMMON_MAPPING;
                    v += "Sammon.vert";
                    f += "Sammon.frag";
                    g += "Sammon.geom";
                    g2 += "Ellipse.geom";
                    
                    delete shader;
                    shader = new Shader(v.c_str(),f.c_str(),g.c_str());

                    delete ellipseShadder;
                    ellipseShadder = new Shader(v.c_str(),f.c_str(), g2.c_str());

                    delete sammon;
                    sammon = new Sammon("creditcard.dat",sammon_N);

                    camera->reset(METHODS::SAMMON_MAPPING);
                
                }else{
                    cout << "ERROR: main.cpp draw_gui error!\n";
                }
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
                    gui_bar.clear();
                    gui_xs.clear();
                    for(int i=0;i<=256;i++){
                        gui_xs.push_back(i);
                        gui_bar.push_back(log2(modelManager->isoValueDistributed[i]));
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
    else if(method == METHODS::SAMMON_MAPPING)
        draw_sammon_gui();

    // 排版 reset camera
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::SetCursorPosX(250);
    
    if(ImGui::Button("Reset Camera",ImVec2(btnSz, 20))){
        if(method != METHODS::SAMMON_MAPPING) camera -> reset();
        else camera -> reset(METHODS::SAMMON_MAPPING);
    }
    
    ImGui::End();
    
    if(method == METHODS::ISO_SURFACE){
        //------histogram--------
        ImGui::SetNextWindowBgAlpha(0.35f);
        ImGui::Begin("Histogram");
        ImGui::PlotHistogram("##iso_value_his", modelManager->isoValueDistributed.data(), 256, 0,  NULL, FLT_MAX, FLT_MAX, ImVec2(200, 130));
        int text_len = ImGui::CalcTextSize("Iso-value").x;
        ImGui::SetCursorPosX(100-text_len/2.0);
        ImGui::Text("Iso-value");
        ImGui::End();
    }

    ImGui::ShowDemoWindow(); // Show demo window! :)
    ImPlot::ShowDemoWindow();
}
void my_init(){
    gui_RGBA.assign(4,vector<float>(256,0));

    string v,f,g,dir;
    #ifdef __linux__
        dir = "/home/yu/Desktop/school/Visualization/src/shaders/";
    #else
        dir = "D:\\school\\Visualization\\src\\shaders\\";
    #endif

    reset_RGBA();
    
    // init varibale
    method = METHODS::SAMMON_MAPPING;
    projectMethod = PROJECTION_METHODS::ORTHO;
    
    streamline_h = 0.1;
    streamline_density = 1;
    streamline_gap = 1;
    streamline_points_threshold1 = 50;
    streamline_points_threshold2 = 1000;
    streamline_width_ratio_clamp_m = 0.3;
    streamline_width_ratio_clamp_M = 0.7;
    
    raycasting_gap = 0.3;
    
    sammon_N = 900;
    sammon_mode = MODE::BEFORE_CALC;
    sammon_ball_sz_ratio = 0.5;
    sammon_color_0 = ImVec4(0.0f / 255.0f, 255.0f / 255.0f, 238.0f / 255.0f, 255.0f / 255.0f);
    sammon_color_1 = ImVec4(255.0f / 255.0f, 122.0f / 255.0f, 189.0f / 255.0f, 255.0f / 255.0f);
    sammon_ellipse_color_0 = ImVec4(0.0f / 255.0f, 122.0f / 255.0f, 122.0f / 255.0f, 255.0f / 255.0f);
    sammon_ellipse_color_1 = ImVec4(255.0f / 255.0f, 51.0f / 255.0f, 153.0f / 255.0f, 255.0f / 255.0f);
    sammon_line_width_ratio = 0.5;
    sammon_show_ellipse = 0;
    sammon_nstd[0] = false;
    sammon_nstd[1] = true;
    sammon_nstd[2] = false;

    isosurface_enablecliped = 0;
    isosurface_clipnormal = glm::vec4(0,1,0,-150);

    gui_rgba = 0;

    // init camera
    camera = new Camera(glm::vec3(0,0,-200),glm::vec3(0,0,0),glm::vec3(0,1,0),100);
    camera->set_projection_method(projectMethod);

    // init model
    if(method == METHODS::ISO_SURFACE){
        renderModeIndex = {0,0};
        modelManager = new ModelManager(METHODS::ISO_SURFACE, modelFileList[modelFileIndex.first],200);
        v = dir + "IsoSurface.vert"; 
        f = dir + "IsoSurface.frag"; 
        shader = new Shader(v.c_str(),f.c_str());
    }else if(method == METHODS::SLICE_METHOD){
        renderModeIndex = {1,1};
        modelManager = new ModelManager(METHODS::SLICE_METHOD, modelFileList[modelFileIndex.first]);
        v = dir + "SliceMethod.vert"; 
        f = dir + "SliceMethod.frag"; 
        shader = new Shader(v.c_str(),f.c_str());
    }else if(method == METHODS::RAY_CASTING){
        renderModeIndex = {2,2};
        modelManager = new ModelManager(METHODS::RAY_CASTING, modelFileList[modelFileIndex.first]);
        v = dir + "RayCasting.vert"; 
        f = dir + "RayCasting.frag"; 
        shader = new Shader(v.c_str(),f.c_str());
    }else if(method == METHODS::STREAMLINE){
        renderModeIndex = {3,3};
        v = dir + "Streamline.vert"; 
        f = dir + "Streamline.frag"; 
        g = dir + "Streamline.geom";

        streamline = new Streamline("1.vec",streamline_h,streamline_density,streamline_gap,streamline_points_threshold1,streamline_points_threshold2);
        streamline->create_1dtexture(gui_RGBA);

        shader = new Shader(v.c_str(),f.c_str(),g.c_str());

    }else if(method == METHODS::SAMMON_MAPPING){
        renderModeIndex = {4,4};
        v = dir + "Sammon.vert"; 
        f = dir + "Sammon.frag";
        g = dir + "Sammon.geom";

        string g2 = dir + "Ellipse.geom";

        sammon = new Sammon("creditcard.dat",sammon_N);
        shader = new Shader(v.c_str(),f.c_str(),g.c_str());
        ellipseShadder = new Shader(v.c_str(),f.c_str(), g2.c_str());
        camera->reset(METHODS::SAMMON_MAPPING);
    }
    else cout << "ERROR: main.cpp modelManager cant find mrthod.\n";

    

    gui_xs.clear();
    gui_bar.clear();
    if(method == METHODS::SLICE_METHOD || method == METHODS::RAY_CASTING){
        for(int i=0;i<256;i++){
            gui_xs.push_back(i);
            gui_bar.push_back(log2(modelManager->isoValueDistributed[i]));
        }
    }else if(method == METHODS::STREAMLINE){
        for(int i=0;i<256;i++){
            gui_xs.push_back(i/256.0);
        }
    }
    // string dir = "D:\\school\\Visualization\\src\\asset\\";    
    // string infFile = dir + modelFileList[modelFileIndex.first] + ".inf";
    // string rawFile = dir + modelFileList[modelFileIndex.first] + ".raw";
    // Volume* mv = new Volume(METHODS::VOLUME_RENDERING, infFile, rawFile);
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
        // glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
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

            shader->set_uniform("clipNormal",isosurface_clipnormal);
            shader->set_uniform("enableCliped",isosurface_enablecliped);
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
            shader->set_uniform("gap", raycasting_gap);
            shader->set_uniform("openPhong",modelManager->openPhong);
            modelManager->volumeArray[0].draw();
        }else if(method == METHODS::STREAMLINE){
 
            shader->set_uniform("projection", camera->get_projection_matrix());
            shader->set_uniform("view", camera->get_view_matrix());
            shader->set_uniform("maxMagnitude", streamline->maxMagnitude);
            shader->set_uniform("minMagnitude", streamline->minMagnitude);
            shader->set_uniform("texture1d", 0);
            shader->set_uniform("widthRatioClamp_m",streamline_width_ratio_clamp_m);
            shader->set_uniform("widthRatioClamp_M",streamline_width_ratio_clamp_M);
            streamline -> draw();

        }else if(method == METHODS::SAMMON_MAPPING){
            shader->set_uniform("projection", camera->get_projection_matrix());
            shader->set_uniform("view", camera->get_view_matrix());
            shader->set_uniform("screenW", (float)camera->screenW);
            shader->set_uniform("screenH", (float)camera->screenH);
            shader->set_uniform("ballSizeRatio", sammon_ball_sz_ratio);
            
            shader->set_uniform("color0", glm::vec4(sammon_color_0.x,sammon_color_0.y,sammon_color_0.z,sammon_color_0.w));
            shader->set_uniform("color1", glm::vec4(sammon_color_1.x,sammon_color_1.y,sammon_color_1.z,sammon_color_1.w));
            sammon -> draw(sammon_mode);

            if(sammon_show_ellipse){
                ellipseShadder->use();
                ellipseShadder->set_uniform("projection", camera->get_projection_matrix());
                ellipseShadder->set_uniform("view", camera->get_view_matrix());
                ellipseShadder->set_uniform("screenW", (float)camera->screenW);
                ellipseShadder->set_uniform("screenH", (float)camera->screenH);
                ellipseShadder->set_uniform("color0", glm::vec4(sammon_ellipse_color_0.x,sammon_ellipse_color_0.y,sammon_ellipse_color_0.z,sammon_ellipse_color_0.w));
                ellipseShadder->set_uniform("color1", glm::vec4(sammon_ellipse_color_1.x,sammon_ellipse_color_1.y,sammon_ellipse_color_1.z,sammon_ellipse_color_1.w));
                ellipseShadder->set_uniform("widthRatio", sammon_line_width_ratio);
                for(int nstd=1;nstd<=3;nstd++) 
                    if(sammon_nstd[nstd-1])
                        sammon -> draw_ellipse(nstd);
            }
        }else cout << "error in display func!!\n";
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
            camera->ProcessKeyDown3D(key);
        else if(method == METHODS::STREAMLINE || method == METHODS::SAMMON_MAPPING)
            camera->ProcessKeyDown2D(key);

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
    //     
    // }
}