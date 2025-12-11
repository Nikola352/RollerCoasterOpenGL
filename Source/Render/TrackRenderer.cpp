#include <GL/glew.h>
#include "../../Header/Render/TrackRenderer.h"
#include "../../Header/Game/Track.h"

TrackRenderer::TrackRenderer(unsigned int colorShader, size_t samples)
	: shader(colorShader), numSamples(samples), VAO(0), VBO(0) { }

TrackRenderer::~TrackRenderer() {
    if (VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
}

std::vector<float> TrackRenderer::generateTrackPoints() {
    std::vector<float> vertices;
    vertices.reserve(numSamples * 4);

    float point_dist = 2.0f / numSamples;

    for (float x = -1; x <= 1; x += point_dist) {
        float y = getTrackHeightAt(x), s = getTrackSlopeAt(x);

        // Calculate perpendicular direction (normal to the track)
        float len = sqrt(1 + s * s);
        float nx = -s / len;
        float ny = 1.0f / len;

        vertices.push_back(x + nx * 0.01f);
        vertices.push_back(y + ny * 0.01f);

        vertices.push_back(x - nx * 0.01f);
        vertices.push_back(y - ny * 0.01f);
    }

    return vertices;
}

void TrackRenderer::setupBuffers(std::vector<float> vertices) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void TrackRenderer::init() {
    std::vector<float> vertices = generateTrackPoints();
    setupBuffers(vertices);
}

void TrackRenderer::render() const {
    if (VAO == 0) {
        return;
    }

    glUseProgram(shader);

    glUniform3f(glGetUniformLocation(shader, "uColor"), 0.45f, 0.5f, 0.6f); // metallic

    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, numSamples * 2);
}