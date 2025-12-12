#include <GL/glew.h>
#include "../../Header/Render/NameRenderer.h"
#include "../../Header/Util.h"
#include "../../Header/Constants.h"

NameRenderer::NameRenderer(unsigned int textureShader)
    : shader(textureShader), VAO(0), VBO(0), texture(0) {
}

NameRenderer::~NameRenderer() {
    if (VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
}

void NameRenderer::setupBuffers() {
    float vertices[] = {
         // Position    // Texture Coords
         0.7f, 0.8f,    0.0f, 0.0f,    // bottom-left
         1.0f, 0.8f,    1.0f, 0.0f,    // bottom-right
         1.0f, 1.0f,    1.0f, 1.0f,    // top-right
         0.7f, 1.0f,    0.0f, 1.0f     // top-left
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

void NameRenderer::init() {
    texture = loadImageToTexture(NAME_TEXTURE_PATH);
    setupBuffers();
}

void NameRenderer::render() const {
    if (VAO == 0) {
        return;
    }

    glUseProgram(shader);

    glUniform1f(glGetUniformLocation(shader, "uX"), 0.0f);
    glUniform1f(glGetUniformLocation(shader, "uY"), 0.0f);
    glUniform1f(glGetUniformLocation(shader, "uRotSin"), 0.0f);
    glUniform1f(glGetUniformLocation(shader, "uRotCos"), 1.0f);
    glUniform1i(glGetUniformLocation(shader, "uHasOverlay"), false);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}