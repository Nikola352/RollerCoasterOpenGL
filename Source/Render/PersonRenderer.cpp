#include <GL/glew.h>
#include "../../Header/Render/PersonRenderer.h"
#include "../../Header/Game/Person.h"
#include "../../Header/Util.h"
#include "../../Header/Constants.h"

PersonRenderer::PersonRenderer(unsigned int textureShader)
    : shader(textureShader), VAO(0), VBO(0), normalTexture(0), sickTexture(0), seatbeltTexture(0) 
{}

PersonRenderer::~PersonRenderer() {
    if (VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
}

void PersonRenderer::setupBuffers() {
    float vertices[] = {
        // Position      // Texture Coords
        -0.014f, -0.025f,    0.0f, 0.0f,    // bottom-left
         0.014f, -0.025f,    1.0f, 0.0f,    // bottom-right
         0.014f, 0.025f,     1.0f, 1.0f,    // top-right
        -0.014f, 0.025f,     0.0f, 1.0f     // top-left
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

void PersonRenderer::loadTextures() {
    normalTexture = loadImageToTexture(PERSON_TEXTURE_PATH);
    sickTexture = loadImageToTexture(SICK_PERSON_TEXTURE_PATH);
    seatbeltTexture = loadImageToTexture(SEATBELT_TEXTURE_PATH);
}

void PersonRenderer::init() {
    loadTextures();
    setupBuffers();
}

void PersonRenderer::render(const Person& person) const {
    if (VAO == 0) {
        return;
    }

    glUseProgram(shader);

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    float aspect = (float)viewport[2] / (float)viewport[3];
    glUniform1f(glGetUniformLocation(shader, "uAspect"), aspect);

    glUniform1f(glGetUniformLocation(shader, "uX"), person.getXPos());
    glUniform1f(glGetUniformLocation(shader, "uY"), person.getYPos());

    float rotTan = person.getRotationTangent();
    float div = sqrt(1 + rotTan * rotTan);
    float rotSin = rotTan / div;
    float rotCos = 1.0f / div;

    glUniform1f(glGetUniformLocation(shader, "uRotSin"), rotSin);
    glUniform1f(glGetUniformLocation(shader, "uRotCos"), rotCos);

    glUniform1i(glGetUniformLocation(shader, "uTex"), 0);
    glUniform1i(glGetUniformLocation(shader, "uOverlayTex"), 1);

    glActiveTexture(GL_TEXTURE0);
    if (person.getIsSick()) {
        glBindTexture(GL_TEXTURE_2D, sickTexture);
    }
    else {
        glBindTexture(GL_TEXTURE_2D, normalTexture);
    }

    glUniform1i(glGetUniformLocation(shader, "uHasOverlay"), person.getHasSeatbelt());
    if (person.getHasSeatbelt()) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, seatbeltTexture);
    }

    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void PersonRenderer::render(const std::vector<Person*>& persons) const {
    if (VAO == 0 || persons.empty()) {
        return;
    }

    glUseProgram(shader);

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    float aspect = (float)viewport[2] / (float)viewport[3];
    glUniform1f(glGetUniformLocation(shader, "uAspect"), aspect);

    glBindVertexArray(VAO);

    GLint xLoc = glGetUniformLocation(shader, "uX");
    GLint yLoc = glGetUniformLocation(shader, "uY");
    GLint sinLoc = glGetUniformLocation(shader, "uRotSin");
    GLint cosLoc = glGetUniformLocation(shader, "uRotCos");
    GLint texLoc = glGetUniformLocation(shader, "uTex");
    GLint overlayTexLoc = glGetUniformLocation(shader, "uOverlayTex");
    GLint hasOverlayLoc = glGetUniformLocation(shader, "uHasOverlay");

    glUniform1i(texLoc, 0);
    glUniform1i(overlayTexLoc, 1);

    for (const auto& person : persons) {
        glUniform1f(xLoc, person->getXPos());
        glUniform1f(yLoc, person->getYPos());

        float rotTan = person->getRotationTangent();
        float div = sqrt(1 + rotTan * rotTan);
        float rotSin = rotTan / div;
        float rotCos = 1.0f / div;

        glUniform1f(sinLoc, rotSin);
        glUniform1f(cosLoc, rotCos);

        glActiveTexture(GL_TEXTURE0);
        if (person->getIsSick()) {
            glBindTexture(GL_TEXTURE_2D, sickTexture);
        }
        else {
            glBindTexture(GL_TEXTURE_2D, normalTexture);
        }

        bool hasSeatbelt = person->getHasSeatbelt();
        glUniform1i(hasOverlayLoc, hasSeatbelt);

        if (hasSeatbelt) {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, seatbeltTexture);
        }

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
}