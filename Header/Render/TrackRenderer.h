#pragma once
#include <vector>

class TrackRenderer {
private:
    unsigned int shader;

    unsigned int VAO;
    unsigned int VBO;

    // How many points to sample along the curve
    size_t numSamples;

    std::vector<float> generateTrackPoints();

    void setupBuffers(std::vector<float> vertices);
public:
    TrackRenderer(unsigned int colorShader, size_t samples = 200);

    ~TrackRenderer();

    void init();

    void render() const;
};