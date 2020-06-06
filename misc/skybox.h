#include "../common/common.h"
#include "../camera/camera.h"
#include "../glObjects/vao.h"
#include "../shader/shader.h"

class Skybox {
public:

    Skybox(std::array<std::string, 6> cubeMap);

    void draw(Camera &cam);

private:

    unsigned int loadCubemap(std::array<std::string, 6> faces);

    Shader m_SkyboxShader = Shader("shader/skybox.vert", "shader/skybox.frag");
    unsigned int m_TextureID;
    VertexArray m_VAO;
};