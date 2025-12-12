#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../Header/Util.h"
#include "../Header/Render/TrackRenderer.h"
#include "../Header/Render/WagonRenderer.h"
#include "../Header/Render/PersonRenderer.h"
#include "../Header/Game/RollerCoaster.h"

RollerCoaster* g_rollerCoaster = nullptr;

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action != GLFW_PRESS) return;

    if (g_rollerCoaster == nullptr) return;

    switch (key)
    {
    case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        break;

    case GLFW_KEY_SPACE:
        g_rollerCoaster->handleAddPassengerSignal();
        break;

    case GLFW_KEY_ENTER:
        g_rollerCoaster->handleStartSignal();
        break;

    case GLFW_KEY_1: case GLFW_KEY_2: case GLFW_KEY_3: case GLFW_KEY_4:
    case GLFW_KEY_5: case GLFW_KEY_6: case GLFW_KEY_7: case GLFW_KEY_8:
        g_rollerCoaster->handleSickSignal(key - GLFW_KEY_1);
    }
}

float screenWidth, screenHeight;

void mouseCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        Vec2 cursorPos = getCursorPosition(window, screenWidth, screenHeight);
        g_rollerCoaster->handleClick(cursorPos.x, cursorPos.y);
    }
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    screenWidth = mode->width, screenHeight = mode->height;

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

    TrackRenderer trackRenderer(colorShader);
    trackRenderer.init();

    WagonRenderer wagonRenderer(textureShader);
    wagonRenderer.init();

    PersonRenderer personRenderer(textureShader);
    personRenderer.init();

    RollerCoaster rollerCoaster;
    g_rollerCoaster = &rollerCoaster;

    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseCallback);

    glClearColor(0.245f, 0.6f, 0.85f, 1.0f);

    double lastTimeForRefresh = glfwGetTime();
    double lastTime = lastTimeForRefresh;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);

        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        rollerCoaster.update(deltaTime);

        trackRenderer.render();
        wagonRenderer.render(rollerCoaster.getTrain().getWagons());
        personRenderer.render(rollerCoaster.getTrain().getAllPassengers());

        glfwSwapBuffers(window);
        limitFps(lastTimeForRefresh);
    }

    g_rollerCoaster = nullptr;
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}