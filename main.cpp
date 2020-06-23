#include "common/common.h"
#include <deque>
#include "mesh/model.h"
#include "camera/camera.h"
#include "shader/shader.h"
#include "glObjects/vao.h"
#include "glObjects/vbo.h"
#include "misc/skybox.h"
// #include "shader/pointlight.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#define FPS_HISTORY_SIZE 45

/* Declaring callbacks */
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_pos_callback(GLFWwindow* window, double xPos, double yPos);

bool cursorMode = true;
bool shadows = true;

float bias = 0.15f;
float diskRadius = 0.05f;

double lastXPos = 0;
double lastYPos = 0;
float movementspeed = 0.3f;
float pi = 3.1415926535897932384626433832;

bool state = false;

Camera player;
glm::vec3 movement;

std::deque<float> fps_history;

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

    Model myModel = Model();
    myModel.loadModel("models/SciFi/Corridor.obj");
    myModel.loadToBuffer();

    Shader shader = Shader("shader/meshShader.vert", "shader/meshShader.frag");

    player = Camera(glm::vec3(-3.0f, -3.0f, -3.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    player.lookAt(glm::vec3(0.0f, 0.0f, 0.0f));

    // myLight.setResolution(2048, 2048);

    // myLight.updateShadowmap(newObject);

    /* Initializing timer variables */
    double lastFrame = glfwGetTime();
    double currentFrame;
    double deltaTime;
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix[3][0] = 5.0f;
    modelMatrix[3][1] = 5.0f;
    modelMatrix[3][2] = 5.0f;
    modelMatrix[0][0] = 0.01f;
    modelMatrix[1][1] = 0.01f;
    modelMatrix[2][2] = 0.01f;

    float scale = modelMatrix[0][0];
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
    ImFont *font1 = io.Fonts->AddFontDefault();
    ImFont *font2 = io.Fonts->AddFontDefault();
    font2->Scale = 1.5f;

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

        shader.use();
        shader.setUniform("playerPos", player.getPosition());
        shader.setUniform("light.position", lightPos);
        shader.setUniform("light.color", glm::vec3(1.0f, 1.0f, 1.0f));
        shader.setUniform("assembledMatrix", player.getPerspectiveMatrix() * player.getViewMatrix() * modelMatrix);
        shader.setUniform("model", modelMatrix);
        myModel.draw(shader);
                
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
        ImGui::Text("Looking direction: %f, %f, %f", looking.x, looking.y, looking.z);
        ImGui::InputFloat3("Position", player.getPositionValuePtr());
        ImGui::InputFloat3("Light position", glm::value_ptr(lightPos));
        if (ImGui::Button("Light to location")) {
            lightPos = player.getPosition();
        }
        ImGui::Text("MouseX: %f, MouseY: %f", lastXPos, lastYPos);
        ImGui::InputFloat("Movementspeed", &movementspeed, 0.01, 0.1, "%.3f");
        ImGui::InputFloat2("Min, Max", interval);
        if (ImGui::SliderFloat("Scale", &scale, interval[0], interval[1], "%.4f")) {
            modelMatrix[0][0] = scale;
            modelMatrix[1][1] = scale;
            modelMatrix[2][2] = scale;
        }

        ImGui::End();
        ImGui::PushFont(font2);
        ImGui::Begin("Controls");
        ImGui::Text("Escape: Exit Program");
        ImGui::Text("Space: Toggle Cursor");
        ImGui::Text("Alt: Activate Triangle View");
        ImGui::Text("WASDQE: Forward, Left, Backward, Right, Down, Up");
        ImGui::End();
        ImGui::PopFont();

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