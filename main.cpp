#include "mesh/mesh.h"
#include "common/common.h"
#include "camera/camera.h"
#include "shader/shader.h"
#include "glObjects/vao.h"
#include "glObjects/vbo.h"
#include "imgui/imgui.h"
#include "imgui/examples/imgui_impl_glfw.h"
#include "imgui/examples/imgui_impl_opengl3.h"

/* Declaring callbacks */
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_pos_callback(GLFWwindow* window, double xPos, double yPos);

bool cursorMode = true;
double lastXPos = 0;
double lastYPos = 0;
float movementspeed = 0.05f;
float pi = 3.1415926535897932384626433832;

bool state = true;

Camera player;
glm::vec3 movement;

int main(void) {
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
    GLCall(glCullFace(GL_FRONT_AND_BACK));

    /* Vertices for a cube */
    // float vertices[] = {

    //     // Coordinates        //Normals
    //     -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    //      0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    //      0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    //      0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    //     -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    //     -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    //     -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    //      0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    //      0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    //      0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    //     -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    //     -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    //     -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    //     -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    //     -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    //     -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    //     -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    //     -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

    //      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
    //      0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    //      0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    //      0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    //      0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
    //      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    //     -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
    //      0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
    //      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    //      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    //     -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    //     -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    //     -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    //      0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    //      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    //      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    //     -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    //     -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    // };

    // std::vector<glm::vec3> verts = {
    //     glm::vec3(-0.5f, -0.5f, 0.5f),
    //     glm::vec3(0.5f, -0.5f, 0.5f),
    //     glm::vec3(0.5f, 0.5f, 0.5f),
    //     glm::vec3(-0.5f, 0.5f, 0.5f),
    //     glm::vec3(-0.5f, -0.5f, -0.5f),
    //     glm::vec3(0.5f, -0.5f, -0.5f),
    //     glm::vec3(0.5f, 0.5f, -0.5f),
    //     glm::vec3(-0.5f, 0.5f, -0.5f)
    // };

    // std::vector<std::array<ushort, 3>> triangles = {
    //     {0, 2, 1},
    //     {0, 2, 3},
    //     {3, 4, 0},
    //     {3, 4, 7},
    //     {4, 1, 0},
    //     {4, 1, 5},
    //     {5, 7, 6},
    //     {5, 7, 4},
    //     {2, 5, 6},
    //     {2, 5, 1},
    //     {7, 2, 6},
    //     {7, 2, 3},
    // };


    Mesh newObject;
    newObject.parseFile("models/Apartment.obj");

    newObject.loadMesh();
    newObject.temp_setPosition(glm::vec3(5.0f, 5.0f, 5.0f));
    newObject.temp_scale(0.01f);

    Shader myShader = Shader("shader/standard.vertexShader", "shader/standard.fragmentShader");
    player = Camera(glm::vec3(-3.0f, -3.0f, -3.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    player.lookAt(glm::vec3(0.0f, 0.0f, 0.0f));

    /* Initializing timer variables */
    double lastFrame = glfwGetTime();
    double currentFrame;
    double deltaTime;
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix[3][0] = 5.0f;
    modelMatrix[3][1] = 5.0f;
    modelMatrix[3][2] = 5.0f;


    glm::mat4 matrix;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    float rotAmount = 0.00f;
    glm::vec3 lightPos = glm::vec3(5.0f, 8.0f, 5.0f);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // printf("Frametime: %fms; %fFPS\n", deltaTime*1000, 1/deltaTime);

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

        /* Tick based events */
        glm::vec3 fixedMovement = ( movement.z * glm::normalize(player.getLookingDir() * glm::vec3(1, 0, 1)) + 
                                    movement.x * player.getSideDir() + 
                                    movement.y * glm::vec3(0, 1, 0));
        player.move(fixedMovement);

        newObject.meshShader.use();
        newObject.meshShader.setUniform("playerPos", player.getPosition());
        newObject.meshShader.setUniform("objColor", glm::vec3(0.6f, 0.30196078431f, 0.0431372549f));
        newObject.meshShader.setUniform("light.color", glm::vec3(0.7f));
        newObject.meshShader.setUniform("light.ambient", glm::vec3(0.1f));
        newObject.meshShader.setUniform("light.position", lightPos);
        newObject.meshShader.setUniform("shininess", 32.0f);
        newObject.draw(player);

        // newObject.temp_resetModel();

        glm::vec3 looking = player.getLookingDir();
        glm::vec3 position = player.getPosition();
        ImGui::Begin("main");
        ImGui::Text("FPS: %f", 1/deltaTime);
        ImGui::Text("Looking direction: %f, %f, %f", looking.x, looking.y, looking.z);
        ImGui::InputFloat3("Position", player.getPositionValuePtr());
        ImGui::InputFloat3("Light position", glm::value_ptr(lightPos));
        ImGui::Text("MouseX: %f, MouseY: %f", lastXPos, lastYPos);
        ImGui::InputFloat("Movementspeed", &movementspeed, 0.01, 0.1, "%.3f");
        ImGui::InputFloat("Object rotation", &rotAmount, 0.001, 0.01, "%.3f");
        ImGui::End();

        ImGui::Begin("matrix");
        ImGui::Text(matrixToString(player.getLookingMatrix()).c_str());
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
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