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
    float vertices[] = {

        // Coordinates        //Normals
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    VertexArray VAO;

    VAO.bufferData(sizeof(vertices), (void *)vertices);
    VAO.addAttrib(GL_FLOAT, 3);
    VAO.addAttrib(GL_FLOAT, 3);

    Shader myShader = Shader("shader/standard.vertexShader", "shader/standard.fragmentShader");
    player = Camera(glm::vec3(-3.0f, -3.0f, -3.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    player.lookAt(glm::vec3(0.0f, 0.0f, 0.0f));

    /* Initializing timer variables */
    double lastFrame = glfwGetTime();
    double currentFrame;
    double deltaTime;
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix[3][0] = 0.0f;
    modelMatrix[3][1] = 0.0f;
    modelMatrix[3][2] = 0.0f;
    glm::mat4 matrix;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

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
        glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

        /* Tick based events */
        glm::vec3 fixedMovement = ( movement.z * glm::normalize(player.getLookingDir() * glm::vec3(1, 0, 1)) + 
                                    movement.x * player.getSideDir() + 
                                    movement.y * glm::vec3(0, 1, 0));
        player.move(fixedMovement);

        /* Binding buffers */
        VAO.use();

        /* Assembling the matrix used */
        matrix = player.getPerspectiveMatrix() * player.getViewMatrix() * modelMatrix;

        /* Setting up shaders */
        myShader.use();
        myShader.setUniform("assembledMatrix", matrix);
        myShader.setUniform("model", modelMatrix);
        myShader.setUniform("playerPos", player.getPosition());
        myShader.setUniform("objColor", glm::vec3(0.6f, 0.30196078431f, 0.0431372549f));

        VAO.drawArrays(36);
        glm::vec3 looking = player.getLookingDir();
        glm::vec3 position = player.getPosition();
        ImGui::Begin("main");
        ImGui::Text("FPS: %f", 1/deltaTime);
        ImGui::Text("Looking direction: %f, %f, %f", looking.x, looking.y, looking.z);
        ImGui::InputFloat3("Position", player.getPositionValuePtr());
        ImGui::Text("MouseX: %f, MouseY: %f", lastXPos, lastYPos);
        ImGui::InputFloat("Movementspeed", &movementspeed, 0.01, 0.1, "%.3f");
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

    VAO.destroy();
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