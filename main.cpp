#include "common/common.h"
#include <deque>
#include "mesh/model.h"
#include "camera/camera.h"
#include "shader/shader.h"
#include "glObjects/vao.h"
#include "glObjects/vbo.h"
#include "misc/skybox.h"
#include "shader/pointlight.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <memory>

#define FPS_HISTORY_SIZE 2000

/* Declaring callbacks */
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_pos_callback(GLFWwindow* window, double xPos, double yPos);

bool cursorMode = false;
bool shadows = false;

float bias = 0.15f;
float diskRadius = 0.05f;

double lastXPos = 0;
double lastYPos = 0;
float movementspeed = 0.3f;
float pi = 3.1415926535897932384626433832;

bool state = false;

Camera player;
glm::vec3 movement;

std::vector<std::shared_ptr<glm::mat4>> modelMatrices;
std::vector<std::shared_ptr<Model>> models;
std::vector<std::shared_ptr<float>> scales;
std::vector<std::shared_ptr<std::array<float, 3>>> positions;
std::vector<std::shared_ptr<std::string>> paths;
char path[64] = "";

std::deque<float> fps_history;

std::array<float, 2> HiLo(std::vector<float> vector) {
    std::array<float, 2> array = {INFINITY, 0};
    for (auto it = vector.begin(); it != vector.end(); it++) {
        if (*it < array[0]) {
            array[0] = *it;
        }
        if (*it > array[1]) {
            array[1] = *it;
        }
    }
    return array;
}

inline void HelpMarker(const char* desc) {
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}


void ModelsWindow() {

    ImGui::Begin("Models");

    std::vector<uint> erase;
    for (uint i = 0; i < (uint)models.size(); i++) {
        char tmp[64]; sprintf(tmp, "Model %d", i);
        if (ImGui::TreeNode(tmp)) {
            if (ImGui::Button("Update")) {
                models[i]->update();
                models[i]->loadToBuffer();
            }
            ImGui::Text("Path: %s", paths[i]->c_str());
            if (ImGui::SliderFloat("Scale", scales[i].get(), 0.0f, 5.0f, "%.4f")) {
                (*modelMatrices[i])[0][0] = *scales[i];
                (*modelMatrices[i])[1][1] = *scales[i];
                (*modelMatrices[i])[2][2] = *scales[i];
            }
            if (ImGui::InputFloat3("Position", positions[i]->data(), 3)) {
                (*modelMatrices[i])[3][0] = (*positions[i])[0];
                (*modelMatrices[i])[3][1] = (*positions[i])[1];
                (*modelMatrices[i])[3][2] = (*positions[i])[2];
            }
            sprintf(tmp, "Groups - %d", (int)models[i]->Groups.size());
            if ( ImGui::TreeNode(tmp)) {
                for (uint x = 0; x < models[i]->Groups.size(); x++) {
                    if (ImGui::TreeNode(models[i]->Groups[x].getName().c_str())) {
                        ImGui::Bullet(); ImGui::ColorEdit3("Ambient", glm::value_ptr(models[i]->Groups[x].material.ambient));
                        ImGui::Bullet(); ImGui::ColorEdit3("Diffuse", glm::value_ptr(models[i]->Groups[x].material.diffuse));
                        ImGui::Bullet(); ImGui::ColorEdit3("Specular", glm::value_ptr(models[i]->Groups[x].material.specular));
                        int tmpint = models[i]->Groups[x].material.illum;
                        ImGui::Bullet(); if (ImGui::InputInt("Illumination model", &tmpint, 1, 1)) {
                            models[i]->Groups[x].material.illum = tmpint;
                        }
                        ImGui::Bullet(); ImGui::InputFloat("Specular exponent", &models[i]->Groups[x].material.specE, 1.0f, 5.0f, "%.1f");
                        ImGui::Bullet(); ImGui::InputFloat("Opacity", &models[i]->Groups[x].material.opacity, 1.0f, 5.0f, "%.3f");
                        ImGui::Bullet(); ImGui::InputFloat("Optical density", &models[i]->Groups[x].material.opticalDensity, 1.0f, 5.0f, "%.3f");
                        ImGui::TreePop();
                    }
                }
                ImGui::TreePop();
            }
            sprintf(tmp, "Meshes - %d", (int)models[i]->Meshes.size());
            if (ImGui::TreeNode(tmp)) {
                for (int j = 0; j < models[i]->Meshes.size(); j++) {
                    if (ImGui::TreeNode(models[i]->Meshes[j].Name.c_str())) {
                        ImGui::Text("Polygon count: %d", (int)models[i]->Meshes[j].Polygons.size());
                        ImGui::Text("Vertices: %d", (int)models[i]->Meshes[j].Vertices.size());
                        if (ImGui::Checkbox("Enabled", &models[i]->Meshes[j].Enabled)) {
                            models[i]->loadToBuffer();
                        };
                        if(ImGui::Button("Force calculate normals")) {
                            models[i]->Meshes[j].updatePolygonNormals(true);
                        }
                        ImGui::TreePop();
                    }
                }
                ImGui::TreePop();
            }
            if (ImGui::Button("Remove")) {
                erase.push_back(i);
            }
            ImGui::TreePop();
        }
    }
    for (auto i = erase.begin(); i != erase.end(); i++) {
        paths.erase(paths.begin() + *i);
        models.erase(models.begin() + *i);
        scales.erase(scales.begin() + *i);
        positions.erase(positions.begin() + *i);
        modelMatrices.erase(modelMatrices.begin() + *i);
    }

    ImGui::Separator();
    ImGui::InputText("Source path", path, 64, ImGuiInputTextFlags_CharsNoBlank);
    ImGui::SameLine(); HelpMarker("Source is relative to the 'models' folder");
    if (ImGui::Button("Add source")) {
        paths.push_back(std::make_shared<std::string>(std::string(path)));
        models.push_back(std::make_shared<Model>(Model()));
        models.back()->loadModel("models/" + std::string(path));
        models.back()->loadToBuffer();
        modelMatrices.push_back(std::make_shared<glm::mat4>(glm::mat4(1)));
        scales.push_back(std::make_shared<float>(float(1)));
        positions.push_back(std::make_shared<std::array<float, 3>>(std::array<float, 3>{0.0f, 0.0f, 0.0f}));
        memset(path, 0, 64);
    }

    ImGui::End();

}

int main(void) {

    fps_history.resize(FPS_HISTORY_SIZE);

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Applying settings to GL */
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwSwapInterval(1);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1000, 600, "Hello World", NULL, NULL);
    glViewport(0, 0, 1000, 600);
    player.updatePerspectiveMatrix(glm::radians(110.0f), (float)1000/(float)600);
    if (!window) {
        glfwTerminate();
        return -1;
    }


    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Misc settings */

    /* Setting callbacks */
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_pos_callback);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));


    /* GL settings */
    GLCall(glEnable(GL_DEPTH_TEST));
    // GLCall(glDisable(GL_DEPTH_TEST));
    GLCall(glCullFace(GL_FRONT_AND_BACK));
    // PointLight myLight(Light{1, glm::vec3(0.7f), {}});
    // myLight.setPosition(glm::vec3(5.0f, 8.0f, 5.0f));

    Skybox mySky = Skybox({
        "misc/skybox/right.jpg",
        "misc/skybox/left.jpg",
        "misc/skybox/top.jpg",
        "misc/skybox/bottom.jpg",
        "misc/skybox/front.jpg",
        "misc/skybox/back.jpg"
    });

    Shader shader = Shader("shader/meshShadowShader.vert", "shader/meshShadowShader.frag");

    player = Camera(glm::vec3(-3.0f, -3.0f, -3.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    player.lookAt(glm::vec3(0.0f, 0.0f, 0.0f));

    PointLight myLight;
    myLight.setResolution(2048, 2048);
    myLight.setColor(glm::vec3(1));

    /* Initializing timer variables */
    double lastFrame = glfwGetTime();
    double currentFrame;
    double deltaTime;
    glm::mat4 matrix;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    float rotAmount = 0.00f;

    bool update = false;

    double frameTime = 0;

    glm::vec3 lightPos = glm::vec3(5.0f, 5.0f, 20.0f);

    float interval[2] = {0.0f, 5.0f};
    io.Fonts->AddFontDefault();
    ImFont *font2 = io.Fonts->AddFontDefault();
    font2->Scale = 1.5f;

    paths.push_back(std::unique_ptr<std::string>(new std::string("SciFi/Corridor.obj")));
    models.push_back(std::make_shared<Model>(Model()));
    models.back()->loadModel("models/SciFi/Corridor.obj");
    models.back()->loadToBuffer();
    modelMatrices.push_back(std::unique_ptr<glm::mat4>(new glm::mat4(1.0f)));
    scales.push_back(std::unique_ptr<float>(new float(0.009f)));
    positions.push_back(std::unique_ptr<std::array<float, 3>>(new std::array<float, 3>{0.0f, 0.0f, 0.0f}));


    (*modelMatrices[0])[0][0] = *scales[0];
    (*modelMatrices[0])[1][1] = *scales[0];
    (*modelMatrices[0])[2][2] = *scales[0];

    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    /* Loop until the user closes the window */

    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();

        G_triangles = 0;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        fps_history.push_back(io.Framerate);
        fps_history.pop_front();

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

        /* Tick based events */
        glm::vec3 fixedMovement = ( movement.z * glm::normalize(player.getLookingDir() * glm::vec3(1, 0, 1)) + 
                                    movement.x * player.getSideDir() + 
                                    movement.y * glm::vec3(0, 1, 0));
        player.move(fixedMovement);

        if (update) {
            update = false;
            // myLight.updateShadowmap(newObject);
        }
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));

        // shader.use();
        // shader.setUniform("playerPos", player.getPosition());
        // shader.setUniform("light.position", player.getPosition());
        // shader.setUniform("light.color", glm::vec3(1.0f, 1.0f, 1.0f));
        // shader.setUniform("ambient", glm::vec3(0.1f));
        // shader.setUniform("assembledMatrix", player.getPerspectiveMatrix() * player.getViewMatrix() * marchingMatrix);
        // shader.setUniform("model", marchingMatrix);
        // mc.Draw(player, &shader);

        shader.use();
        myLight.lightUniform(&shader);
        shader.setUniform("ambient", glm::vec3(0.1f));
        shader.setUniform("shadows", shadows);
        shader.setUniform("diskRadius", diskRadius);
        shader.setUniform("bias", bias);
        shader.setUniform("playerPos", player.getPosition());
        for (uint z = 0; z < models.size(); z++) {
            shader.setUniform("assembledMatrix", player.getPerspectiveMatrix() * player.getViewMatrix() * (*modelMatrices[z]));
            shader.setUniform("model", *modelMatrices[z]);
            models[z]->draw(&shader);
        }
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));

        mySky.draw(player);
        
        glm::vec3 looking = player.getLookingDir();
        glm::vec3 position = player.getPosition();
        static float values[FPS_HISTORY_SIZE] = {};
        float average = 0;

        for(int n = 0; n < FPS_HISTORY_SIZE; n++)
        {
        values[n] = fps_history[n];
        average += fps_history[n];
        }

        average /= FPS_HISTORY_SIZE;
        char overlay[32];
        sprintf(overlay, "avg %.2f fps (%.2f ms)", average, 1000.0f/average);

        ImGui::Begin("main");
        ImGui::Text("Triangles: %d", G_triangles);
        ImGui::PlotLines("", values, IM_ARRAYSIZE(values), 0, overlay, 0.0f, 100.0f, ImVec2(240,60));
        ImGui::Text("Frame time: %f", frameTime);
        std::array<float, 2> highestandlowest = HiLo(std::vector<float>(fps_history.begin(), fps_history.end()));
        ImGui::Text("High: %.3f ms| Low: %.3f ms", 1000.0f/highestandlowest[1], 1000.0f/highestandlowest[0]);
        ImGui::Text("Looking direction: %f, %f, %f", looking.x, looking.y, looking.z);
        ImGui::InputFloat3("Position", player.getPositionValuePtr());
        ImGui::InputFloat3("Light position", glm::value_ptr(*myLight.getPositionReference()));
        if (ImGui::Button("Light to location")) {
            myLight.setPosition(player.getPosition());
        }
        if (ImGui::Button("Update shadowmap")) {
            Shader& s = myLight.BeginShadowMap();
            for (uint z = 0; z < models.size(); z++) {
                s.setUniform("model", *modelMatrices[z]);
                models[z]->draw(&s);
            }
            myLight.EndShadowmap();
        }
        ImGui::Checkbox("Shadows", &shadows);
        ImGui::InputFloat("Disk Radius", &diskRadius, 0.01f, 0.1f, "%.3f");
        ImGui::InputFloat("Bias", &bias, 0.01f, 0.1f, "%.3f");
        ImGui::Text("MouseX: %f, MouseY: %f", lastXPos, lastYPos);
        ImGui::InputFloat("Movementspeed", &movementspeed, 0.01, 0.1, "%.3f");
        ImGui::End();

        ImGui::PushFont(font2);
        ImGui::Begin("Controls");
        ImGui::Text("Escape: Exit Program");
        ImGui::Text("Space: Toggle Cursor");
        ImGui::Text("Alt: Activate Triangle View");
        ImGui::Text("WASDQE: Forward, Left, Backward, Right, Down, Up");
        ImGui::End();
        ImGui::PopFont();

        ModelsWindow();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        frameTime = glfwGetTime() - currentFrame;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    player.updatePerspectiveMatrix(glm::radians(110.0f), (float)width/(float)height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, true);
    }
    int mod = 1;
    if (action == GLFW_PRESS) {
        mod = 1;
    }
    if (action == GLFW_RELEASE) {
        mod = 0;
    }

    switch (key) {
        case GLFW_KEY_W:
            movement.z = movementspeed * mod;
            break;
        
        case GLFW_KEY_S:
            movement.z = -movementspeed * mod;
            break;
        
        case GLFW_KEY_A:
            movement.x = -movementspeed * mod;
            break;
        
        case GLFW_KEY_D:
            movement.x = movementspeed * mod;
            break;
        
        case GLFW_KEY_Q:
            movement.y = -movementspeed * mod;
            break;

        case GLFW_KEY_E:
            movement.y = movementspeed * mod;
            break;

        case GLFW_KEY_SPACE:
            if (mod == 0) break;
            if (cursorMode) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                cursorMode = false;
            }
            else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                cursorMode = true;
            }
            break;

        case GLFW_KEY_LEFT_ALT:
            if (mod == 0) break;
            if (state) {
                GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
                state = false;
            }
            else {
                GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)); 
                state = true;
            }
        
        case GLFW_KEY_LEFT_CONTROL:
            if (mod == 0) break;
            shadows = 1 - shadows;

        default:
            break;
    }
}

void mouse_pos_callback(GLFWwindow* window, double xPos, double yPos) {
    double xPosDiff = xPos - lastXPos;
    double yPosDiff = yPos - lastYPos;

    lastXPos = xPos;
    lastYPos = yPos;
    if (cursorMode) {
        player.rotateY(xPosDiff/1000);
        player.rotateX(-yPosDiff/700);
    }
}