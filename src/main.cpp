#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "./header/Volume.hpp"
#include "./header/Shader.hpp"
#include "./header/Camera.hpp"

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
float fovy = 100;

glm::vec4 clipNormal = glm::vec4(0,1,0,-150);
int main()
{
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
    Shader shader(v,f);

    string inf = "D:\\school\\Visualization\\src\\asset\\Carp.inf";
    string raw = "D:\\school\\Visualization\\src\\asset\\Carp.raw";
    
    Volume volume(inf,raw,30);
    // Volume volume2(inf,raw,200);



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

        
//----------------------
ImGui::SetNextWindowBgAlpha(0.35f);
ImGui::Begin("00957116 C. Y. Wang");
ImGui::Text("Clipped plane:");
ImGui::SetNextItemWidth(50);
ImGui::SliderFloat("x + ",&clipNormal.x,0.0f,1.0f); // ax + by + cz + d = 0
ImGui::SameLine();

ImGui::SetNextItemWidth(50);
ImGui::SliderFloat("y + ",&clipNormal.y,0.0f,1.0f);
ImGui::SameLine();

ImGui::SetNextItemWidth(50);
ImGui::SliderFloat("z + ",&clipNormal.z,0.0f,1.0f);
ImGui::SameLine();

ImGui::SetNextItemWidth(100);
ImGui::SliderFloat(" = 0",&clipNormal.w,-150,150);

if(ImGui::RadioButton("clipped section", enableCliped)){
    enableCliped ^= 1;
}
ImGui::End();

//------histogram--------
ImGui::SetNextWindowBgAlpha(0.35f);
ImGui::Begin("Histogram");
ImGui::PlotHistogram("##iso_value", volume.data.data(), 256, 0,  NULL, FLT_MAX, FLT_MAX, ImVec2(200, 130));
int text_iso_value_len = ImGui::CalcTextSize("Iso-value").x;
ImGui::SetCursorPosX(100-text_iso_value_len/2.0);
ImGui::Text("Iso-value");
ImGui::End();
ImGui::ShowDemoWindow(); // Show demo window! :)
//---------------------

        // input
        processInput(window);

        // render
        // clear the colorbuffer
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
    
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(fovy),(float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 2000.0f);
        shader.set_uniform("projection",projection);


        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 fixedRY = glm::mat4(1.0f);
        fixedRY = glm::rotate(fixedRY, (float)glfwGetTime()* glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        // model = glm::rotate(model, glm::radians((float)20), glm::vec3(1.0f, 0.0f, 1.0f));

        shader.set_uniform("model",model);
        shader.set_uniform("fixedRY",fixedRY);

        shader.set_uniform("clipNormal",clipNormal);
        shader.set_uniform("enableCliped",enableCliped);
        
        // camera/view transformation
        glm::mat4 view = glm::lookAt(glm::vec3(0,0,-200),glm::vec3(0,0,0),glm::vec3(0,1,0));
        shader.set_uniform("view", view);
        shader.set_uniform("viewPos", glm::vec3(0,0,-200));

        glm::vec3 lightPos = glm::vec3(0,0,-300);
        shader.set_uniform("lightPos",lightPos);
        
        shader.set_uniform("objectColor",glm::vec4(1.0f, 76/255.0f, 153/255.0f, 1.0f));
        // volume2.draw();

        shader.set_uniform("objectColor",glm::vec4(0.5f, 0.5f, 0.5f,0.8f));
        volume.draw();

//---------------------
ImGui::Render();
ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//---------------------

        // swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteProgram(shader.ID);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    // if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    //     enableCliped ^= 1;

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
        clipNormal.x--;
    }else if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        clipNormal.x++;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        clipNormal.y--;
    }else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        clipNormal.y++;
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
        clipNormal.z--;
    }else if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        clipNormal.z++;
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
        clipNormal.w--;
        clipNormal.w = max(-200.0f,clipNormal[3]);
    }else if(glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS){
        clipNormal.w++;
        clipNormal.w = min(200.0f,clipNormal[3]);
    }

    
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn){

}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fovy -= (float)yoffset;
    if (fovy < 1.0f)
        fovy = 1.0f;
    if (fovy > 120.0f)
        fovy = 100.0f;
    // camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    if (button == GLFW_MOUSE_BUTTON_RIGHT){
        enableCliped ^= 1;
    }
}