#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../Header/Util.h"
#include "../Header/Render/TrackRenderer.h"

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "RollerCoaster", monitor, NULL);
    if (window == NULL) return endProgram("Failed to create window.");
    glfwMakeContextCurrent(window);

    GLFWcursor* cursor = loadImageToCursor("Resources/cursor.png");
    glfwSetCursor(window, cursor);

    if (glewInit() != GLEW_OK) return endProgram("Failed to initialize GLEW.");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    unsigned colorShader = createShader("Shader/color.vert", "Shader/color.frag");
    unsigned textureShader = createShader("Shader/texture.vert", "Shader/texture.frag");

    Track track;
    TrackRenderer trackRenderer(colorShader);
    trackRenderer.initialize();


    glClearColor(0.145f, 0.588f, 0.745f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        trackRenderer.render(track);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}