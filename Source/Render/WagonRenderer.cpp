#include <GL/glew.h>
#include "../../Header/Render/WagonRenderer.h"
#include "../../Header/Game/Wagon.h"
#include "../../Header/Util.h"
#include "../../Header/Constants.h"

WagonRenderer::WagonRenderer(unsigned int textureShader)
    : shader(textureShader), VAO(0), VBO(0), texture(0) {
}

WagonRenderer::~WagonRenderer() {
    if (VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
}

void WagonRenderer::setupBuffers() {
    float vertices[] = {
        // Position      // Texture Coords
        -0.04f, -0.05f,    0.0f, 0.0f,    // bottom-left
         0.04f, -0.05f,    1.0f, 0.0f,    // bottom-right
         0.04f, 0.05f,     1.0f, 1.0f,    // top-right
        -0.04f, 0.05f,     0.0f, 1.0f     // top-left
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute (location = 0)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute (location = 1)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void WagonRenderer::init() {
    texture = loadImageToTexture(WAGON_TEXTURE_PATH);
    setupBuffers();
}

void WagonRenderer::render(const Wagon& wagon) const {
    if (VAO == 0) {
        return;
    }

    glUseProgram(shader);

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    float aspect = (float)viewport[2] / (float)viewport[3];
    glUniform1f(glGetUniformLocation(shader, "uAspect"), aspect);

    glUniform1f(glGetUniformLocation(shader, "uX"), wagon.getXPos());
    glUniform1f(glGetUniformLocation(shader, "uY"), wagon.getYPos());

    float rotTan = wagon.getRotationTangent();
    float div = sqrt(1 + rotTan * rotTan);
    float rotSin = rotTan / div;
    float rotCos = 1.0f / div;

    glUniform1f(glGetUniformLocation(shader, "uRotSin"), rotSin);
    glUniform1f(glGetUniformLocation(shader, "uRotCos"), rotCos);

    glUniform1i(glGetUniformLocation(shader, "uHasOverlay"), false);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glBindVertexArray(VAO);
    
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void WagonRenderer::render(const std::vector<Wagon>& wagons) const {
    if (VAO == 0 || wagons.empty()) {
        return;
    }

    glUseProgram(shader);

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    float aspect = (float)viewport[2] / (float)viewport[3];
    glUniform1f(glGetUniformLocation(shader, "uAspect"), aspect);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);

    GLint hasOverlayLoc = glGetUniformLocation(shader, "uHasOverlay");
    glUniform1i(hasOverlayLoc, false);

    GLint xLoc = glGetUniformLocation(shader, "uX");
    GLint yLoc = glGetUniformLocation(shader, "uY");
    GLint sinLoc = glGetUniformLocation(shader, "uRotSin");
    GLint cosLoc = glGetUniformLocation(shader, "uRotCos");

    for (const Wagon wagon : wagons) {
        glUniform1f(xLoc, wagon.getXPos());
        glUniform1f(yLoc, wagon.getYPos());

        float rotTan = wagon.getRotationTangent();
        float div = sqrt(1 + rotTan * rotTan);
        float rotSin = rotTan / div;
        float rotCos = 1.0f / div;

        glUniform1f(sinLoc, rotSin);
        glUniform1f(cosLoc, rotCos);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
}